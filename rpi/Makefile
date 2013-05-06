SYSDIRS=-I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads

THIRDPARTY=-I../thirdparty
LOSTINCLUDE=-I..
GLOBALINCLUDE=-include ../lost/prefix-shared.h

INCDIR=${SYSDIRS} ${THIRDPARTY} ${LOSTINCLUDE} ${GLOBALINCLUDE}

LIBS=-lGLESv2 -lEGL -lm -lbcm_host -L/opt/vc/lib

LOSTENGINE_DEFS=-DLOST_GL_DEBUG -DLOST_PLATFORM_RPI

CPPFLAGS=${LOSTENGINE_DEFS} -std=c++0x

COMMONSRC=esUtil.c main.cpp
THIRDPARTYSRC=../thirdparty/stb_image.c
LOSTSRC=../lost/StringStream.cpp ../lost/Path.cpp ../lost/Log.cpp

default: le2

le2:  ${THIRDPARTYSRC} ${COMMONSRC} ${LOSTSRC}
	g++ ${CPPFLAGS} ${COMMONSRC} ${LOSTSRC} ${THIRDPARTYSRC} -o le2.out ${INCDIR} ${LIBS} 

