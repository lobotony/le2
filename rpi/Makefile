SYSDIRS=-I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads
THIRDPARTY=-I../thirdparty
GLOBALINCLUDE=-include ../lost/prefix-shared.h
LOSTENGINE_DEFS=-DLOST_GL_DEBUG -DLOST_PLATFORM_RPI
INCDIR=${SYSDIRS} ${THIRDPARTY}

LIBS=-lGLESv2 -lEGL -lm -lbcm_host -L/opt/vc/lib

CPPFLAGS=

COMMONSRC=esUtil.c main.cpp
THIRDPARTYSRC=../thirdparty/stb_image.c

default: le2

le2:  ${THIRDPARTYSRC} ${COMMONSRC}
	g++ ${CPPFLAGS} ${COMMONSRC} ${THIRDPARTYSRC} -o le2.out ${INCDIR} ${LIBS} 

