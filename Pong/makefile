COMPILER = clang++

SOURCE_LIBS = -Ilib/

MAC_OPT = -std=c++17 -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a

MAC_OUT = -o "bin/Pong"

CFILES = src/*.cpp

Pong:
	$(COMPILER) $(CFILES) $(SOURCE_LIBS) $(MAC_OUT) $(MAC_OPT)
	./bin/Pong
