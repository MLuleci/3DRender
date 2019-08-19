#include <cstdio>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "solid.hpp"
#include "camera.hpp"
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576
#define SCREEN_FPS 60

// Create global camera & solid
Camera gCamera(SCREEN_WIDTH, SCREEN_HEIGHT);
Solid gSolid;

void render()
{
	gCamera.render(gSolid);
}

void loop(int v)
{
	// TODO: Get user input
	render();
	glutTimerFunc(1000 / SCREEN_FPS, loop, v);
}

int main(int argc, char **argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\nFile must be in `.stl` format.\n", argv[0]);
		return 1;
	}

	// Read given STL file
	if (!gSolid.readFile(argv[1])) return 1;

	// Initialize GLUT cont.
	glutInitContextVersion(	2, 1); // Create OpenGL 2.1 context
	glutInitDisplayMode(GLUT_DOUBLE); // Create double buffered window
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("3DRender");

    // Initialize OpenGL
	glMatrixMode(GL_PROJECTION); // Projection matrix = identity
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); // Model matrix = identity
    glLoadIdentity();
	glClearColor(0.f, 0.f, 0.f, 1.f);

	GLenum err = glGetError();
    if(err != GL_NO_ERROR) {
        fprintf(stderr, "Error initializing OpenGL: %s\n", gluErrorString(err));
        return 1;
    }

	// Set callbacks & run
	glutDisplayFunc(render);
	glutTimerFunc(1000 / SCREEN_FPS, loop, 0);
    glutMainLoop();
}
