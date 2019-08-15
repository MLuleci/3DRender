LIBS = -lm
FLAGS = -Wall -g
OFILE = render.out
_OBJ = main.o vector3.o triangle.o solid.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
ODIR = obj
CC = g++

$(OFILE) : $(OBJ)
	$(CC) $(FLAGS) -o $@ $^ $(LIBS)

$(ODIR)/%.o : %.cpp
	$(CC) $(FLAGS) -c -o $@ $< $(LIBS)

clean :
	rm $(OFILE) $(OBJ)