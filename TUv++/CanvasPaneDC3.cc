/*
 *  $BJ?@.(B9-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%AO:n<T$K$h(B
 *  $B$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$kEy$NCx(B
 *  $B:n8"$r?/32$9$k9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
 *
 *  Copyright 1997-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the creator are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holders or the creator are not responsible for any
 *  damages in the use of this program.
 *  
 *  $Id: CanvasPaneDC3.cc,v 1.4 2007-11-29 07:06:06 ueshiba Exp $
 */
#include "TU/v/CanvasPaneDC3.h"
#include <X11/keysym.h>

namespace TU
{
namespace v
{
/************************************************************************
*  class CanvasPaneDC							*
************************************************************************/
/*
 *  Public member functions
 */
CanvasPaneDC3::CanvasPaneDC3(CanvasPane& parentCanvasPane, u_int w, u_int h)
    :CanvasPaneDC(parentCanvasPane, w, h),
     DC3(DC3::X, 128.0)
{
}

CanvasPaneDC3::~CanvasPaneDC3()
{
}

void
CanvasPaneDC3::callback(CmdId id, CmdVal val)
{
    if (id == Id_KeyPress)
	switch (val)
	{
	  case 'n':
	    *this << TU::v::axis(Z) << TU::v::rotate( 5 * M_PI / 180.0)
		  << TU::v::repaint;
	    break;;
	  case 'm':
	    *this << TU::v::axis(Z) << TU::v::rotate(-5 * M_PI / 180.0)
		  << TU::v::repaint;
	    break;;
	  case 'h':
	    *this << TU::v::axis(Y) << TU::v::rotate( 5 * M_PI / 180.0)
		  << TU::v::repaint;
	    break;;
	  case 'j':
	    *this << TU::v::axis(X) << TU::v::rotate( 5 * M_PI / 180.0)
		  << TU::v::repaint;
	    break;;
	  case 'k':
	    *this << TU::v::axis(X) << TU::v::rotate(-5 * M_PI / 180.0)
		  << TU::v::repaint;
	    break;;
	  case 'l':
	    *this << TU::v::axis(Y) << TU::v::rotate(-5 * M_PI / 180.0)
		  << TU::v::repaint;
	    break;;

	  case 'N':
	    *this << TU::v::axis(Z) << TU::v::translate(-0.05 * getDistance())
		  << TU::v::repaint;
	    break;;
	  case 'M':
	    *this << TU::v::axis(Z) << TU::v::translate( 0.05 * getDistance())
		  << TU::v::repaint;
	    break;;
	  case 'H':
	    *this << TU::v::axis(X) << TU::v::translate( 0.05 * getDistance())
		  << TU::v::repaint;
	    break;;
	  case 'J':
	    *this << TU::v::axis(Y) << TU::v::translate(-0.05 * getDistance())
		  << TU::v::repaint;
	    break;;
	  case 'K':
	    *this << TU::v::axis(Y) << TU::v::translate( 0.05 * getDistance())
		  << TU::v::repaint;
	    break;;
	  case 'L':
	    *this << TU::v::axis(X) << TU::v::translate(-0.05 * getDistance())
		  << TU::v::repaint;
	    break;;
	}
    else
	CanvasPaneDC::callback(id, val);
}

/*
 *  Protected member functions
 */
void
CanvasPaneDC3::initializeGraphics()
{
  // Set initial internal and external parameters.
    setInternal(width() / 2, height() / 2, 800.0, 800.0, 1.0, 1000.0);
    Matrix<double>	Rt(3, 3);
    Rt[0][0] = Rt[2][1] = 1.0;
    Rt[1][2] = -1.0;
    setExternal(Vector<double>(3), Rt);
}

}
}
