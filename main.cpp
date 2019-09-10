#include <cstdio>
#include <iostream>
#include <limits>
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "solid.hpp"
#include "camera.hpp"
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576

#define PI 3.1415926535

// Create global camera & solid
Camera gCamera;
Solid gSolid;

void display()
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render solid
	gCamera.render(gSolid);
	glFlush();

	// Update screen buffer
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); // Viewport transformation
	gCamera.setRatio(w / h);
}

void init()
{
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
	if (r == std::numeric_limits<double>::infinity()) r = std::numeric_limits<double>::max();
	gCamera.setDir(gSolid.getCenter());
	gCamera.setPos(gSolid.getCenter() + gCamera.getDir() * (2 * r));
	double d = (gCamera.getPos() - gSolid.getCenter()).mag(); // Distance to solid's center
	if (d == std::numeric_limits<double>::infinity()) d = std::numeric_limits<double>::max();
	double f = 2.0 * std::atan2(r, d);
	gCamera.setFov(180.0 * f / PI);
	gCamera.setClipping(d + r, d - r);

	// TODO: Print controls help

	// Enter GLUT main loop
	glutMainLoop();
	return 0;
}
