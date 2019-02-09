/*!
 *  \file	main.cc
 */
#include <QApplication>
#include "TU/V4L2++.h"
#include "MainWindow.h"

/************************************************************************
*  global functions							*
************************************************************************/
//! メイン関数
/*!
  \param argc	引数の数(コマンド名を含む)
  \param argv   引数文字列の配列
*/
int
main(int argc, char* argv[])
{
    using	namespace TU;

    QApplication		app(argc, argv);
    MainWindow<V4L2Camera>	mainWindow;

    return app.exec();
}
