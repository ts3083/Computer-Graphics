#include "bmpfuncs.h"
#include <GL/freeglut.h>
#include <gl/glut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PI 3.14159265

volatile double theta = 45.0;
volatile double phi = 45.0;
volatile int radius = 6;
volatile double upVector = cos(phi * PI / 180);

volatile double* angle = new double[7]();
volatile double rotateFactor = 1.0;
bool spinEnable = true;
bool display_axis = false;
bool set_materialProperty = true;

/* 광원 설정 모드 */
volatile unsigned char lightMode = 0;

/* 시야 설정 */
volatile double eyex, eyey, eyez;

/* texture mapping set variable */
GLuint texName[6];	// make cube
GLuint texName2[3];	// make cylinder
GLuint* texName3 = new GLuint();
GLuint tmp;

/* quadric object 생성 */
GLUquadricObj* qobj = gluNewQuadric();

/* object selector */
volatile unsigned char selector = 4;

void init(void);
void resize(int, int);

void draw(void);
void draw_axis(void);
void draw_cylinder(void);
void draw_sphere(void);
void draw_string(void*, const char*, float, float, float, float, float);
void draw_text(void);
void draw_teapot(void);

void mouse(int, int, int, int);
void mouseWheel(int but, int dir, int x, int y);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);

void popup_menu(void);
void sub_menu_function(int option);
void main_menu_function(int option);

void printInstruction(void);
void setDefault(void);

typedef unsigned char uchar;

int main(int argc, char** argv) {
	// initialize window
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	// GLUT_DOUBLE, GLUT_RGBA로 권장

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("Hello World");
	init();

	// CPU 유후 시간 처리
	//glutIdleFunc(idle);

	// Define Draw Callback
	glutDisplayFunc(draw);

	// 키보드 인터럽트 처리
	glutKeyboardFunc(keyboard);

	// 특수 키보드 방향키 처리
	glutSpecialFunc(specialKeyboard);

	//마우스 처리 glut
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouseWheel);

	// Popup Menu
	popup_menu();

	// Main Loop Start!
	glutMainLoop();

	return 0;
}

void draw(void) {
	/* 화면을 깨끗하게 지우기 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	eyex = radius * sin(theta * PI / 180) * cos(phi * PI / 180);
	eyey = radius * sin(phi * PI / 180);
	eyez = radius * cos(theta * PI / 180) * cos(phi * PI / 180);
	upVector = cos(phi * PI / 180);

	//// 차례대로 x에서 보는 위치, y에서 보는 위치, z에서 보는 위치
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, upVector, 0);

	/* 텍스처 */
	draw_text(); // 4
	
	/* 구 */
	draw_sphere(); // 1

	/* 실린더 */
	draw_cylinder(); // 3

	/* 티팟 */
	draw_teapot(); // 2

	/* 실제로 출력하는 함수 */
	glutSwapBuffers();
}

void draw_axis(void) {
	if (!display_axis) goto out1;
	glLineWidth(1.5); // 좌표 축의 두께 설정

	glBegin(GL_LINES);
	glColor3f(0.5, 0, 0); // x axis color : red
	glVertex3f(0, 0, 0);
	glVertex3f(3, 0, 0);

	glColor3f(0, 0.5, 0); // y axis color : green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 3, 0);

	glColor3f(0, 0, 0.5); // z axis color : blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 3);
	glEnd();

	glLineWidth(1); // 좌표 축의 두께 : 1(default)
out1:
	glutPostRedisplay();

}


