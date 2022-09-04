#include<windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#define M_PI 3.1415926
GLboolean moving = GL_FALSE;
#define MAX_PLANES 1000
#define v3f glVertex3f
#define v2f glVertex2f

// угол поворота камеры
float angle = 0.0;
// координаты вектора направления движения камеры
float lx = 0.0f, lz = -1.0f;
// XZ позиция камеры
float x = 0.0f, z = 5.0f;
//Ключи статуса камеры. Переменные инициализируются нулевыми значениями
//когда клавиши не нажаты
float deltaAngle = 0.0f;
float deltaMove = 0;
float ugol = 0.0f;
bool light = true;
bool light2 = true;
bool texturetoggle = true;
float lightposition = -0.5;
struct {
	float speed;
	GLfloat red, green, blue;
	float theta;
	float x, y, z, angle;
} planes[MAX_PLANES];

float texCoord[] = { 0,0, 1,0, 1,1, 0,1 }; //координаты наложения текстур
unsigned int texture;
unsigned int texture2;

void texturing() {

	struct { unsigned char r, g, b, a; } data[2][2];
	memset(data, 0, sizeof(data));
	data[0][0].r = 0x48;
	data[0][0].g = 0x4E;
	data[0][0].b = 0x50;

	data[1][0].r = 0x48;
	data[1][0].g = 0x4E;
	data[1][0].b = 0x50;

	data[1][1].r = 0x3B;
	data[1][1].g = 0x40;
	data[1][1].b = 0x42;

	data[0][1].r = 0x2B;
	data[0][1].g = 0x2F;
	data[0][1].b = 0x30;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void texturing2() {

	struct { unsigned char r, g, b, a; } data2[2][2];
	memset(data2, 0, sizeof(data2));
	data2[0][0].r = 0;
	data2[0][0].g = 179;
	data2[0][0].b = 38;

	data2[1][0].r = 0;
	data2[1][0].g = 179;
	data2[1][0].b = 38;

	data2[1][1].r = 0;
	data2[1][1].g = 179;
	data2[1][1].b = 38;

	data2[0][1].r = 0;
	data2[0][1].g = 200;
	data2[0][1].b = 20;

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void tick_per_plane(int i)
{
	float theta = planes[i].theta += planes[i].speed;
	planes[i].z = -9 + 100 * cos(theta);
	planes[i].x = 100 * sin(2 * theta);
	planes[i].y = sin(theta / 3.4) * sin(theta / 3.4) * 20 + 7;
	planes[i].angle = ((atan(2.0) + M_PI / 2) * sin(theta) - M_PI / 2) * 180 / M_PI;
	if (planes[i].speed < 0.0) {
		planes[i].angle += 180;
	}
}

void add_plane(void)
{
	int i;

	for (i = 0; i < MAX_PLANES; i++)
		if (planes[i].speed == 0) {

#define SET_COLOR(r,g,b) \
    planes[i].red=r; planes[i].green=g; planes[i].blue=b;

			switch (rand() % 6) {
			case 0:
				SET_COLOR(1.0, 0.0, 0.0);
				break;
			case 1:
				SET_COLOR(1.0, 1.0, 1.0);
				break;
			case 2:
				SET_COLOR(0.0, 1.0, 0.0);
				break;
			case 3:
				SET_COLOR(1.0, 0.0, 1.0);
				break;
			case 4:
				SET_COLOR(1.0, 1.0, 0.0);
				break;
			case 5:
				SET_COLOR(0.0, 0.4, 0);
				break;
			}
			planes[i].speed = 0.00019;
			if (rand() & 1)
				planes[i].speed *= -1;
			planes[i].theta = ((float)(rand() % 1000));
			tick_per_plane(i);
			if (!moving)
				glutPostRedisplay();
			return;
		}
}


void tick(void)
{
	int i;

	for (i = 0; i < MAX_PLANES; i++)
		if (planes[i].speed != 0.0)
			tick_per_plane(i);
}

void animate(void)
{
	tick();
	glutPostRedisplay();
}

void visible(int state)
{
	if (state == GLUT_VISIBLE) {
		if (moving)
			glutIdleFunc(animate);
	}
	else {
		if (moving)
			glutIdleFunc(NULL);
	}
}


void changeSize(int w, int h) {
	// предотвращение деления на ноль
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;
	// используем матрицу проекции
	glMatrixMode(GL_PROJECTION);
	// обнуляем матрицу
	glLoadIdentity();
	// установить параметры вьюпорта
	glViewport(0, 0, w, h);
	// установить корректную перспективу
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	// вернуться к матрице проекции
	glMatrixMode(GL_MODELVIEW);
}


void drawPlane(int i) {
	GLfloat red, green, blue;
	glPushMatrix();
	glTranslatef(planes[i].x, planes[i].y, planes[i].z);
	glRotatef(290.0, 1.0, 0.0, 0.0);
	glRotatef(planes[i].angle, 0.0, 0.0, 1.0);
	glScalef(1.0 / 3.0, 1.0 / 4.0, 1.0 / 4.0);
	glTranslatef(0.0, -4.0, -1.5);
	glBegin(GL_TRIANGLE_STRIP);
	v3f(-7.0, 0.0, 2.0);
	v3f(-1.0, 0.0, 3.0);
	GLfloat color[] = { planes[i].red,planes[i].green,planes[i].blue,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	v3f(-1.0, 7.0, 3.0);
	GLfloat color2[] = { 0.6 * planes[i].red, 0.6 * planes[i].green, 0.6 * planes[i].blue,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color2);
	v3f(0.0, 0.0, 0.0);
	v3f(0.0, 8.0, 0.0);
	v3f(1.0, 0.0, 3.0);
	v3f(1.0, 7.0, 3.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color2);
	v3f(7.0, 0.0, 2.0);
	glEnd();
	glPopMatrix();
}

void computePos(float deltaMove)
{
	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle)
{
	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

void renderScene(void) {

	glClearColor(0.5, 0.8, 0.9, 1);
	if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);

	glEnable(GL_LIGHTING);

	if (light == true) {
		glEnable(GL_LIGHT1);
	}
	else {
		glDisable(GL_LIGHT1);
	}

	if (light2 == true) {
		glEnable(GL_LIGHT0);
	}
	else {
		glDisable(GL_LIGHT0);
	}

	glEnable(GL_NORMALIZE);

	if (light2 == true) {
		glPushMatrix();
		glRotatef(ugol, 0, 0, 1);
		float position[] = { 0, 55, 0, 0 };
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glPopMatrix();
		ugol = ugol + 0.05f;
	}
	//очистить буфер цвета и глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// обнулить трансформацию
	glLoadIdentity();
	// установить камеру
	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);

	GLfloat positionmain[] = { x, 2, z, 1 };
	GLfloat colormain[] = { 1, 1, 1, 1 };
	GLfloat directionmain[] = { lx, lightposition, lz };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, colormain);
	glLightfv(GL_LIGHT1, GL_POSITION, positionmain);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, directionmain);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 70);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10);

	if (texturetoggle) {
		glEnable(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, texture2);
	for (int i = -100; i < 100; i++) {
		for (int j = -100; j < 100; j++) {
			glNormal3f(0, 1, 0);
			GLfloat color[] = { 0,0.69,0.15,1 };
			GLfloat color1[] = { 0, 0.3,0.06,1 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color1);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(i, 0.0f, j);
			glTexCoord2f(15, 0);
			glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(15, 15);
			glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(0, 15);
			glVertex3f(i + 1, 0.0f, j);
			glEnd();
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);


	glBindTexture(GL_TEXTURE_2D, texture);
	for (int i = -100; i < 100; i++) {
		for (int j = -8; j < 8; j++) {
			glNormal3f(0, 1, 0);
			GLfloat color[] = { 0.31, 0.31, 0.31,1 };
			GLfloat color1[] = { 0.18,0.18,0.18,1 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color1);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(i, 0.01f, j);
			glTexCoord2f(15, 0);
			glVertex3f(i, 0.01f, j + 1);
			glTexCoord2f(15, 15);
			glVertex3f(i + 1, 0.01f, j + 1);
			glTexCoord2f(0, 15);
			glVertex3f(i + 1, 0.01f, j);
			glEnd();
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, texture);
	for (int i = -8; i < 8; i++) {
		for (int j = -100; j < 100; j++) {
			glNormal3f(0, 1, 0);
			GLfloat color[] = { 0.31, 0.31, 0.31,1 };
			GLfloat color1[] = { 0.18,0.18,0.18,1 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color1);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(i, 0.01f, j);
			glTexCoord2f(15, 0);
			glVertex3f(i + 1, 0.01f, j);
			glTexCoord2f(15, 15);
			glVertex3f(i + 1, 0.01f, j + 1);
			glTexCoord2f(0, 15);
			glVertex3f(i, 0.01f, j + 1);
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = -100; i < 100; i++) {
		for (float j = -0.2; j < 0.2; j = j + 0.025) {
			glNormal3f(0, 1, 0);
			GLfloat color[] = { 1, 1, 1, 1 };
			GLfloat color1[] = { 0,0,0,1 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color1);
			glBegin(GL_QUADS);
			glVertex3f(i, 0.02f, j);
			glVertex3f(i, 0.02f, j + 0.025);
			glVertex3f(i + 1, 0.02f, j + 0.025);
			glVertex3f(i + 1, 0.02f, j);
			glEnd();
		}
	}


	for (int i = -100; i < 100; i++) {
		for (float j = -0.2; j < 0.2; j = j + 0.025) {
			glNormal3f(0, 1, 0);
			GLfloat color[] = { 1, 1, 1, 1 };
			GLfloat color1[] = { 0,0,0,1 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color1);
			glBegin(GL_QUADS);
			glVertex3f(i, 0.02f, j);
			glVertex3f(i, 0.02f, j + 0.025);
			glVertex3f(i + 1, 0.02f, j + 0.025);
			glVertex3f(i + 1, 0.02f, j);
			glEnd();
		}
	}


	for (float i = -0.2; i < 0.2; i = i + 0.025) {
		for (int j = -100; j < 100; j = j + 1) {
			glNormal3f(0, 1, 0);
			GLfloat color[] = { 1, 1, 1, 1 };
			GLfloat color1[] = { 0,0,0,1 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color1);
			glBegin(GL_QUADS);
			glVertex3f(i, 0.02f, j);
			glVertex3f(i + 0.025, 0.02f, j);
			glVertex3f(i + 0.025, 0.02f, j + 1);
			glVertex3f(i, 0.02f, j + 1);
			glEnd();
		}
	}


	for (int i = 0; i < MAX_PLANES; i++)
		if (planes[i].speed != 0.0) {
			glNormal3f(0, 0, 1);
			drawPlane(i);
		}

	glutSwapBuffers();
}



void keyboard(unsigned char ch, int x, int y)
{
	switch (ch) {
	case 27:
		exit(0);
		break;
	case 'm':
		moving = GL_TRUE;
		glutIdleFunc(animate);
		break;
	case 77:
		moving = GL_TRUE;
		glutIdleFunc(animate);
		break;
	case 'n':
		moving = GL_FALSE;
		glutIdleFunc(NULL);
		glutIdleFunc(renderScene);
		break;
	case 78:
		moving = GL_FALSE;
		glutIdleFunc(NULL);
		glutIdleFunc(renderScene);
		break;
	case 108:
		if (light == true) {
			light = false;
		}
		else {
			light = true;
		}
		glutPostRedisplay();
		break;
	case 76:
		if (light == true) {
			light = false;
		}
		else {
			light = true;
		}
		glutPostRedisplay();
		break;
	case 75:
		if (light2 == true) {
			light2 = false;
		}
		else {
			light2 = true;
		}
		glutPostRedisplay();
		break;
	case 107:
		if (light2 == true) {
			light2 = false;
		}
		else {
			light2 = true;
		}
		glutPostRedisplay();
		break;
	case 74:
		if (texturetoggle == true) {
			texturetoggle = false;
		}
		else {
			texturetoggle = true;
		}
		glutPostRedisplay();
		break;
	case 106:
		if (texturetoggle == true) {
			texturetoggle = false;
		}
		else {
			texturetoggle = true;
		}
		glutPostRedisplay();
		break;
	case 43:
		for (int i = 0; i < 100; i++) {
			if (abs(planes[i].speed) < 0.01) { planes[i].speed = planes[i].speed * 2; }
		}
		glutPostRedisplay();
		break;
	case 45:
		for (int i = 0; i < 100; i++) {
			if (abs(planes[i].speed) > 0.00002375) { planes[i].speed = planes[i].speed / 2; }
		}
		glutPostRedisplay();
		break;
	}
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_LEFT:
		deltaAngle = -0.01f;
		break;
	case GLUT_KEY_RIGHT:
		deltaAngle = 0.01f;
		break;
	case GLUT_KEY_UP:
		deltaMove = 0.5f;
		break;
	case GLUT_KEY_DOWN:
		deltaMove = -0.5f;
		break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		deltaAngle = 0.0f;
		break;
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		deltaMove = 0;
		break;
	}
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1200, 700);
	glutCreateWindow("Самолеты");

	for (int i = 0; i < 100; i++) {
		add_plane();
	}

	texturing();
	texturing2();
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutSpecialFunc(pressKey);

	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(releaseKey);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	return 1;
}
