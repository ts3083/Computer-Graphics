#include <gl/glut.h>
#include <stdio.h>
#include <math.h>

/* Lab7 */

bool spin_state = TRUE;
double sun_rotAngle = 0;
double venus_rotAngle = 0;
double venus_revAngle = 0;
double earth_rotAngle = 0;
double earth_revAngle = 0;
double mars_rotAngle = 0;
double mars_revAngle = 0;
double moon_rotAngle = 0;
double moon_revAngle = 0;

void init()
{
	/* 화면의 기본색으로 dark blue 설정 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	printf("init 함수 호출\n");
}

void resize(int width, int height) {
	glViewport(0, 0, width, height); // viewprot transform
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)width / (float)height, 1, 500); // projection transform
	glMatrixMode(GL_MODELVIEW);

	printf("resize 함수 호출\n");
}

void idle() {
	if (spin_state == TRUE) {
		sun_rotAngle += 0.005;
		if (venus_rotAngle > 360)
			venus_rotAngle -= 360;

		/* 금성의 자전, 공전 각도 변화 */
		venus_rotAngle += 0.125;
		if (venus_rotAngle > 360)
			venus_rotAngle -= 360;
		venus_revAngle += 1;
		if (venus_revAngle > 360)
			venus_revAngle -= 360;

		/* 지구의 자전, 공전 각도 변화 */
		earth_rotAngle += 0.025;
		if (earth_rotAngle > 360)
			earth_rotAngle -= 360;
		earth_revAngle += 0.75;
		if (earth_revAngle > 360)
			earth_revAngle -= 360;

		/* 화성의 자전, 공전 각도 변화 */
		mars_rotAngle += 0.01;
		if (mars_rotAngle > 360)
			mars_rotAngle -= 360;
		mars_revAngle += 1;
		if (mars_revAngle > 360)
			mars_revAngle -= 360;

		/* 달의 자전, 공전 각도 변화 */
		moon_rotAngle += 0.025;
		if (moon_rotAngle > 360)
			moon_rotAngle -= 360;
		moon_revAngle += 0.125;
		if (moon_revAngle > 360)
			moon_revAngle -= 360;
	}

	glutPostRedisplay();
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

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	glColor3f(1, 0, 0); // red
	glRotatef(sun_rotAngle, 0, 1, 0); // draw_axis();
	glutWireSphere(2, 20, 20); // 태양
	draw_axis();

	glPushMatrix(); // 현재의 행렬을 스택에 저장
	//glPushMatrix();

	/*  첫 번째 sphere 그리기 */
	glRotatef(venus_rotAngle, 0, 1, 0); // draw_axis();
	glTranslatef(5, 0, 0); // draw_axis();
	glRotatef(venus_revAngle, 0, 1, 0); // draw_axis();

	glColor3f(1, 1, 1); // snow
	glutWireSphere(0.9, 10, 10); // 금성

	glPopMatrix();

	/* 두 번째 sphere 그리기 */
	glRotatef(earth_rotAngle, 0, 1, 0); // 지구의 공전 구현
	glTranslatef(8, 0, 0); // 중심으로부터 이동
	glRotatef(earth_revAngle, 0, 1, 0); // 지구의 자전 구현

	glColor3f(0, 1, 0.5); // green
	glutWireSphere(1, 10, 10); // 지구

	glRotatef(moon_rotAngle, 0, 1, 0); // 달의 공전 구현
	glTranslatef(2, 0, 0); // 중심으로부터 이동
	glRotatef(moon_revAngle, 0, 1, 0); // 달의 자전 구현

	glColor3f(0, 1, 1); // yellow
	glutWireSphere(0.3, 10, 10); // 달

	glPopMatrix(); // 태양만 그렸을 때의 상태로 행렬 복귀

	/* 세 번째 sphere 그리기 */
	glRotatef(mars_rotAngle, 0, 1, 0); // draw_axis();
	glTranslatef(10, 0, 0); // draw_axis();
	glRotatef(mars_revAngle, 0, 1, 0); // draw_axis();

	glColor3f(1, 0.6, 0); // orange
	glutWireSphere(0.5, 10, 10); // 화성

	glPopMatrix();

	glFlush();
	glutSwapBuffers();
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

	// Idle 콜백 함수
	glutIdleFunc(idle); // 컴퓨터의 유휴시간에 호출
	glutDisplayFunc(draw); // 실제 그리기 함수
	glutReshapeFunc(resize);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}
