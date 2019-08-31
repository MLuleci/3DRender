#include <cstdio>
#include <iostream>
#include <limits>
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "solid.hpp"
#include "camera.hpp"

#if defined(_WIN32)
	#define clear() system("cls")
#else
	#define clear() system("clear")
#endif
#define pause() std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n')
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576

#define PI 3.1415926535

// Create global camera & solid
Camera gCamera;
Solid gSolid;

void display()
{
	// Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render solid
	gCamera.render(gSolid);

	// Update screen buffer
	glutSwapBuffers();

	// Re-draw TUI
	clear();
	printf(
		"Click & drag to pan camera\n" \
		"Arrow keys to move camera\n" \
		"Roll [L]eft or [R]ight\n" \
		"Zoom in or out [+/-]\n" \
		"[T]oggle lighting\n" \
		"Toggle perspective or orthographic [P]rojection\n" \
		"[I]ncrease or [D]ecrease field of view\n"
	);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); // Viewport transformation
	gCamera.setupProj(); // Projection transformation
}

void init()
{
	// Enable array(s)
	glEnableClientState(GL_VERTEX_ARRAY);

	// Enable attribute(s)
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	// glClearDepth is set to default, i.e. farthest from the camera
	glShadeModel(GL_FLAT);
}

int main(int argc, char **argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow(argv[0]);

	// Set callback(s)
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	/* TODO
	glutKeyboardFunc();
	glutMouseFunc();
	glutMotionFunc();
	*/

	// Initialize GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error initializing OpenGL: %s\n", gluErrorString(err));
		return 1;
	}

	// Initialize OpenGL
	init();

	// Check args
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <filename>\nFile must be in `.stl` format.\n", argv[0]);
		return 1;
	}

	// Read STL file
	if (!gSolid.readFile(argv[1])) return 1;

	// Initialize camera
	gCamera.setRatio(SCREEN_WIDTH / SCREEN_HEIGHT);
	double r = gSolid.getRadius(); // Radius of sphere bounding solid
	double d = (gCamera.getPos() - gSolid.getCenter()).mag(); // Distance to solid's center
	double f = 2.0 * atan2(r, d);
	gCamera.setFov(180.0 * f / PI);
	gCamera.setClipping(d + r, d - r);

	std::cout << "Press ENTER to continue..." << std::endl;
	pause();

	// Enter GLUT main loop
	glutMainLoop();
	return 0;
}
