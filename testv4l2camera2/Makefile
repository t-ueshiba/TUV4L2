#
#  $Id$
#
#################################
#  User customizable macros	#
#################################
PROGRAM		= $(shell basename $(PWD))
#LIBRARY		= lib$(shell basename $(PWD))

VPATH		=

IIDCDIR		= ../../TUIIDC++/testIIDCcamera2
IDLS		=
MOCHDRS		= CmdPane.h SliderCmd.h

INCDIRS		= -I. -I$(IIDCDIR) -I$(PREFIX)/include
CPPFLAGS	= -DNDEBUG $(shell pkg-config --cflags Qt5Widgets)
CFLAGS		= -O3
NVCCFLAGS	= -O
ifeq ($(shell arch), armv7l)
  CPPFLAGS     += -DNEON
else ifeq ($(shell arch), aarch64)
  CPPFLAGS     += -DNEON
else
  CPPFLAGS     += -DSSE4
  CFLAGS       += -msse4
endif
CCFLAGS		= $(CFLAGS)

LIBS		= $(shell pkg-config --libs Qt5Widgets) \
		  -lTUV4L2++ -lTUTools++ -lyaml-cpp
LINKER		= $(CXX)

BINDIR		= $(PREFIX)/bin
LIBDIR		= $(PREFIX)/lib
INCDIR		= $(PREFIX)/include

#########################
#  Macros set by mkmf	#
#########################
SUFFIX		= .cc:sC .cpp:sC .cu:sC .idl:hC
EXTHDRS		= ../../TUIIDC++/testIIDCcamera2/CameraWindow.h \
		../../TUIIDC++/testIIDCcamera2/CmdPane.h \
		../../TUIIDC++/testIIDCcamera2/ImageView.h \
		../../TUIIDC++/testIIDCcamera2/MainWindow.h \
		../../TUIIDC++/testIIDCcamera2/xpms.h \
		/usr/local/include/TU/Array++.h \
		/usr/local/include/TU/Camera++.h \
		/usr/local/include/TU/Geometry++.h \
		/usr/local/include/TU/IIDC++.h \
		/usr/local/include/TU/Image++.h \
		/usr/local/include/TU/Manip.h \
		/usr/local/include/TU/Minimize.h \
		/usr/local/include/TU/V4L2++.h \
		/usr/local/include/TU/V4L2CameraArray.h \
		/usr/local/include/TU/Vector++.h \
		/usr/local/include/TU/algorithm.h \
		/usr/local/include/TU/iterator.h \
		/usr/local/include/TU/pair.h \
		/usr/local/include/TU/range.h \
		/usr/local/include/TU/tuple.h \
		/usr/local/include/TU/type_traits.h
HDRS		= CmdPane.h \
		ROI_Dialog.h \
		SliderCmd.h
SRCS		= ImageView.cc \
		ROI_Dialog.cc \
		SliderCmd.cc \
		V4L2Specializations.cc \
		main.cc
OBJS		= ImageView.o \
		ROI_Dialog.o \
		SliderCmd.o \
		V4L2Specializations.o \
		main.o

#include $(PROJECT)/lib/rtc.mk		# IDLHDRS, IDLSRCS, CPPFLAGS, OBJS, LIBS
include $(PROJECT)/lib/qt.mk		# MOCSRCS, OBJS
#include $(PROJECT)/lib/cnoid.mk	# CPPFLAGS, LIBS, LIBDIR
#include $(PROJECT)/lib/lib.mk		# PUBHDRS TARGHDRS
include $(PROJECT)/lib/common.mk
###
ImageView.o: ../../TUIIDC++/testIIDCcamera2/ImageView.h \
	/usr/local/include/TU/Image++.h /usr/local/include/TU/pair.h \
	/usr/local/include/TU/type_traits.h /usr/local/include/TU/Manip.h \
	/usr/local/include/TU/Camera++.h /usr/local/include/TU/Geometry++.h \
	/usr/local/include/TU/Minimize.h /usr/local/include/TU/Vector++.h \
	/usr/local/include/TU/Array++.h /usr/local/include/TU/range.h \
	/usr/local/include/TU/iterator.h /usr/local/include/TU/tuple.h \
	/usr/local/include/TU/algorithm.h
ROI_Dialog.o: SliderCmd.h ROI_Dialog.h /usr/local/include/TU/IIDC++.h \
	/usr/local/include/TU/Image++.h /usr/local/include/TU/pair.h \
	/usr/local/include/TU/type_traits.h /usr/local/include/TU/Manip.h \
	/usr/local/include/TU/Camera++.h /usr/local/include/TU/Geometry++.h \
	/usr/local/include/TU/Minimize.h /usr/local/include/TU/Vector++.h \
	/usr/local/include/TU/Array++.h /usr/local/include/TU/range.h \
	/usr/local/include/TU/iterator.h /usr/local/include/TU/tuple.h \
	/usr/local/include/TU/algorithm.h
SliderCmd.o: SliderCmd.h
V4L2Specializations.o: /usr/local/include/TU/V4L2CameraArray.h \
	/usr/local/include/TU/V4L2++.h /usr/local/include/TU/Image++.h \
	/usr/local/include/TU/pair.h /usr/local/include/TU/type_traits.h \
	/usr/local/include/TU/Manip.h /usr/local/include/TU/Camera++.h \
	/usr/local/include/TU/Geometry++.h /usr/local/include/TU/Minimize.h \
	/usr/local/include/TU/Vector++.h /usr/local/include/TU/Array++.h \
	/usr/local/include/TU/range.h /usr/local/include/TU/iterator.h \
	/usr/local/include/TU/tuple.h /usr/local/include/TU/algorithm.h \
	../../TUIIDC++/testIIDCcamera2/MainWindow.h \
	../../TUIIDC++/testIIDCcamera2/CameraWindow.h \
	../../TUIIDC++/testIIDCcamera2/CmdPane.h \
	../../TUIIDC++/testIIDCcamera2/ImageView.h \
	../../TUIIDC++/testIIDCcamera2/xpms.h SliderCmd.h ROI_Dialog.h \
	/usr/local/include/TU/IIDC++.h
main.o: /usr/local/include/TU/V4L2++.h /usr/local/include/TU/Image++.h \
	/usr/local/include/TU/pair.h /usr/local/include/TU/type_traits.h \
	/usr/local/include/TU/Manip.h /usr/local/include/TU/Camera++.h \
	/usr/local/include/TU/Geometry++.h /usr/local/include/TU/Minimize.h \
	/usr/local/include/TU/Vector++.h /usr/local/include/TU/Array++.h \
	/usr/local/include/TU/range.h /usr/local/include/TU/iterator.h \
	/usr/local/include/TU/tuple.h /usr/local/include/TU/algorithm.h \
	../../TUIIDC++/testIIDCcamera2/MainWindow.h \
	../../TUIIDC++/testIIDCcamera2/CameraWindow.h \
	../../TUIIDC++/testIIDCcamera2/CmdPane.h \
	../../TUIIDC++/testIIDCcamera2/ImageView.h \
	../../TUIIDC++/testIIDCcamera2/xpms.h
