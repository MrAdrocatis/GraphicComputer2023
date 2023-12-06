#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h> 

#include "getBMP.h"

GLuint textureID;

// Globals.
static int isAnimate = 0; // Animated?
static float angle = 0.0; // Angle of rotation.
static int frameCount = 0; // Number of frames
static int light0On = 1; // White light on?
static int light1On = 1; // Green light on?
static float d = 1.0; // Diffuse and specular white light intensity.
static float m = 0.2; // Global ambient white light intensity.
static int localViewer = 1; // Local viewpoint?
static float p = 1.0; // Positional light?
static float t = 0.0; // Quadratic attenuation factor.
static float zMove = 0.0; // z-direction component.
static float xAngle = 290.0, yAngle = 0.0; // Rotation angles of white light.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static char theStringBuffer[10]; // String buffer.

static unsigned int texture[2]; // Array of texture ids.

// Fungsi untuk menggambar model APAR
void drawFireExtinguisher() {
	// Cylinder utama (badan APAR, warna merah)
	glColor3f(1.0, 0.0, 0.0);
	GLUquadricObj* cylinderMain = gluNewQuadric();
	gluQuadricDrawStyle(cylinderMain, GLU_FILL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]); // Mengikat texture yang telah dibuat sebelumnya.
	gluCylinder(cylinderMain, 1.0, 1.0, 10.0, 20, 20); // Radius bawah, radius atas, tinggi, slices, stacks

	// Cone di atas cylinder utama
	glColor3f(1.0, 0.0, 0.0);
	GLUquadricObj* coneTop = gluNewQuadric();
	gluQuadricDrawStyle(coneTop, GLU_FILL);
	glTranslatef(0.0, 0.0, 10.0);
	gluCylinder(coneTop, 1.0, 0.333, 0.5, 20, 20); // Radius bawah, radius atas (0 untuk kerucut), tinggi, slices, stacks

	// Cylinder kecil di atas cone (tutup cone)
	glColor3f(1.0, 0.0, 0.0);
	GLUquadricObj* cylinderTop = gluNewQuadric();
	gluQuadricDrawStyle(cylinderTop, GLU_FILL);
	glTranslatef(0.0, 0.0, 0.5);
	gluCylinder(cylinderTop, 0.333, 0.333, 0.333, 20, 20); // Radius bawah, radius atas, tinggi, slices, stacks

	// Balok pertama (badan selang)
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 0.5);
	glScalef(0.333, 0.5, 0.666); // Skala untuk membuat balok
	glutSolidCube(1.0);

	// Cylinder untuk selang
	glColor3f(1.0, 0.0, 0.0);
	GLUquadricObj* cylinderHose = gluNewQuadric();
	gluQuadricDrawStyle(cylinderHose, GLU_FILL);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0); // Rotasi untuk posisi selang
	glScalef(1.0, 1.0, 1.0);
	gluCylinder(cylinderHose, 0.3, 0.3, 15.0, 20, 20); // Radius bawah, radius atas, tinggi, slices, stacks

	// Cone di ujung selang (pegangan APAR)
	glColor3f(1.0, 0.0, 0.0);
	GLUquadricObj* coneHandle = gluNewQuadric();
	gluQuadricDrawStyle(coneHandle, GLU_FILL);
	glTranslatef(0.0, 0.0, 15.0);
	gluCylinder(coneHandle, 0.3, 0.5, 2.0, 20, 20); // Radius bawah, radius atas (0 untuk kerucut), tinggi, slices, stacks

	// Balok kedua (badan tuas)
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.0, 0.5, -16.0);
	glScalef(0.5, 0.5, 3.0); // Skala untuk membuat balok
	glutSolidCube(1.0);

	// Balok ketiga (badan tuas)
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.0, 1.0, 0.0);
	glRotatef(45.0, 1.0, 0.0, 0.0); // Rotasi untuk balok kedua
	glScalef(1, 0.5, 2); // Skala untuk membuat balok
	glutSolidCube(1.0);

	gluDeleteQuadric(cylinderMain);
	gluDeleteQuadric(coneTop);
	gluDeleteQuadric(cylinderTop);
	gluDeleteQuadric(cylinderHose);
	gluDeleteQuadric(coneHandle);
}

