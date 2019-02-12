/*
 *  $Id: main.cc,v 1.11 2012-08-29 19:35:49 ueshiba Exp $
 */
/*!
  \mainpage	testv4l2camera - program for testing an Video for Linux v.2 camera
  \anchor	testv4l2camera

  \section copyright 著作権
  Copyright (C) 2003 Toshio UESHIBA
  National Institute of Advanced Industrial Science and Technology (AIST)

  Written by Toshio UESHIBA <t.ueshiba@aist.go.jp>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include "MyV4L2CameraArray.h"

namespace TU
{
GtkWidget*	createMenubar(MyV4L2Camera& camera, GtkWidget* showable);
GtkWidget*	createCommands(MyV4L2Camera& camera)			;
GtkWidget*	createCameraArrayMenubar(MyV4L2CameraArray& cameras)	;
GtkWidget*	createCameraArrayCommands(MyV4L2CameraArray& cameras,
					  GtkWidget* window)		;

/************************************************************************
*  static functions							*
************************************************************************/
//! 使用法を説明する
/*!
  \param s	コマンド名
*/
static void
usage(const char* s)
{
    using namespace	std;
    
    cerr << "\nControl Video for Linux v.2 digital cameras.\n"
	 << endl;
    cerr << " Usage: " << s << " [<device>]"
         << endl;
    cerr << " arguments.\n"
         << "  <device>  device file of the camera\n"
         << endl;
}

//! 一つのカメラだけをテスト
/*!
  \param dev	カメラのデバイスファイル
*/
static void
testV4L2camera(const char* dev)
{
    MyV4L2Camera    camera(dev);	// カメラを開く．
    
    const auto	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "V4L2 camera controller");
    gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, TRUE);
    gtk_signal_connect(GTK_OBJECT(window), "destroy",
		       GTK_SIGNAL_FUNC(gtk_exit), NULL);
    gtk_signal_connect(GTK_OBJECT(window), "delete_event",
		       GTK_SIGNAL_FUNC(gtk_exit), NULL);
    
    const auto	table = gtk_table_new(2, 2, FALSE);
    const auto	commands = createCommands(camera);
    camera.setCommands(commands, table);	// コールバック用に記憶する．
    gtk_container_add(GTK_CONTAINER(window), table);
    gtk_table_attach(GTK_TABLE(table), createMenubar(camera, NULL),
		     0, 2, 0, 1, GTK_FILL, GTK_SHRINK, 0, 0);
    gtk_table_attach(GTK_TABLE(table), commands,
		     1, 2, 1, 2, GTK_SHRINK, GTK_SHRINK, 5, 0);
		     // 1,2,1,2に配置: MyV4L2Camera::refreshCommands()
    gtk_table_attach(GTK_TABLE(table), camera.canvas(), 0, 1, 1, 2,
		     GTK_SHRINK, GTK_SHRINK, 0, 0);
    gtk_widget_show_all(window);
    
    gtk_main();
}

//! ポートに接続している全てのカメラをリストアップ
static void
testV4L2cameras()
{
    MyV4L2CameraArray	cameras;
    
    const auto	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "V4L2 camera controller");
    gtk_widget_set_usize(GTK_WIDGET(window), 450, 280);
    gtk_signal_connect(GTK_OBJECT(window), "destroy",
		       GTK_SIGNAL_FUNC(gtk_exit), NULL);
    gtk_signal_connect(GTK_OBJECT(window), "delete_event",
		       GTK_SIGNAL_FUNC(gtk_exit), NULL);
    
    const auto	table = gtk_table_new(2, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(window), table);
    gtk_table_attach_defaults(GTK_TABLE(table), cameras.canvas(),
			      0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table),
			      createCameraArrayCommands(cameras, window),
			      1, 2, 1, 2);
    gtk_widget_show_all(window);
    
    gtk_main();
}

}

/************************************************************************
*  global functions							*
************************************************************************/
//! メイン関数
/*!
  "-d <device>" でカメラのデバイスファイルを指定する．
  \param argc	引数の数(コマンド名を含む)
  \param argv   引数文字列の配列
*/
int
main(int argc, char* argv[])
{
    using namespace	TU;
    
    gtk_init(&argc, &argv);	// GTK+ の初期化.

    extern char*	optarg;
    for (int c; (c = getopt(argc, argv, "h")) != EOF; )
	switch (c)
	{
	  case 'h':
	    usage(argv[0]);
	    return 1;
	}
    extern int	optind;
    const char*	dev = nullptr;
    if (optind < argc)
	dev = argv[optind];
    
  // 本業を行う．
    try
    {
	if (dev)
	    testV4L2camera(dev);
	else
	    testV4L2cameras();
    }
    catch (const std::exception& err)
    {
	std::cerr << err.what() << std::endl;
	return 1;
    }

    return 0;
}
