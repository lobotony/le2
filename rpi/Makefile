INCDIR=-I./Common -I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads
LIBS=-lGLESv2 -lEGL -lm -lbcm_host -L/opt/vc/lib

CPPFLAGS=

COMMONSRC=main.cpp

default: le2

le2: main.cpp
	g++ ${CPPFLAGS} ${COMMONSRC} -o $@ ${INCDIR} ${LIBS} 