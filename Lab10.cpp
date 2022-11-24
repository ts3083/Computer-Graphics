#include <gl/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* Lab10 */

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
unsigned char light;
int name = 0;

void init()
{
	/* 화면의 기본색으로 dark blue 설정 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// 0번 조명 관련 설정
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	// 1번 조명 관련 설정
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	// 2번 조명 관련 설정
	//GLfloat light_ambient1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	//GLfloat light_diffuse1[] = { 0.0f, 0.0f, 1.0f, 1.0f }; // blue
	//GLfloat light_specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient1);
	//glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse1);
	//glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular1);
	//glMaterialf(GL_FRONT, GL_SHININESS, 64);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	// 3번 조명 관련 설정
	//GLfloat light_ambient2[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	//GLfloat light_diffuse2[] = { 0.0f, 1.0f, 1.0f, 1.0f }; // cyan
	//GLfloat light_specular2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient2);
	//glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse2);
	//glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular2);
	//glMaterialf(GL_FRONT, GL_SHININESS, 64);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	// 4번 조명 관련 설정
	glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	// Depth-Test Enable
	glFrontFace(GL_CW); // Teapot은 CW로 정의됨
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST); // 없을 때와 있을 때 결과 비교

	// 조명 스위치와 0번 조명 스위치 켜기
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	printf("init 함수 호출\n");
}

void resize(int width, int height) {
	glViewport(0, 0, width, height); // viewprot transform
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500); // projection transform
	glMatrixMode(GL_MODELVIEW);

	printf("resize 함수 호출\n");
}

void draw_string(void* font, const char* str, int x, int y, int z) {
	glRasterPos3i(x, y, z);
	for (int i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
}

void idle() {
	if (spin_state == TRUE) {
		sun_rotAngle += 0.003;
		if (venus_rotAngle > 360)
			venus_rotAngle -= 360;

		/* 금성의 자전, 공전 각도 변화 */
		venus_rotAngle += 0.07;
		if (venus_rotAngle > 360)
			venus_rotAngle -= 360;
		venus_revAngle += 0.6;
		if (venus_revAngle > 360)
			venus_revAngle -= 360;

		/* 지구의 자전, 공전 각도 변화 */
		earth_rotAngle += 0.02;
		if (earth_rotAngle > 360)
			earth_rotAngle -= 360;
		earth_revAngle += 0.4;
		if (earth_revAngle > 360)
			earth_revAngle -= 360;

		/* 화성의 자전, 공전 각도 변화 */
		mars_rotAngle += 0.008;
		if (mars_rotAngle > 360)
			mars_rotAngle -= 360;
		mars_revAngle += 0.8;
		if (mars_revAngle > 360)
			mars_revAngle -= 360;

		/* 달의 자전, 공전 각도 변화 */
		moon_rotAngle += 0.02;
		if (moon_rotAngle > 360)
			moon_rotAngle -= 360;
		moon_revAngle += 0.07;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(15, 15, 15, 0, 0, 0, 0, 1, 0);
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };

	if (name == 1) {
		glColor3f(1, 1, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12181735 Kwon hyeokkyu", 0, 5, 0);
	}
	if (light == '0') {
		light_position[0] = 15;
		light_position[1] = 15;
		light_position[2] = 15; 
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	if (light == '4') {
		light_position[0] = -15;
		light_position[1] = -15;
		light_position[2] = 15;
		glLightfv(GL_LIGHT4, GL_POSITION, light_position);
	}
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	if (light == '1') {
		GLfloat emission0[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_EMISSION, emission0);

		glColor3f(1, 0, 0); // red
		glRotatef(sun_rotAngle, 0, 1, 0); // draw_axis();
		glutSolidSphere(2, 30, 30); // 태양
		draw_axis();

		emission0[0] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission0);
	}
	else {
		GLfloat emission0[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_EMISSION, emission0);

		glColor3f(1, 0, 0); // red
		glRotatef(sun_rotAngle, 0, 1, 0); // draw_axis();
		glutSolidSphere(2, 30, 30); // 태양
		draw_axis();

		emission0[0] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission0);
	}

	glPushMatrix(); // 현재의 행렬을 스택에 저장
	glPushMatrix();

	/*  첫 번째 sphere 그리기 */
	glRotatef(venus_rotAngle, 0, 1, 0); // draw_axis();
	glTranslatef(5, 0, 0); // draw_axis();
	glRotatef(venus_revAngle, 0, 1, 0); // draw_axis();

	glColor3f(1, 1, 1); // snow
	glutSolidSphere(0.9, 10, 10); // 금성

	glPopMatrix();

	/* 두 번째 sphere 그리기 */
	glRotatef(earth_rotAngle, 0, 1, 0); // 지구의 공전 구현
	glTranslatef(8, 0, 0); // 중심으로부터 이동
	glRotatef(earth_revAngle, 0, 1, 0); // 지구의 자전 구현

	glLightfv(GL_LIGHT2, GL_POSITION, light_position);
	if (light == '2') {
		GLfloat emission1[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_EMISSION, emission1);

		glColor3f(0, 0, 1); // blue
		glutSolidSphere(1, 10, 10); // 지구

		emission1[0] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
	}
	else {
		GLfloat emission1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_EMISSION, emission1);

		glColor3f(0, 0, 1); // blue
		glutSolidSphere(1, 10, 10); // 지구

		emission1[0] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
	}

	glRotatef(moon_rotAngle, 0, 1, 0); // 달의 공전 구현
	glTranslatef(2, 0, 0); // 중심으로부터 이동
	glRotatef(moon_revAngle, 0, 1, 0); // 달의 자전 구현

	glLightfv(GL_LIGHT3, GL_POSITION, light_position);
	if (light == '3') {
		GLfloat emission2[] = { 0.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_EMISSION, emission2);

		glColor3f(0, 1, 1); // yellow
		glutSolidSphere(0.3, 10, 10); // 달

		emission2[0] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission2);
	}
	else {
		GLfloat emission2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_EMISSION, emission2);

		glColor3f(0, 1, 1); // yellow
		glutSolidSphere(0.3, 10, 10); // 달

		emission2[0] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission2);
	}

	glPopMatrix(); // 태양만 그렸을 때의 상태로 행렬 복귀

	/* 세 번째 sphere 그리기 */
	glRotatef(mars_rotAngle, 0, 1, 0); // draw_axis();
	glTranslatef(10, 0, 0); // draw_axis();
	glRotatef(mars_revAngle, 0, 1, 0); // draw_axis();

	glColor3f(1, 0.6, 0); // orange
	glutSolidSphere(0.5, 10, 10); // 화성

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