void loadTexture() {
	// Local storage for image data.
	imageFile* image[1];

	// Load the image.
	image[0] = getBMP("C:/Dokumen & Tugas Kampus Pens/Graphic Computer/Texture/Texture APAR.bmp");

	// Create texture object texture[0]. 
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Specify image data for currently active texture object.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->width, image[0]->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{
	char *c;

	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void increaseAngle(void)
{
	angle += 0.2;
	if (angle > 360.0) angle -= 360.0;
	glutPostRedisplay();
}

// Routine to convert floating point to char string.
void floatToString(char * destStr, int precision, float val)
{
	sprintf(destStr, "%f", val);
	destStr[precision] = '\0';
}

// Write data.
void writeData(void)
{
	glDisable(GL_LIGHTING); // Disable lighting.
	glColor3f(1.0, 1.0, 1.0);

	floatToString(theStringBuffer, 4, d);
	glRasterPos3f(-1.0, 1.05, -2.0);
	writeBitmapString((void*)font, "Diffuse and specular white light intensity: ");
	writeBitmapString((void*)font, theStringBuffer);

	floatToString(theStringBuffer, 4, m);
	glRasterPos3f(-1.0, 1.0, -2.0);
	writeBitmapString((void*)font, "Global ambient whitle light intensity: ");
	writeBitmapString((void*)font, theStringBuffer);

	floatToString(theStringBuffer, 4, t);
	glRasterPos3f(-1.0, 0.95, -2.0);
	writeBitmapString((void*)font, "Quadratic attenuation: ");
	writeBitmapString((void*)font, theStringBuffer);

	glRasterPos3f(-1.0, 0.9, -2.0);
	if (localViewer) writeBitmapString((void*)font, "Local viewpoint.");
	else writeBitmapString((void*)font, "Infinite viewpoint.");

	glEnable(GL_LIGHTING); // Re-enable lighting.
}

// Initialization routine.
void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST); // Enable depth testing.

	// Turn on OpenGL lighting.
	glEnable(GL_LIGHTING);

	// Material property vectors.
	float matAmbAndDif[] = { 1.0, 0.0, 0.0, 1.0 };
	float matSpec[] = { 1.0, 1.0, 1,0, 1.0 };
	float matShine[] = { 50.0 };

	// Material properties of APAR.
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

	// Cull back faces.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

// Drawing routine.
void drawScene()
{
	// Light property vectors.
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec0[] = { d, d, d, 1.0 };
	float lightPos0[] = { 0.0, 0.0, 3.0, p };
	float lightDifAndSpec1[] = { 0.0, 1.0, 0.0, 1.0 };
	float lightPos1[] = { 1.0, 2.0, 0.0, 1.0 };
	float globAmb[] = { m, m, m, 1.0 };

	// Light0 properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec0);

	// Light1 properties.
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec1);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer); // Enable local viewpoint

	// Turn lights off/on.
	if (light0On) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
	if (light1On) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// Light quadratic attenuation factor.
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, t);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, t);

	writeData();

	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Draw light source spheres (or arrow) after disabling lighting.
	glDisable(GL_LIGHTING);

	// Light0 positioned and sphere positioned in case of positional light
	// and arrow in case of directional light.
	glPushMatrix();
	glRotatef(40, 0.0, 1.0, 0.0); // Rotation about x-axis.
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glTranslatef(lightPos0[0], lightPos0[1], lightPos0[2]);
	glColor3f(d, d, d);
	if (light0On)
	{
		if (p) glutWireSphere(0.05, 8, 8); // Sphere at positional light source.
		else // Arrow pointing along incoming directional light.
		{
			glLineWidth(3.0);
			glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.25);
			glVertex3f(0.0, 0.0, -0.25);
			glVertex3f(0.0, 0.0, -0.25);
			glVertex3f(0.05, 0.0, -0.2);
			glVertex3f(0.0, 0.0, -0.25);
			glVertex3f(-0.05, 0.0, -0.2);
			glEnd();
			glLineWidth(1.0);
		}
	}
	glPopMatrix();

	// Light1 and its sphere positioned.
	glPushMatrix();
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glTranslatef(lightPos1[0], lightPos1[1], lightPos1[2]);
	glColor3f(0.0, 1.0, 1.0);
	if (light1On) glutWireSphere(0.05, 8, 8);
	glPopMatrix();

	glEnable(GL_LIGHTING);

	glTranslatef(0.0, -5.0, -10);
	glRotatef(xAngle, 1.0, 0.0, 0.0); // Contrl X asix
	glRotatef(yAngle, 0.0, 1.0, 0.0); // Control y axis
	glRotatef(angle, 0.0, 0.0, 1.0); // Animation
	drawFireExtinguisher(); // Summon APAR

	glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 20.0);
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
	case 'w':
		if (light0On) light0On = 0; else light0On = 1;
		glutPostRedisplay();
		break;
	case 'g':
		if (light1On) light1On = 0; else light1On = 1;
		glutPostRedisplay();
		break;
	case 'l':
		if (localViewer) localViewer = 0; else localViewer = 1;
		glutPostRedisplay();
		break;
	case 'L':
		if (localViewer) localViewer = 0; else localViewer = 1;
		glutPostRedisplay();
		break;
	case 'p':
		if (p) p = 0.0; else p = 1.0;
		glutPostRedisplay();
		break;
	case 'P':
		if (p) p = 0.0; else p = 1.0;
		glutPostRedisplay();
		break;
	case 'd':
		if (d > 0.0) d -= 0.05;
		glutPostRedisplay();
		break;
	case 'D':
		if (d < 1.0) d += 0.05;
		glutPostRedisplay();
		break;
	case 'm':
		if (m > 0.0) m -= 0.05;
		glutPostRedisplay();
		break;
	case 'M':
		if (m < 1.0) m += 0.05;
		glutPostRedisplay();
		break;
	case 't':
		if (t > 0.0) t -= 0.01;
		glutPostRedisplay();
		break;
	case 'T':
		t += 0.01;
		glutPostRedisplay();
		break;
	case 'r':
		zMove = 0.0; xAngle = 0.0, yAngle = 0.0;
		glutPostRedisplay();
		break;
	case ' ':
		if (isAnimate)
		{
			isAnimate = 0;
			glutIdleFunc(NULL);
		}
		else
		{
			isAnimate = 1;
			glutIdleFunc(increaseAngle);
		}
		break;
	default:
		break;
	}
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_DOWN)
	{
		xAngle++;
		if (xAngle > 360.0) xAngle -= 360.0;
	}
	if (key == GLUT_KEY_UP)
	{
		xAngle--;
		if (xAngle < 0.0) xAngle += 360.0;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		yAngle++;
		if (yAngle > 360.0) yAngle -= 360.0;
	}
	if (key == GLUT_KEY_LEFT)
	{
		yAngle--;
		if (yAngle < 0.0) yAngle += 360.0;
	}
	if (key == GLUT_KEY_PAGE_UP) zMove -= 0.1;
	if (key == GLUT_KEY_PAGE_DOWN) if (zMove < 3.0) zMove += 0.1;
	glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press 'w' to toggle the white light off/on." << std::endl
		<< "Press 'g' to toggle the green light off/on." << std::endl
		<< "Press 'd/D' to decrease/increase the white light's diffuse and specular intensity." << std::endl
		<< "Press 'm/M' to decrease/increase global ambient white light intensity." << std::endl
		<< "Press 't/T' to decrease/increase the quadratic attenuation parameter." << std::endl
		<< "Press 'l' to toggle between infinite and local viewpoint." << std::endl
		<< "Press 'p' to toggle between positional and directional white light." << std::endl
		<< "Press the page up/down keys to move the APAR." << std::endl
		<< "Press arrow keys to rotate the white light about the original position of the APAR." << std::endl
		<< "Press 'r' to reset the APAR and white light to their original positions." << std::endl;
	std::cout << "Press space to toggle between animation on and off." << std::endl;
}

// Main routine.
int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("APAR.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
