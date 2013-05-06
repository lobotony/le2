SYSDIRS=-I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads
INCDIR=${SYSDIRS}

LIBS=-lGLESv2 -lEGL -lm -lbcm_host -L/opt/vc/lib

CPPFLAGS=-fpermissive

COMMONSRC=esUtil.c Hello_Triangle.c

default: le2

le2: ${COMMONSRC}
	g++ ${CPPFLAGS} ${COMMONSRC} -o le2.out ${INCDIR} ${LIBS} 

