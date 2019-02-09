/*!
 *  \file	Format_7_Dialog.cc
 */
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QDialogButtonBox>
#include "SliderCmd.h"
#include "ROI_Dialog.h"

namespace TU
{
/************************************************************************
*  class ROI_Dialog							*
************************************************************************/
ROI_Dialog::ROI_Dialog(const V4L2Camera& camera)
    :_u0(0), _v0(0), _width(camera.width()), _height(camera.height())
{
    if (!camera.getROI(_u0, _v0, _width, _height))
	throw std::runtime_error("No ROI support for this camera." );

    size_t	minU0, minV0, maxWidth, maxHeight;
    camera.getROILimits(minU0, minV0, maxWidth, maxHeight);

    int		row = 0;
    auto	layout = new QGridLayout(this);
    layout->setHorizontalSpacing(4);
    layout->setVerticalSpacing(2);

    auto	label = new QLabel(tr("u0"), this);
    label->setAlignment(Qt::Alignment(Qt::AlignRight | Qt::AlignVCenter));
    const auto	u0 = new SliderCmd(this);
    u0->setRange(minU0, minU0 + maxWidth - 1, 1);
    u0->setValue(_u0);
    layout->addWidget(label, row, 0, 1, 1);
    layout->addWidget(u0,    row, 1, 1, 1);
    ++row;

    label = new QLabel(tr("v0"), this);
    label->setAlignment(Qt::Alignment(Qt::AlignRight | Qt::AlignVCenter));
    const auto	v0 = new SliderCmd(this);
    v0->setRange(minV0, minV0 + maxHeight - 1, 1);
    v0->setValue(_v0);
    layout->addWidget(label, row, 0, 1, 1);
    layout->addWidget(v0,    row, 1, 1, 1);
    ++row;

    label = new QLabel(tr("width"), this);
    label->setAlignment(Qt::Alignment(Qt::AlignRight | Qt::AlignVCenter));
    const auto	width = new SliderCmd(this);
    width->setRange(0, maxWidth, 1);
    width->setValue(_width);
    layout->addWidget(label, row, 0, 1, 1);
    layout->addWidget(width, row, 1, 1, 1);
    ++row;

    label = new QLabel(tr("height"), this);
    label->setAlignment(Qt::Alignment(Qt::AlignRight | Qt::AlignVCenter));
    const auto	height = new SliderCmd(this);
    height->setRange(0, maxHeight, 1);
    height->setValue(_height);
    layout->addWidget(label,  row, 0, 1, 1);
    layout->addWidget(height, row, 1, 1, 1);
    ++row;

    connect(u0, &SliderCmd::valueChanged,
	    [this, maxWidth, width](double val)
	    {
		_u0 = size_t(val);
		if (_u0 + _width > maxWidth)
		{
		    _width = maxWidth - _u0;
		    width->setValue(_width);
		}
	    });
    connect(v0, &SliderCmd::valueChanged,
	    [this, maxHeight, height](double val)
	    {
		_v0 = size_t(val);
		if (_v0 + _height > maxHeight)
		{
		    _height = maxHeight - _v0;
		    height->setValue(_height);
		}
	    });
    connect(width, &SliderCmd::valueChanged,
	    [this, maxWidth, u0](double val)
	    {
		_width = size_t(val);
		if (_u0 + _width > maxWidth)
		{
		    _u0 = 0;
		    u0->setValue(_u0);
		}
	    });
    connect(height, &SliderCmd::valueChanged,
	    [this, maxHeight, v0](double val)
	    {
		_height = size_t(val);
		if (_v0 + _height > maxHeight)
		{
		    _v0 = 0;
		    v0->setValue(_v0);
		}
	    });

    const auto	dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok,
						       Qt::Horizontal, this);
    layout->addWidget(dialogButtonBox, row, 0, 1, 2);

    connect(dialogButtonBox, &QDialogButtonBox::accepted,
	    this, &QDialog::accept);

    exec();
}

void
ROI_Dialog::getROI(const V4L2Camera& camera,
		   size_t& u0, size_t& v0, size_t& width, size_t& height)
{
    const ROI_Dialog	dialog(camera);

    u0	   = dialog._u0;
    v0	   = dialog._v0;
    width  = dialog._width;
    height = dialog._height;
}

}	// namespace TU
