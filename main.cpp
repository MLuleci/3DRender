#include <cstdio>
#include "solid.hpp"

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\nFile must be in `.stl` format.\n", argv[0]);
		return 1;
	}

	Solid s;
	s.readFile(argv[1]);
}
