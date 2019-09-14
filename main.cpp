#include <iostream>
#include <string>
#include <limits>
#include <cmath>
#include <cctype>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "solid.hpp"
#include "camera.hpp"
#define SCREEN_WIDTH 1024.0
#define SCREEN_HEIGHT 576.0
#define PI 3.1415926535
#define rad(x) (x * PI / 180.f)

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
	if (65 <= key && key <= 90) key = static_cast<unsigned char>(std::tolower(key));
	switch(key)
	{
		case '\x1b': // Escape
			glutLeaveMainLoop();
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
	// Get mouse delta
	double dx = rad(static_cast<double>(x - gX));
	double dy = -rad(static_cast<double>(y - gY));

	gCamera.rotateSolid(dx, dy);

	// Update mouse position
	gX = x;
	gY = y;

	glutPostRedisplay();
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
	// Check args & print help
	if ((argc > 1 && !std::string(argv[1]).compare("-h")) || argc < 2) {
		std::cout 	<< "Usage: " << argv[0] << " <filename>\n"
					<< "File must be in `.stl` format.\n\n"
					<< "Controls:\n"
					<< "Left click & drag to rotate object\n"
					<< "Scroll to zoom in/out\n"
					<< "P to toggle perspective/orthographic\n"
					<< "L to toggle lighting\n"
					<< "ESC to quit\n\n"
					<< "Use `-h` flag to see this help\n";
		return 0;
	}

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

	// Read STL file
	if (!gSolid.readFile(argv[1])) return 1;

	//gCamera.toggleProj();

	// Initialize camera
	gCamera.setRatio(SCREEN_WIDTH / SCREEN_HEIGHT);

	// Set FoV
	gCamera.setFov(45.f);

	// Radius of sphere bounding solid
	double r = gSolid.getRadius();
	if (r == std::numeric_limits<double>::infinity()) r = std::numeric_limits<double>::max();

	// Distance from camera to center of the solid @ given FoV
	double d = r / std::tan(rad(gCamera.getFov()) / 2.f);

	// Set position to view entire solid
	gCamera.setPos(gCamera.getDir() * d);

	// Set clipping that covers the entire solid
	double dia = 2 * r;
	gCamera.setClipping(d + dia, d - dia);

	// Setup orthographic projection values
	gCamera.setupOrtho(dia, gCamera.getFov());

	// Enter GLUT main loop
	glutMainLoop();
	return 0;
}
