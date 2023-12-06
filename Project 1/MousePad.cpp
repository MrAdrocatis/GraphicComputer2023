/////////////////////////////////          
// Made by :
// 1. Yusuf 'Ammar Fathin Arrafif (5222600043)
// 2. Gustavino Mahaken (5222600040)
// Class
// GTB 10
/////////////////////////////////

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h> 

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.6f, 0.3f, 0.0f);
	glTranslatef(0.0, 0.0, -45.0);
	// Draw a polygon with specified vertices.
	glBegin(GL_POLYGON);
	glVertex3f(-80.0, 80.0, 0.0);
	glVertex3f(-80.0, -80.0, 0.0);
	glVertex3f(80.0, -80.0, 0.0);
	glVertex3f(80.0, 80.0, 0.0);
	glEnd();

	glPushMatrix();

	glColor3f(0.0f, 0.0f, 0.5f);
	glLoadIdentity();
	// Modeling transformations.
	glTranslatef(0.0, 0.0, -40.0);
	glScalef(10.0, 8.0, 1.0);
	glutSolidCube(5.1); // Box.

	glPopMatrix();
	glPushMatrix();


	glColor3f(0.1f, 0.1f, 0.1f);
	glLoadIdentity();
	// Modeling transformations.
	glTranslatef(27.2, 22.0, -40.0);
	glScalef(1.0, 8.0, 1.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.5, 5.5, 70.0, 70.0);
	glPopMatrix();
	glPushMatrix();


	glColor3f(0.1f, 0.1f, 0.1f);
	glLoadIdentity();
	// Modeling transformations.
	glTranslatef(-27.2, 22.0, -40.0);
	glScalef(1.0, 8.0, 1.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.5, 5.5, 70.0, 70.0);
	
	glPopMatrix();
	glPushMatrix();


	glColor3f(0.1f, 0.1f, 0.1f);
	glLoadIdentity();
	// Modeling transformations.
	glTranslatef(-27.5, 22.0, -40.0);
	glScalef(10.0, 1.0, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glutSolidCylinder(0.5, 5.5, 70.0, 70.0);

	glPopMatrix();
	glPushMatrix();


	glColor3f(0.1f, 0.1f, 0.1f);
	glLoadIdentity();
	// Modeling transformations.
	glTranslatef(-27.5, -22.0, -40.0);
	glScalef(10.0, 1.0, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glutSolidCylinder(0.5, 5.5, 70.0, 70.0);

	glPopMatrix();

	glFlush();
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("box.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}