void keyboard(unsigned char key, int x, int y)
{
	if (key == '0') { // 조명은 카메라의 위치
		printf("Camera light ON\n");
		light = '0';

		glEnable(GL_LIGHT0);

		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
	}
	else if (key == '1') { // 태양의 위치
		printf("Sun light ON\n");
		light = '1';

		// 조명 스위치와 1번 조명 스위치 켜기
		glEnable(GL_LIGHT1);

		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT4);
	}
	else if (key == '2') { // 지구의 위치
		printf("Earth light ON\n");
		light = '2';

		// 조명 스위치와 2번 조명 스위치 켜기
		glEnable(GL_LIGHT2);

		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT4);
	}
	else if (key == '3') { // 달의 위치
		printf("Moon light ON\n");
		light = '3';

		// 조명 스위치와 3번 조명 스위치 켜기
		glEnable(GL_LIGHT3);

		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT4);
	}
	else if (key == '4') { // world 좌표계의 어느 위치
		printf("(-15, -15, 15, 1) light ON\n");
		light = '4';

		glEnable(GL_LIGHT4);

		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
	}
	else if (key == '5') {
		if (name == 0) {
			name = 1;
		}
		else {
			name = 0;
		}
	}
	draw();
}

int main(int argc, char** argv)
{
	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Hello world");
	init(); // -> 사용자 초기화 함수

	/* 메인 메뉴*/
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Init", 1);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* 콜백 함수 정의 */
	glutIdleFunc(idle); // 컴퓨터의 유휴시간에 호출
	glutDisplayFunc(draw); // 실제 그리기 함수
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}