void draw_teapot(void) {
	glViewport(WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	glColor3f(1.0, 0.5, 1.0);

	glutWireTeapot(1.0);

	draw_axis();

	glFlush();
}


void draw_cylinder(void) {
	glViewport(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	glColor3f(1.0, 1.0, 1.0);

	glTranslatef(0, 0, 1.0);
	gluDisk(qobj, 0, 0.5, 10, 10);

	glTranslatef(0, 0, -2.0);
	gluCylinder(qobj, 0.5, 0.5, 2.0, 10, 10);

	gluDisk(qobj, 0, 0.5, 10, 10);

	draw_axis();

	glFlush();
}

void draw_sphere() {
	glViewport(0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	glColor3f(1.0, 1.0, 1.0);

	gluSphere(qobj, 0.7, 100, 100);

	draw_axis();

	glFlush();
}

void init(void) {
	/* 화면의 기본색으로 Black 설정 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/* resize 콜백함수 지정 */
	glutReshapeFunc(resize);

	/* 조작법 출력 */
	printInstruction();
}

void mouse(int button, int state, int x, int y) {
}

void mouseWheel(int but, int dir, int x, int y) {
	if (dir > 0) {
		if (radius > 2) { radius--; printf("\'zoom in!\', raduis : %d\n", radius); }
	}
	else {
		if (radius < 100) { radius++; printf("\'zoom out!\', raduis : %d\n", radius); }
	}
	glutPostRedisplay();
}

void popup_menu(void) {
	int submenu1 = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("cube", 1);
	glutAddMenuEntry("cylinder", 2);
	glutAddMenuEntry("sphere", 3);
	glutAddMenuEntry("all", 4);

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Clear", 900);

	glutAddSubMenu("view mode", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void sub_menu_function(int option) {
	switch (option) {
	case 1:
		lightMode = 0;
		selector = 1;
		break;
	case 2:
		lightMode = 0;
		selector = 2;
		break;
	case 3:
		lightMode = 0;
		selector = 3;
		break;
	case 5:
		lightMode = 0;
		selector = 4;
		break;
	default:
		break;
	}
	printf("object viewmode%d\n", selector);
	glutPostRedisplay();
}

void main_menu_function(int option) {
	if (option == 999)
		exit(0);
	else if (option == 900)
		setDefault();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case '0':
		lightMode = 0;
		printf("light%d at camera\n", lightMode);
		break;
	case '1':
		lightMode = 1;
		printf("light%d at cube\n", lightMode);
		break;
	case '2':
		lightMode = 2;
		printf("light%d at cylinder\n", lightMode);
		break;
	case '3':
		lightMode = 3;
		printf("light%d at sphere\n", lightMode);
		break;
	case 's': spinEnable = !spinEnable; if (spinEnable) printf("Spin enabled!\n"); else printf("spin disabled!\n"); break;
	case 'd': display_axis = !display_axis; if (display_axis) printf("Display axis!\n"); else printf("Remove axis!\n"); break;
	case 'm':
		set_materialProperty = !set_materialProperty;
		if (set_materialProperty)
			printf("Enable material property!\n");
		else
			printf("Disable material property!\n");
		break;
	case '+':
		if (rotateFactor < 3.0) {
			rotateFactor += 0.1;
			printf("rotate velocity++: %lf\n", rotateFactor);
		}
		break;
	case '-':
		if (rotateFactor > 0.2) {
			rotateFactor -= 0.1;
			printf("rotate velocity--: %lf\n", rotateFactor);
		}
		break;
	default: printf("you pressed %d\n", key); break;
	}
	glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		theta -= 1;
		if (theta <= -360) theta = 0.0;
		break;
	case GLUT_KEY_RIGHT:
		theta += 1;
		if (theta >= 360) theta = 0.0;
		break;
	case GLUT_KEY_DOWN:
		phi -= 1;
		if (abs((int)phi) % 360 == 0) phi = 0.0;
		break;
	case GLUT_KEY_UP:
		phi += 1;
		if (abs((int)phi) % 360 == 0) phi = 0.0;
		break;
	case 5:
		printInstruction();
		setDefault();
		break;
	default:
		printf("%d is pressed\n", key);
		break;
	}
	glutPostRedisplay();
}

void printInstruction(void) {
	// 조작접 console 출력
	printf("\n------------ keyboard navigation ------------\n\'x\': zoom in\n");
	printf("\'z\': zoom out\n");
	printf("방향키: camera 위치\n");
	//printf("\'s\': spin Enable/Disable\n");
	printf("\'d\': print/remove draw_axis\n");
	printf("\'+\': rotate velocity up\n");
	printf("\'-\': rotate velocity down\n");
	printf("0~4: 광원 변경(object 3개 모두 있을 때만)\n");
	printf("F5: 새로고침\n\n");
}

void setDefault(void) {
	radius = 18;
	selector = 4;
	lightMode = 0;
	theta = 45.0;
	phi = 45.0;
	rotateFactor = 1.0;
	spinEnable = true;
	display_axis = false;
	set_materialProperty = true;
}

void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(red, green, blue);
	glRasterPos3f(x_position, y_position, 0);
	for (unsigned int i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void draw_text(void) {
	glViewport(0, WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	draw_string(GLUT_BITMAP_TIMES_ROMAN_24,	"Graphics are cool!!", 0, 0, 1.0f, 1.0f, 0);
	
	glFlush();
}

void resize(int width, int height) {
	/* Viewport는 함수 이름과 역할이 동일한데, view를 시작할 점(여기선 원점)과 시작점으로부터 너비와 높이를 정해준다 */
	glViewport(0, 0, width, height);
	printf("WIDTH: %d HEIGHT: %d\n", width, height);
	glMatrixMode(GL_PROJECTION); // 3D 투영을 위한 프로젝션 선언
	glLoadIdentity();

	/*
	Perspective 설정 gluPerspective(fovy, aspect, zNear, zFar)
	첫 번째 인자 fovy: 수직 방향의 field of view --> 단위 각(degree), 자료형 double
	두 번째 인자 aspect: clipping window의 가로:세로 비율 --> 가로/세로, 자료형 double
	세 번째 인자 zNear: Near plane까지의 거리 ( > 0 ) --> 자료형 double
	네 번째 인자 zFar: Far plane까지의 거리 ( > 0 ) --> 자료형 double
	*/
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}
