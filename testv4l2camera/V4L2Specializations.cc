/*!
 *  \file	V4L2Specializations.cc
 */
#include <QLabel>
#include <QMenu>
#include <boost/foreach.hpp>
#include "TU/V4L2CameraArray.h"
#include "TU/qt/Slider.h"
#include "MainWindow.h"
#include "ROI_Dialog.h"

#ifndef TUV4L2PP_CONF_DIR
#  define TUV4L2PP_CONF_DIR	"/usr/local/etc"
#endif

namespace TU
{
/************************************************************************
*  global functions							*
************************************************************************/
template <> QString
cameraName(const V4L2Camera& camera)
{
    return QString::fromStdString(camera.dev());
}

namespace qt
{
/************************************************************************
*  static functions							*
************************************************************************/
static std::string
getFormatString(const V4L2Camera& camera)
{
    u_int		fps_n, fps_d;
    camera.getFrameRate(fps_n, fps_d);
    
    std::ostringstream	s;
    s << camera.getName(camera.pixelFormat())
      << '[' << camera.width() << 'x' << camera.height()
      << '@' << fps_n << '/' << fps_d << ']';

    return s.str();
}
    
template <class KEY> static void
addControl(V4L2Camera& camera, const KEY& key, QGridLayout* layout, u_int row)
{
}
    
/************************************************************************
*  class MainWindow<V4L2Camera>						*
************************************************************************/
template <> V4L2Camera
MainWindow<V4L2Camera>::createCamera(int n)
{
    return V4L2Camera(("/dev/video" + std::to_string(n)).c_str());
}

template <> QString
MainWindow<V4L2Camera>::defaultConfigFile()
{
    return tr(TUV4L2PP_CONF_DIR).append("/")
	  .append(V4L2CameraArray::DEFAULT_CAMERA_NAME).append(".conf");
}

/************************************************************************
*  class CameraWindow<V4L2Camera>					*
************************************************************************/
template <> void
CameraWindow<V4L2Camera>::captureAndDisplay()
{
    switch (_camera.pixelFormat())
    {
      case V4L2Camera::BGR24:
	captureRawAndDisplay<BGR>();
	break;

      case V4L2Camera::RGB24:
	captureRawAndDisplay<RGB>();
	break;

      case V4L2Camera::BGR32:
	captureRawAndDisplay<BGRA>();
	break;

      case V4L2Camera::RGB32:
	captureRawAndDisplay<ARGB>();
	break;

      case V4L2Camera::GREY:
	captureRawAndDisplay<uint8_t>();
	break;

      case V4L2Camera::Y16:
	captureRawAndDisplay<uint16_t>();
	break;

      case V4L2Camera::YUYV:
	captureRawAndDisplay<YUYV422>();
	break;

      case V4L2Camera::UYVY:
	captureRawAndDisplay<YUV422>();
	break;

      case V4L2Camera::SBGGR8:
      case V4L2Camera::SGBRG8:
      case V4L2Camera::SGRBG8:
#ifdef V4L2_PIX_FMT_SRGGB8
      case V4L2Camera::SRGGB8:
#endif
	captureBayerAndDisplay();
	break;

      default:
	break;
    }
}

/************************************************************************
*  class CmdPane							*
************************************************************************/
template <> void
CmdPane::addFormatAndFeatureCmds(V4L2Camera& camera)
{
  // PixelFormat label を生成する．
    auto	label = new QLabel(tr(getFormatString(camera).c_str()), _pane);
    _layout->addWidget(label, 1, 0, 1, 3);
    
  // PixelFormat button を生成する．
    auto	button = new QPushButton(tr("Format"), _pane);
    const auto	menu   = new QMenu(button);

    BOOST_FOREACH (auto pixelFormat, camera.availablePixelFormats())
    {
	QMenu*		frameSizeMenu = nullptr;

	BOOST_FOREACH (const auto& frameSize,
		       camera.availableFrameSizes(pixelFormat))
	{
	    if (!frameSizeMenu)
	    {
		frameSizeMenu = new QMenu(menu);

		const auto	pixelFormatAction
				    = new QAction(tr(camera.getName(
							 pixelFormat).c_str()),
						  menu);
		pixelFormatAction->setMenu(frameSizeMenu);
		menu->addAction(pixelFormatAction);
	    }

	    QMenu*	frameRateMenu = nullptr;

	    BOOST_FOREACH (const auto& frameRate,
			   frameSize.availableFrameRates())
	    {
		if (!frameRateMenu)
		{
		    frameRateMenu = new QMenu(frameSizeMenu);

		    const auto	frameSizeAction
					= new QAction(
					    tr(frameSize.getName().c_str()),
					    frameSizeMenu);
		    frameSizeAction->setMenu(frameRateMenu);
		    frameSizeMenu->addAction(frameSizeAction);
		}

		const auto	frameRateAction
					= new QAction(
					    tr(frameRate.getName().c_str()),
					    frameRateMenu);
		frameRateMenu->addAction(frameRateAction);
		_pane->connect(
		    frameRateAction, &QAction::triggered,
		    [&camera, pixelFormat, &frameSize, &frameRate, label]()
		    {
			if (frameRate.fps_n.min == frameRate.fps_n.max &&
			    frameRate.fps_d.min == frameRate.fps_d.max)
			{
			    camera.setFormat(pixelFormat,
					     frameSize.width.max,
					     frameSize.height.max,
					     frameRate.fps_n.min,
					     frameRate.fps_d.max);
			    label->setText(
				tr(getFormatString(camera).c_str()));
			}
		    });
	    }
	}

	// if (camera.pixelFormat() == pixelFormat)
	//     button->setText(tr(camera.getName(pixelFormat).c_str()));
    }
    button->setMenu(menu);
    _layout->addWidget(button, 0, 1, 1, 1);

  // ROI button を生成する．
    button = new QPushButton(tr("Set ROI"), _pane);
    _pane->connect(button, &QPushButton::clicked,
		   [this, &camera]()
		   {
		       try
		       {
			   size_t	u0, v0, width, height;
			   ROI_Dialog::getROI(camera, u0, v0, width, height);
			   camera.setROI(u0, v0, width, height);
		       }
		       catch (const std::exception& err)
		       {
			   QMessageBox::warning(this, tr("Warning"),
						tr(err.what()));
		       }
		   });
    _layout->addWidget(button, 0, 2, 1, 1);

  // Feature button/slider を生成する．
    auto	row = _layout->rowCount();
    BOOST_FOREACH (auto feature, camera.availableFeatures())
    {
	const auto	menuItems = camera.availableMenuItems(feature);
	
	if (menuItems.first == menuItems.second)
	{
	    int	min, max, step;
	    camera.getMinMaxStep(feature, min, max, step);

	    if (min == 0 && max == 1)	// toggle button
	    {
		const auto toggle = new QPushButton(
					  tr(camera.getName(feature).c_str()),
					  _pane);
		toggle->setCheckable(true);
		_pane->connect(toggle, &QPushButton::toggled,
			      [this, &camera, feature, toggle](bool enable)
			      {
				  try
				  {
				      camera.setValue(feature,
						      (enable ? 1 : 0));
				  }
				  catch (const std::exception& err)
				  {
				      toggle->setChecked(
					  camera.getValue(feature));

				      QMessageBox::critical(this, tr("Error"),
							    tr(err.what()));
				  }
			      });
		toggle->setChecked(camera.getValue(feature));
		_layout->addWidget(toggle, row, 1, 1, 2);
	    }
	    else if (min == 0 && max == 0)	// button
	    {
		const auto button = new QPushButton(
					  tr(camera.getName(feature).c_str()),
					  _pane);
		button->setCheckable(false);
		_pane->connect(button, &QPushButton::clicked,
			       [this, &camera, feature]()
			       {
				   try
				   {
				       camera.setValue(feature, 0);
				   }
				   catch (const std::exception& err)
				   {
				       QMessageBox::critical(this, tr("Error"),
							     tr(err.what()));
				   }
			       });
		_layout->addWidget(button, row, 1, 1, 2);
	    }
	    else			// slider
	    {
		const auto label = new QLabel(
					tr(camera.getName(feature).c_str()),
					_pane);
		label->setAlignment(Qt::Alignment(Qt::AlignRight |
						  Qt::AlignVCenter));
		_layout->addWidget(label, row, 0, 1, 1);

		const auto	slider = new Slider(_pane);
		_pane->connect(slider, &Slider::valueChanged,
			       [&camera, feature, slider](double val)
			       {
				   try
				   {
				       camera.setValue(feature, int(val));
				   }
				   catch (const std::exception& err)
				   {
				       slider->setValue(
					   camera.getValue(feature));

				       std::cerr << err.what() << std::endl;
				   }
			       });
		slider->setRange(min, max, step);
		slider->setValue(camera.getValue(feature));
		_layout->addWidget(slider, row, 1, 1, 2);
	    }
	}
	else				// menu button
	{
	    const auto label = new QLabel(tr(camera.getName(feature).c_str()),
					  _pane);
	    label->setAlignment(Qt::Alignment(Qt::AlignRight |
					      Qt::AlignVCenter));
	    _layout->addWidget(label, row, 0, 1, 1);
	    
	    const auto	button = new QPushButton(_pane);
	    const auto	menu   = new QMenu(button);

	    BOOST_FOREACH (const auto& menuItem, menuItems)
	    {
		const auto action = new QAction(tr(menuItem.name.c_str()),
						menu);
		menu->addAction(action);
		_pane->connect(action, &QAction::triggered,
			       [this, &camera, feature, &menuItem, button]()
			       {
				   try
				   {
				       camera.setValue(feature, menuItem.index);
				       button->setText(
					   tr(menuItem.name.c_str()));
				   }
				   catch (const std::exception& err)
				   {
				       QMessageBox::critical(this, tr("Error"),
							     tr(err.what()));
				   }
			       });

		if (camera.getValue(feature) == menuItem.index)
		    button->setText(tr(menuItem.name.c_str()));
	    }

	    button->setMenu(menu);
	    _layout->addWidget(button, row, 1, 1, 2);
	}

	++row;
    }
}

}	// namespace qt
}	// namespace TU
