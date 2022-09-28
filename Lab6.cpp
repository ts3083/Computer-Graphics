#include <gl/glut.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535897

/* Lab6 */

double r = 10;
double theta = 45, phi = 45;
double cam[3] = { 10, 10, 10 };
double center[3];
double up[3] = { 0, 1, 0 };

double getRadian(int n) {
	return n * (PI / 180);
}

void init()
{
	printf("init func called\n");
	
	/* 화면의 기본색으로 dark blue 설정 */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* 화면 좌표 정보 설정 */
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);*/

	// set blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// set antialiasing
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_LINE_SMOOTH);

	theta = 45;
	phi = 45;
	glutPostRedisplay();
}

// 화살표 방향키 입력에 따른 도형 이동
void special_keyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT: //왼쪽 키
		phi -= 5;
		if (phi == 0)
			phi = 360;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT: //오른쪽 키
		phi += 5;
		if (phi == 360)
			phi = 0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP: //위 키
		theta -= 5;
		if (theta == 0)
			theta = 360;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN: //아래 키
		theta += 5;
		if (theta == 360)
			theta = 0;
		glutPostRedisplay();
		break;
	}
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45, (float)width / (float)height, 1, 500);
	gluPerspective(60, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}

void draw_axis(void) {
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0); // X축은 red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);
	glColor3f(0, 1, 0); // Y축은 green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);
	glColor3f(0, 0, 1); // Z축은 blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);
	glEnd();
	glLineWidth(1);
}

void draw(void) // 기존 함수 변경
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	cam[0] = r * sin(getRadian(theta)) * sin(getRadian(phi));
	cam[1] = r * cos(getRadian(theta));
	cam[2] = r * sin(getRadian(theta)) * cos(getRadian(phi));

	up[1] = (theta >= 0 && theta <= 180) ? 1 : -1;

	gluLookAt(cam[0], cam[1], cam[2], 0, 0, 0, up[0], up[1], up[2]);
	glColor3f(1, 1, 0);
	glutWireTeapot(3);
	draw_axis();

	printf("theta : %f, phi : %f\n", theta, phi);

	glFlush();
	glutSwapBuffers(); // double buffering시
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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Hello World");
	init(); // -> 사용자 초기화 함수

	/* 메인 메뉴*/
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Init", 1);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* 콜백 함수 정의 */
	glutDisplayFunc(draw); // 실제 그리기 함수
	glutSpecialFunc(special_keyboard);
	glutReshapeFunc(resize);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}
