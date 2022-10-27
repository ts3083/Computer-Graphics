#include <gl/glut.h>
#include <stdio.h>
#include <math.h>

/* Lab8 */

double m_RotateAngle = 0;

void init()
{
	/* 화면의 기본색으로 dark blue 설정 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f,1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);	
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMateriali(GL_FRONT, GL_SHININESS, 20);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT, GL_CULL_FACE);
	//glPolygonMode(GL_FRONT, GL_LINE);

	// Depth-Test Enable
	glFrontFace(GL_CW); // Teapot은 CW로 정의됨
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST); // 없을 때와 있을 때 결과 비교

	glPolygonMode(GL_BACK, GL_LINE);
	//glPolygonMode(GL_BACK, GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);

	printf("init 함수 호출\n");
}

// 화살표 방향키 입력에 따른 도형 이동
void special_keyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT: //왼쪽 키
		m_RotateAngle -= 5;
		if (m_RotateAngle == 0)
			m_RotateAngle = 360;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT: //오른쪽 키
		m_RotateAngle += 5;
		if (m_RotateAngle == 360)
			m_RotateAngle = 0;
		glutPostRedisplay();
		break;
	}
}

void resize(int width, int height) {
	glViewport(0, 0, width, height); // viewprot transform
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500); // projection transform
	glMatrixMode(GL_MODELVIEW);

	printf("resize 함수 호출\n");
}

//void idle() {
//	if (m_RotateAngle > 360)
//		m_RotateAngle -= 360;
//
//	glutPostRedisplay();
//}

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

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
	glColor3f(1, 0, 0); // red

	glPushMatrix();
	glRotatef(m_RotateAngle, 0, 1, 0);
	//glRotatef(m_RotateAngle, 1, 0, 0);
	GLdouble eq[4] = { 1.0, 0.0, 0.0, 0.0 };
	//GLdouble eq[4] = { 0.0, 1.0, 0.0, 0.0 };
	glClipPlane(GL_CLIP_PLANE0, eq);
	glEnable(GL_CLIP_PLANE0);
	glPopMatrix();
	
	glutSolidTeapot(3);
	
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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
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
	//glutIdleFunc(idle); // 컴퓨터의 유휴시간에 호출
	glutDisplayFunc(draw); // 실제 그리기 함수
	glutSpecialFunc(special_keyboard);
	glutReshapeFunc(resize);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}
