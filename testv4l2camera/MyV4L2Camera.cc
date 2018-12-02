/*
 *  $Id: MyV4L2Camera.cc,v 1.14 2012-08-29 19:35:49 ueshiba Exp $
 */
#include <sys/time.h>
#include <stdexcept>
#include <iostream>
#include "MyV4L2Camera.h"

namespace TU
{
GtkWidget*	createCommands(MyV4L2Camera& camera)			;

/************************************************************************
*  static functions							*
************************************************************************/
//! ループ10回に要した時間の平均をとることにより，フレームレートを測定する．
static void
countTime()
{
    static int			nframes = 0;
    static struct timeval	start;

    if (nframes == 10)
    {
	struct timeval	end;
	gettimeofday(&end, NULL);
	double	interval = (end.tv_sec  - start.tv_sec) +
	    (end.tv_usec - start.tv_usec) / 1.0e6;
	std::cerr << nframes / interval << " frames/sec" << std::endl;
	nframes = 0;
    }
    if (nframes++ == 0)
	gettimeofday(&start, NULL);
}

//! 再描画のためのコールバック関数．
/*!
  \param widget		DrawingArea widget
  \param event		イベント
  \param userdata	MyV4L2Camera (V4L2カメラ)
  \return		TRUEを返す
*/
static gboolean
CBexpose(GtkWidget* widget, GdkEventExpose* event, gpointer userdata)
{
    MyV4L2Camera*	camera = (MyV4L2Camera*)userdata;
    camera->draw();
    return TRUE;
}

/************************************************************************
*  class MyV4L2Camera							*
************************************************************************/
//! V4L2カメラノードを生成する
/*!
  \param dev	カメラのデバイス名
*/
MyV4L2Camera::MyV4L2Camera(const char* dev)
    :V4L2Camera(dev),
     _canvas(gtk_drawing_area_new()),
     _buf(),
     _rgb()
{
    gdk_rgb_init();
    gtk_signal_connect(GTK_OBJECT(_canvas), "expose_event",
		       GTK_SIGNAL_FUNC(CBexpose), (gpointer)this);
    
  // 現在のカメラのフォーマットに合わせてバッファの確保を行う．
    u_int	fps_n, fps_d;
    getFrameRate(fps_n, fps_d);
    setFormat(pixelFormat(), width(), height(), fps_n, fps_d);
}

//! 画素フォーマット，画像サイズおよびフレームレートを設定する
/*!
  \param pixelFormat	設定したい画素フォーマット
  \param width		設定したい画像幅
  \param height		設定したい画像高さ
  \param fps_n		設定したいフレームレートの分子
  \param fps_d		設定したいフレームレートの分母
  \return		このカメラオブジェクト
*/
MyV4L2Camera&
MyV4L2Camera::setFormat(PixelFormat pixFmt,
			size_t w, size_t h, u_int fps_n, u_int fps_d)
{
    V4L2Camera::setFormat(pixFmt, w, h, fps_n, fps_d);
    allocateBuffers();

    return *this;
}

//! 画像のROI(Region of Interest)を設定する
/*!
  \param u0	ROIの左上隅の横座標
  \param v0	ROIの左上隅の縦座標
  \param width	ROIの幅
  \param height	ROIの高さ
  \return	このカメラオブジェクト
*/
MyV4L2Camera&
MyV4L2Camera::setROI(size_t u0, size_t v0, size_t w, size_t h)
{
    V4L2Camera::setROI(u0, v0, w, h);
    allocateBuffers();

    return *this;
}
    
//! カメラから画像を読み込み，canvasに表示する．
/*!
  画像キャプチャボタンが押されている間は，idle関数として他にやることが
  ない時にくりかえし呼ばれる．
*/
void
MyV4L2Camera::idle()
{
  // フレームレートの測定．
    countTime();

  // V4L2Camera から画像データを読み込む．
    if ((pixelFormat() == SBGGR8) ||
	(pixelFormat() == SGBRG8) || (pixelFormat() == SGRBG8))
	snap().captureBayerRaw(_rgb.data());
    else
	snap().captureRaw(_buf.data());
    draw();			// canvasに表示する．
}

//! バッファ中の画像画像をcanvasに表示する．
/*!
  idle(), CBexpose()から呼ばれる．
*/
void
MyV4L2Camera::draw()
{
  // 必用なら YUV -> RGB の変換を行ってから画像を表示．
    switch (pixelFormat())
    {
      case UYVY:
      {
	auto	p = reinterpret_cast<const YUV422*>(_buf.data());
	auto	q = _rgb.data();
	for (size_t y = 0; y < height(); ++y)
	{
	    std::copy(make_pixel_iterator(p), make_pixel_iterator(p + width()),
		      make_pixel_iterator(q));
	    p += width();
	    q += width();
	}
	gdk_draw_rgb_image(_canvas->window,
			   _canvas->style->fg_gc[GTK_WIDGET_STATE(_canvas)],
			   0, 0, width(), height(), GDK_RGB_DITHER_NONE,
			   reinterpret_cast<guchar*>(_rgb.data()), 3*width());
      }
	break;
      case YUYV:
      {
	auto	p = reinterpret_cast<const YUYV422*>(_buf.data());
	auto	q = _rgb.data();
	for (size_t y = 0; y < height(); ++y)
	{
	    std::copy(make_pixel_iterator(p), make_pixel_iterator(p + width()),
		      make_pixel_iterator(q));
	    p += width();
	    q += width();
	}
	gdk_draw_rgb_image(_canvas->window,
			   _canvas->style->fg_gc[GTK_WIDGET_STATE(_canvas)],
			   0, 0, width(), height(), GDK_RGB_DITHER_NONE,
			   reinterpret_cast<guchar*>(_rgb.data()), 3*width());
      }
	break;
      case RGB24:
	gdk_draw_rgb_image(_canvas->window,
			   _canvas->style->fg_gc[GTK_WIDGET_STATE(_canvas)],
			   0, 0, width(), height(), GDK_RGB_DITHER_NONE,
			   _buf.data(), 3*width());
	break;
#ifdef V4L2_PIX_FMT_SRGGB8
      case SRGGB8:
#endif
      case SBGGR8:
      case SGBRG8:
      case SGRBG8:
	gdk_draw_rgb_image(_canvas->window,
			   _canvas->style->fg_gc[GTK_WIDGET_STATE(_canvas)],
			   0, 0, width(), height(), GDK_RGB_DITHER_NONE,
			   reinterpret_cast<guchar*>(_rgb.data()), 3*width());
	break;
      case RGB32:
	gdk_draw_rgb_32_image(_canvas->window,
			      _canvas->style->fg_gc[GTK_WIDGET_STATE(_canvas)],
			      0, 0, width(), height(), GDK_RGB_DITHER_NONE,
			      _buf.data(), 4*width());
	break;
      case GREY:
	gdk_draw_gray_image(_canvas->window,
			    _canvas->style->fg_gc[GTK_WIDGET_STATE(_canvas)],
			    0, 0, width(), height(), GDK_RGB_DITHER_NONE,
			    _buf.data(), width());
	break;
      case Y16:
      {
	auto	p = reinterpret_cast<const u_short*>(_buf.data());
	auto	q = _buf.data();
	for (size_t y = 0; y < height(); ++y)
	    for (size_t x = 0; x < width(); ++x)
		*q++ = *p++;
	gdk_draw_gray_image(_canvas->window,
			    _canvas->style->fg_gc[GTK_WIDGET_STATE(_canvas)],
			    0, 0, width(), height(), GDK_RGB_DITHER_NONE,
			    _buf.data(), width());
      }
	break;
    }
}
 
//! バッファ中の画像をsaveする．
/*!
  モノクロ画像はPGM形式で、カラー画像はPPM形式でsaveされる．
  \param out	画像を書き出す出力ストリーム．
  \return	outで指定した出力ストリーム．
*/
std::ostream&
MyV4L2Camera::save(std::ostream& out) const
{
    using namespace	std;
    
    switch (pixelFormat())
    {
      case UYVY:
      case YUYV:
	out << "P6" << '\n' << width() << ' ' << height() << '\n' << 255
	    << endl;
	out.write(reinterpret_cast<const char*>(_rgb.data()),
		  3*width()*height());
	break;

      case RGB24:
#ifdef V4L2_PIX_FMT_SRGGB8
      case SRGGB8:
#endif
      case SBGGR8:
      case SGBRG8:
      case SGRBG8:
	out << "P6" << '\n' << width() << ' ' << height() << '\n' << 255
	    << endl;
	out.write(reinterpret_cast<const char*>(_buf.data()),
		  3*width()*height());
	break;

      case GREY:
      case Y16:
      {
	  out << "P5" << '\n' << width() << ' ' << height() << '\n' << 255
	      << endl;
	  out.write(reinterpret_cast<const char*>(_buf.data()),
		    width()*height());
      }
	break;
    }
    
    return out;
}
 
void
MyV4L2Camera::allocateBuffers()
{
  // 指定したフォーマットに合わせて入力画像バッファとRGBバッファを再確保する．
    size_t	buffSize;
    switch (pixelFormat())
    {
      case RGB32:
	buffSize = width() * height() * 4;
	break;
      case RGB24:
	buffSize = width() * height() * 3;
	break;
      case Y16:
      case YUYV:
      case UYVY:
	buffSize = width() * height() * 2;
	break;
      case GREY:
#ifdef V4L2_PIX_FMT_SRGGB8
      case SRGGB8:
#endif
      case SBGGR8:
      case SGBRG8:
      case SGRBG8:
	buffSize = width() * height();
	break;
      default:
	throw std::invalid_argument("Unsupported pixel format!!");
	break;
    }
    _buf.resize(buffSize);
    _rgb.resize(width() * height());

  // 指定したフォーマットに合わせてcanvasの大きさを変更する．
    gtk_drawing_area_size(GTK_DRAWING_AREA(_canvas), width(), height());
    gtk_widget_show(_canvas);
}

//! コマンドボタンのコンテナとその親ウィジェットを記録する
/*!
  フォーマット変更のコールバック用にコマンド類とその親ウィジェットを記録する．
  カメラのフォーマットが変更されると，サポートされる機能も変わる可能性がある．
  \param parent 親ウィジェット
  \param commands コマンド類のコンテナウィジェット
 */
void
MyV4L2Camera::setCommands(GtkWidget* commands, GtkWidget* parent)
{
    _commands  = commands;
    _comParent = parent;
    return;
}

//! 現在のカメラの状態に応じてコマンドのコンテナウィジェットを更新する
/*!
  カメラを制御するためのコマンドボタンを一新する．
  カメラのフォーマットが変更されると，サポートされる機能も変わる可能性がある．
 */
void
MyV4L2Camera::refreshCommands()
{
    const auto	table = _comParent;
    const auto	dead  = _commands;
    assert(table != 0 && (dead != 0));
    _commands = createCommands(*this);
    gtk_table_attach(GTK_TABLE(table), _commands,
		     1, 2, 1, 2, GTK_SHRINK, GTK_SHRINK, 5, 0);
  // commandsはGtkTableの1,2,1,2に配置する
    gtk_widget_show_all(table);
    gtk_widget_destroy(dead);
    return;
}

}
