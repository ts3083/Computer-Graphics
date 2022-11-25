#include <gl/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* hw3 - 1번의 2번 */

void init(void);
void resize(int, int);
void draw(void);
void main_menu_function(int);

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat light_position[] = { -2.0, 1.0, 0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// 0번 조명 관련 설정
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	// material 설정
	GLfloat material_ambient[] = { 0.7f, 0.0f, 0.0f, 1.0f };
	GLfloat material_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat material_specular[] = { 0.7f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 16);

	GLfloat lmodel_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// 조명 스위치와 0번 조명 스위치 켜기
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	printf("init 함수 호출\n");
}

void draw(void) // 기존 함수 변경
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(2, 2, 0, 0, 0, 0, -1, 1, 0);
	
	/* 점의 크기 (초기값은 1.0)*/
	glColor3f(1.0, 0, 0);	// red
	glPointSize(30.0f);
	glBegin(GL_POINTS);
	glNormal3f(0, 1.0, 0);
	glVertex3i(0, 0, 0);
	glEnd();

	glFlush();
	glutSwapBuffers();
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

void main_menu_function(int option)
{
	if (option == 1) {
		printf("You selected Init menu\n");
		init();
	}
	/* 종료 */
	if (option == 999) {
		printf("You selected Quit\n");
		exit(0);
	}
	printf("Main menu %d has been selected\n", option);
}

int main(int argc, char** argv)
{
	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("hello world");
	init(); // -> 사용자 초기화 함수

	/* 메인 메뉴*/
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Init", 1);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* 콜백 함수 정의 */
	glutDisplayFunc(draw); // 실제 그리기 함수
	glutReshapeFunc(resize);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}
