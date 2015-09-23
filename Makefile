INCLUDE = -I/usr/include/ -I/opt/X11/include/
LIBDIR  = -L/opt/X11/lib/

all: monteCarloSpatialGrid

CC = g++

CFLAGS +=  $(shell pkg-config --cflags x11 xi xmu gl glu) $(INCLUDE)
LDFLAGS += $(shell pkg-config --libs x11 xi xmu gl glu)

# GLUT does not provide .pc file for pk-config
LDFLAGS += -lglut -lGL -lGLU $(LIBDIR)

LDFLAGS += -lm

# Debug
CFLAGS += -Wall -g

OBJ = glut_window.o main.o timer.o utils.o

monteCarloSpatialGrid: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o monteCarloSpatialGrid

glut_window.o: glut_window.cpp glut_window.hpp utils.hpp
	$(CC) $(CFLAGS) -c glut_window.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

utils.o: utils.cpp utils.hpp
	$(CC) $(CFLAGS) -c utils.cpp
  
clean:
	rm -fv $(OBJ) monteCarloSpatialGrid

