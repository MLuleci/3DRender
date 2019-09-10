LIBS = -lm -lGLEW -lGLU -lGL -lglut
FLAGS = -Wall -Wextra -Wconversion -pedantic -g
OFILE = render.out
_OBJ = main.o vector3.o triangle.o solid.o camera.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
ODIR = obj
CC = g++

$(OFILE) : $(OBJ)
	$(CC) $(FLAGS) -o $@ $^ $(LIBS)

$(ODIR)/%.o : %.cpp
	$(CC) $(FLAGS) -c -o $@ $< $(LIBS)

clean :
	rm $(OFILE) $(OBJ)