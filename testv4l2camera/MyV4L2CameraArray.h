/*
 * testV4L2camera: test program controlling an V4L2 Digital Camera
 * Copyright (C) 2006 VVV Working Group
 *   National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  $Id: MyV4L2CameraArray.h 822 2012-08-27 02:39:41Z takase $
 */
#ifndef __TUMyV4L2CameraArray_h
#define __TUMyV4L2CameraArray_h

#include <gtk/gtk.h>
#include "MyV4L2Camera.h"

namespace TU
{
/************************************************************************
*  class MyV4L2CameraArray						*
************************************************************************/
/*!
  IEEE1394ボードを表すクラス．GTK+ を用いたカメラ一覧表示のための
  canvas (GTK+ の scrolled window widget)，MyV4L2Cameraの各インスタンスを
  生成する機能を持つ．
*/
class MyV4L2CameraArray
{
  public:
    using iterator	 = std::vector<MyV4L2Camera>::iterator;
    using const_iterator = std::vector<MyV4L2Camera>::const_iterator;
    
  public:
    MyV4L2CameraArray()							;
    ~MyV4L2CameraArray()						;

    size_t		size()		const	{ return _cameras.size(); }
    iterator		begin()			{ return _cameras.begin(); }
    const_iterator	begin()		const	{ return _cameras.begin(); }
    iterator		end()			{ return _cameras.end(); }
    const_iterator	end()		const	{ return _cameras.end(); }
    
  //! リストの表示領域となるウィジェットを返す
    GtkWidget*          canvas()				const	;

  //! 現在リストで選択されているカメラの画面を表示する
    GtkWidget*		view()						;

  //! リストの全項目を消去してカメラを解放する
    void		clear()						;

  //! ポートを走査して見つかったV4L2カメラをリストに加える
    void		scan()						;

  //! リストで選択中のカメラについて順番を1つ前にずらす
    void		up()						;

  //! リストで選択中のカメラについて順番を1つ後にずらす
    void		down()						;

  //! 設定ファイルの保存画面ウィジェットを保持する
    void		pushFileSelection(GtkWidget* filesel)		;

  //! 設定ファイルの保存画面ウィジェットを取り出す
    GtkWidget*		popFileSelection()				;

  //! 転送速度選択ウィジェットを設定する
    void		setSpeedPreference(GtkWidget* filesel)		;

  //! 転送速度選択ウィジェットを取得する
    GtkWidget*		getSpeedPreference() const			;

  private:
  //! i番目とj番目のカメラを入れ替える
    void		swap(size_t i, size_t j)			;

  //! カメラを追加する
    void		emplace_back(const char* dev)			;

  private:
    std::vector<MyV4L2Camera>	_cameras;
    
    GtkWidget* const		_canvas;	// リストの表示領域
    GtkWidget*			_list;		// V4L2カメラのリスト
    GtkWidget*			_filesel;	// ファイル選択画面
};

inline GtkWidget*
MyV4L2CameraArray::canvas() const
{
    return _canvas;
}

}
#endif /* !__TUMyV4L2CameraArray_h	*/
