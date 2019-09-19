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
Vector3 gCoords;
GLdouble projection_matrix[16];
const GLdouble modelview_matrix[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
int viewport_matrix[4] = {0, 0, (int) SCREEN_WIDTH, (int) SCREEN_HEIGHT};

/** Return the coordinates where a ray cast from the camera that passes through
 * a given pair screen coordinates intersects the sphere surrounding the solid.
 * @param x
 * @param y
 * @return The coordinates, origin if ray doesn't intersect sphere
*/
Vector3 sphereCoords(int x, int y)
{
	// Get updated matrices
	glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);

	// Get mouse position in world
	Vector3 p;
	Vector3 e = gCamera.getPos();
	gluUnProject(
		x, y, e.z,
		modelview_matrix, projection_matrix, viewport_matrix,
		&p.x, &p.y, &p.z
	);

	// Find where mouse intersects solid's sphere
	double r = std::pow(gSolid.getRadius(), 2);
	Vector3 m = p - e;

	double a = m.dot(m);
	double b = e.dot(m) * 2.f;
	double c = e.dot(e) - std::pow(r, 2);
	double root = std::pow(b, 2) - (4.f * a * c);
	if (root <= 0) return Vector3(); // Ray doesn't intersect w/ sphere
	double t = (-b - std::sqrt(root)) / (2.f * a);

	return (p + (m * t)).norm();
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
	viewport_matrix[2] = w;
	viewport_matrix[3] = h;
	gCamera.setRatio((w > h ? (double)w / (double)h : (double)h / (double)w));
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
				gCoords = sphereCoords(viewport_matrix[2] - x, y);
				break;
			case 3: // Zoom in/out
			case 4:
				gCamera.setFov(gCamera.getFov() + (btn == 3 ? -1 : 1));
				break;
		}
	}
	glutPostRedisplay();
}

void move(int x, int y)
{
	// Get new position
	Vector3 v = sphereCoords(viewport_matrix[2] - x, y);

	// Apply rotation only if sphere intersects & and coordinates have changed
	if (v != gCoords && v != Vector3()) {
		double a = gCoords.angle(v);
		double w = std::cos(a / 2.f);
		Vector3 n = gCoords.cross(v).norm() * std::sin(a / 2.f);
		gCamera.rotateSolid(n, w);

		// Update position
		gCoords = v;
		glutPostRedisplay();
	}
}

void init()
{
	// Enable attribute(s)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	// glClearDepth is set to default, i.e. farthest from the camera
	glShadeModel(GL_FLAT); // Use flat shading since solid(s) have no color
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

	// Enter GLUT main loop
	glutMainLoop();
	return 0;
}
