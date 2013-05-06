SYSDIRS=-I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads

THIRDPARTY=-I../thirdparty
LOSTINCLUDE=-I..
GLOBALINCLUDE=-include ../lost/prefix-shared.h

INCDIR=${SYSDIRS} ${THIRDPARTY} ${LOSTINCLUDE} ${GLOBALINCLUDE}

LOSTENGINE_DEFS=-DLOST_GL_DEBUG -DLOST_PLATFORM_RPI

CPPSOURCES=main.cpp ../lost/StringStream.cpp ../lost/Path.cpp ../lost/Log.cpp
CSOURCES=esUtil.c ../thirdparty/stb_image.c

CPP=g++
CC=gcc
CPPFLAGS=${LOSTENGINE_DEFS} ${INCDIR} -std=c++0x -c
CFLAGS=${LOSTENGINE_DEFS} ${INCDIR} -c
LDFLAGS=-lGLESv2 -lEGL -lm -lbcm_host -L/opt/vc/lib
EXECUTABLE=le2.out
SOURCES=$(CPPSOURCES) $(CSOURCES)
CPPOBJECTS=$(CPPSOURCES:.cpp=.o)
COBJECTS=$(CSOURCES:.c=.o)
OBJECTS=$(CPPOBJECTS) $(COBJECTS)

default: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CPP) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CPP) $(CPPFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -vf $(OBJECTS)
	rm -vf $(EXECUTABLE)
