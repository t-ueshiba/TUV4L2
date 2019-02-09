/*!
 *  \file	Dialog.h
 */
#ifndef TU_ROI_DIALOG_H
#define TU_ROI_DIALOG_H

#include <QDialog>
#include "TU/V4L2++.h"

namespace TU
{
/************************************************************************
*  class ROI_Dialog							*
************************************************************************/
class ROI_Dialog : public QDialog
{
  public:
    static void
    getROI(const V4L2Camera& camera,
	   size_t& u0, size_t& v0, size_t& width, size_t& height)	;

  private:
    ROI_Dialog(const V4L2Camera& camera)				;

  private:
    size_t	_u0, _v0, _width, _height;
};

}	// namespace TU
#endif	// !TU_ROI_DIALOG_H
