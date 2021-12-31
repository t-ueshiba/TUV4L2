#
#  $Id$
#
#################################
#  User customizable macros	#
#################################
PROGRAM		= $(shell basename $(PWD))
#LIBRARY		= lib$(shell basename $(PWD))

VPATH		=

IDLS		=
MOCHDRS		= CmdPane.h

INCDIRS		= -I. -I$(PREFIX)/include -I../../TUIIDC/testIIDCcamera2
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

LIBS		= -lTUQt -lTUV4L2 -lTUTools -lyaml-cpp \
		  $(shell pkg-config --libs Qt5Widgets)
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
		../../TUIIDC++/testIIDCcamera2/MainWindow.h \
		/usr/local/include/TU/Array++.h \
		/usr/local/include/TU/Camera++.h \
		/usr/local/include/TU/Geometry++.h \
		/usr/local/include/TU/Image++.h \
		/usr/local/include/TU/Manip.h \
		/usr/local/include/TU/Minimize.h \
		/usr/local/include/TU/V4L2++.h \
		/usr/local/include/TU/V4L2CameraArray.h \
		/usr/local/include/TU/Vector++.h \
		/usr/local/include/TU/algorithm.h \
		/usr/local/include/TU/iterator.h \
		/usr/local/include/TU/pair.h \
		/usr/local/include/TU/qt/ImageView.h \
		/usr/local/include/TU/qt/Slider.h \
		/usr/local/include/TU/range.h \
		/usr/local/include/TU/tuple.h \
		/usr/local/include/TU/type_traits.h
HDRS		= CmdPane.h \
		ROI_Dialog.h
SRCS		= ROI_Dialog.cc \
		V4L2Specializations.cc \
		main.cc
OBJS		= ROI_Dialog.o \
		V4L2Specializations.o \
		main.o

#include $(PROJECT)/lib/rtc.mk		# IDLHDRS, IDLSRCS, CPPFLAGS, OBJS, LIBS
include $(PROJECT)/lib/qt.mk		# MOCSRCS, OBJS
#include $(PROJECT)/lib/cnoid.mk	# CPPFLAGS, LIBS, LIBDIR
#include $(PROJECT)/lib/lib.mk		# PUBHDRS TARGHDRS
include $(PROJECT)/lib/common.mk
###
ROI_Dialog.o: /usr/local/include/TU/qt/Slider.h ROI_Dialog.h \
	/usr/local/include/TU/V4L2++.h /usr/local/include/TU/Image++.h \
	/usr/local/include/TU/pair.h /usr/local/include/TU/type_traits.h \
	/usr/local/include/TU/Manip.h /usr/local/include/TU/Camera++.h \
	/usr/local/include/TU/Geometry++.h /usr/local/include/TU/Minimize.h \
	/usr/local/include/TU/Vector++.h /usr/local/include/TU/Array++.h \
	/usr/local/include/TU/range.h /usr/local/include/TU/iterator.h \
	/usr/local/include/TU/tuple.h /usr/local/include/TU/algorithm.h
V4L2Specializations.o: /usr/local/include/TU/V4L2CameraArray.h \
	/usr/local/include/TU/V4L2++.h /usr/local/include/TU/Image++.h \
	/usr/local/include/TU/pair.h /usr/local/include/TU/type_traits.h \
	/usr/local/include/TU/Manip.h /usr/local/include/TU/Camera++.h \
	/usr/local/include/TU/Geometry++.h /usr/local/include/TU/Minimize.h \
	/usr/local/include/TU/Vector++.h /usr/local/include/TU/Array++.h \
	/usr/local/include/TU/range.h /usr/local/include/TU/iterator.h \
	/usr/local/include/TU/tuple.h /usr/local/include/TU/algorithm.h \
	/usr/local/include/TU/qt/Slider.h \
	../../TUIIDC++/testIIDCcamera2/MainWindow.h \
	../../TUIIDC++/testIIDCcamera2/CameraWindow.h \
	/usr/local/include/TU/qt/ImageView.h \
	../../TUIIDC++/testIIDCcamera2/CmdPane.h ROI_Dialog.h
main.o: /usr/local/include/TU/V4L2++.h /usr/local/include/TU/Image++.h \
	/usr/local/include/TU/pair.h /usr/local/include/TU/type_traits.h \
	/usr/local/include/TU/Manip.h /usr/local/include/TU/Camera++.h \
	/usr/local/include/TU/Geometry++.h /usr/local/include/TU/Minimize.h \
	/usr/local/include/TU/Vector++.h /usr/local/include/TU/Array++.h \
	/usr/local/include/TU/range.h /usr/local/include/TU/iterator.h \
	/usr/local/include/TU/tuple.h /usr/local/include/TU/algorithm.h \
	../../TUIIDC++/testIIDCcamera2/MainWindow.h \
	../../TUIIDC++/testIIDCcamera2/CameraWindow.h \
	/usr/local/include/TU/qt/ImageView.h \
	../../TUIIDC++/testIIDCcamera2/CmdPane.h
