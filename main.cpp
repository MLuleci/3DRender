#include <iostream>
#include <limits>
#include <cmath>
#include <cctype>
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

// Values used in dragging
bool gDrag = false;
int gX, gY;

/** Get the projection of a vector u projected onto a plane with normal n.
 * @param Vector that's being projected
 * @param Normal vector that defines the plane
 * @return Projected vector, i.e. u minus the orthogonal component to the plane
*/
Vector3 getProjVector(Vector3 u, Vector3 n)
{
	return u - (n * (u.dot(n) / std::pow(n.mag(), 2)));
}

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
	gCamera.setRatio((w > h ? w / h : h / w));
}

void keyboard(unsigned char key, int x, int y)
{
	if (65 <= key && key <= 90) key = std::tolower(key);
	switch(key)
	{
		case '\x1b': // Escape
			glutLeaveMainLoop();
			break;
		case 'e': // Roll right
			gCamera.setRoll(-1);
			break;
		case 'q': // Roll left
			gCamera.setRoll(1);
			break;
		case 'p': // Toggle projections
			gCamera.toggleProj();
			break;
		case 'l': // Toggle lighting
			gSolid.toggleLight();
			break;
	}
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch(btn)
		{
			case GLUT_LEFT_BUTTON: // Start dragging
				gX = x;
				gY = y;
				gDrag = true;
				break;
			case 3: // Zoom in/out
			case 4:
				gCamera.setFov(gCamera.getFov() + (btn == 3 ? -1 : 1));
				break;
		}
	} else if (btn == GLUT_LEFT_BUTTON) {
		gDrag = false; // Stop dragging
	}
	glutPostRedisplay();
}

void move(int x, int y)
{
	gX = x - gX;
	gY = y - gY;
	// TODO: Rotate solid using gX & gY
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
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(move);

	// Initialize GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "Error initializing OpenGL: " << gluErrorString(err) << std::endl;
		return 1;
	}

	// Initialize OpenGL
	init();

	// Check args
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>\nFile must be in `.stl` format." << std::endl;
		return 1;
	}

	// Read STL file
	if (!gSolid.readFile(argv[1])) return 1;

	// Initialize camera
	gCamera.setRatio(SCREEN_WIDTH / SCREEN_HEIGHT);

	// Radius of sphere bounding solid
	double r = gSolid.getRadius();
	if (r == std::numeric_limits<double>::infinity()) r = std::numeric_limits<double>::max();

	// Set direction to face solid
	gCamera.setDir(gSolid.getCenter());

	// Set position one diameter away from solid
	gCamera.setPos(gSolid.getCenter() + gCamera.getDir() * (2 * r));

	// Distance to solid's center
	double d = (gCamera.getPos() - gSolid.getCenter()).mag();
	if (d == std::numeric_limits<double>::infinity()) d = std::numeric_limits<double>::max();

	// Set FoV that covers the entire solid
	double f = 2.0 * std::atan2(r, d);
	gCamera.setFov(180.0 * f / PI);

	// Set clipping that covers the entire solid
	gCamera.setClipping(d + r, d - r);

	// Print controls help
	std::cout 	<< "=============|CONTROLS|=============\n"
				<< "Left click & drag to rotate object\n"
				<< "E/Q keys to roll left/right\n"
				<< "Scroll to zoom in/out\n"
				<< "P to toggle perspective/orthographic\n"
				<< "L to toggle lighting\n"
				<< "ESC to quit\n"
				<< "====================================\n";

	// Enter GLUT main loop
	glutMainLoop();
	return 0;
}
