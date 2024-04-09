COMPILER = clang++

SOURCE_LIBS = -Ilib/

MAC_OPT = -std=c++20 -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a

MAC_OUT = -o "bin/Space Invaders"

CFILES = src/*.cpp

Space Invaders:
	$(COMPILER) $(CFILES) $(SOURCE_LIBS) $(MAC_OUT) $(MAC_OPT)
	./bin/Space\ Invaders
