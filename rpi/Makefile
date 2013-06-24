SYSDIRS=-I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vmcs_host/ -I/opt/vc/include/interface/vcos/pthreads

THIRDPARTY=-I../thirdparty -I../thirdparty/utfcpp -I../thirdparty/tinythread/source/ -I../thirdparty/jsoncpp-src-0.5.0/include
LOSTINCLUDE=-I..
GLOBALINCLUDE=-include ../lost/prefix-shared.h

INCDIR=${SYSDIRS} ${THIRDPARTY} ${LOSTINCLUDE} ${GLOBALINCLUDE}

#LOSTENGINE_DEFS=-DLOST_GL_DEBUG -DLOST_PLATFORM_RPI
LOSTENGINE_DEFS=-DLOST_PLATFORM_RPI

CPPSOURCES=main.cpp \
					RunnerRpi.cpp \
					../lost/Bitmap.cpp \
					../lost/BitmapPacker.cpp \
					../lost/Buffer.cpp \
					../lost/BufferLayout.cpp \
					../lost/Bundle.cpp \
					../lost/Camera.cpp \
					../lost/Camera2D.cpp \
					../lost/Camera3D.cpp \
					../lost/Clock.cpp \
					../lost/Color.cpp \
					../lost/Context.cpp \
					../lost/ContextRpi.cpp \
					../lost/Data.cpp \
					../lost/DemoEngine.cpp \
					../lost/Engine.cpp \
					../lost/Event.cpp \
					../lost/EventPool.cpp \
					../lost/EventQueue.cpp \
					../lost/Font.cpp \
					../lost/FragmentShader.cpp \
					../lost/FrameBuffer.cpp \
					../lost/Glyph.cpp \
					../lost/HostBuffer.cpp \
					../lost/HybridBuffer.cpp \
					../lost/HybridIndexBuffer.cpp \
					../lost/HybridVertexBuffer.cpp \
					../lost/lgl.cpp \
					../lost/Log.cpp \
					../lost/Material.cpp \
					../lost/Matrix.cpp \
					../lost/Mesh.cpp \
					../lost/MeshAlgo.cpp \
					../lost/Path.cpp \
					../lost/PlatformLinux.cpp \
					../lost/Quad.cpp \
					../lost/Rect.cpp \
					../lost/RectPacker.cpp \
					../lost/RenderBuffer.cpp \
					../lost/ResourceManager.cpp \
					../lost/Runner.cpp \
					../lost/Shader.cpp \
					../lost/ShaderPreprocessor.cpp \
					../lost/ShaderProgram.cpp \
					../lost/StringAdditions.cpp \
					../lost/StringStream.cpp \
					../lost/TextBuffer.cpp \
					../lost/TextMesh.cpp \
					../lost/TextRender.cpp \
					../lost/Texture.cpp \
					../lost/TruetypeFont.cpp \
					../lost/Uniform.cpp \
					../lost/UniformBlock.cpp \
					../lost/Vec2.cpp \
					../lost/Vec3.cpp \
					../lost/Vec4.cpp \
					../lost/VertexAttribute.cpp \
					../lost/VertexShader.cpp \
					../thirdparty/tinythread/source/tinythread.cpp \
					../thirdparty/jsoncpp-src-0.5.0/src/lib_json/json_reader.cpp \
					../thirdparty/jsoncpp-src-0.5.0/src/lib_json/json_value.cpp \
					../thirdparty/jsoncpp-src-0.5.0/src/lib_json/json_writer.cpp \
					../guiro/EventSystem.cpp \
					../guiro/Frame.cpp \
					../guiro/layers/Layer.cpp \
					../guiro/RenderSystem.cpp \
					../guiro/UpdateSystem.cpp \
					../guiro/UserInterface.cpp \
					../guiro/views/View.cpp

CSOURCES=../thirdparty/stb_image.c

CPP=g++
CC=gcc
COMMONFLAGS=
CPPFLAGS=${LOSTENGINE_DEFS} ${INCDIR} -std=c++0x -c
CFLAGS=${LOSTENGINE_DEFS} ${INCDIR} -c
LDFLAGS=-lGLESv2 -lEGL -lm -lbcm_host -L/opt/vc/lib
EXECUTABLE=le2.out
SOURCES=$(CPPSOURCES) $(CSOURCES)
CPPOBJECTS=$(CPPSOURCES:.cpp=.o)
COBJECTS=$(CSOURCES:.c=.o)
OBJECTS=$(CPPOBJECTS) $(COBJECTS)

default: Makefile $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CPP) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CPP) $(CPPFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -vf $(OBJECTS)
	rm -vf $(EXECUTABLE)
