#include <gl/glut.h>
#include <stdio.h>

void init()
{
	/* 화면의 기본색으로 dark blue 설정 */
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

	/* 화면 좌표 정보 설정 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

void draw_point(void) {
	/* 점의 색상을 붉은 색으로 */
	glColor3f(1.0f, 0.0f, 0.0f);

	/* 점의 크기 (초기값은 1.0)*/
	glPointSize(4.0f);

	glBegin(GL_POINTS);
	glVertex2i(30, 30);
	glVertex2i(200, 400);
	glVertex2i(400, 250);
	glEnd();
}

void draw_line(void)
{
	/* 선의 색상을 노란색으로 */
	glColor3f(1.0f, 1.0f, 0.0f);

	glBegin(GL_LINES);
	glVertex2i(50, 50);
	glVertex2i(250, 450);
	glVertex2i(300, 450);
	glVertex2i(400, 100);
	glEnd();
}

void draw_triangle(void)
{
	/* 면의 색상을 초록색으로 */
	glColor3f(0.3f, 0.5f, 0.0f);
	glBegin(GL_TRIANGLES);

	glVertex2i(50, 50);
	glVertex2i(250, 450);
	glVertex2i(400, 100);

	glEnd();
}

void draw_triangle_strip(void) {
	/* 면의 색상을 초록색으로 */
	glColor3f(0.3f, 0.5f, 0.0f);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex2i(50, 50);
	glVertex2i(50, 480);
	glVertex2i(200, 40);
	glVertex2i(90, 470);
	glVertex2i(300, 30);
	glVertex2i(250, 450);
	glEnd();
}

void draw_triangle_fan(void) {
	/* 면의 색상을 초록색으로 */
	glColor3f(0.3f, 0.5f, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 앞뒤면을 선으로 그리기

	glBegin(GL_TRIANGLE_FAN);
	glVertex2i(200, 200);
	glVertex2i(200, 450);
	glVertex2i(300, 400);
	glVertex2i(400, 350);
	glVertex2i(420, 180);
	glEnd();
}

void draw_quads(void) {
	/* 면의 색상을 회색으로 */
	glColor3f(0.2f, 0.3f, 0.3f);

	glBegin(GL_QUADS);
	glVertex2i(80, 50);
	glVertex2i(200, 40);
	glVertex2i(180, 200);
	glVertex2i(100, 150);

	glVertex2i(250, 30);
	glVertex2i(370, 40);
	glVertex2i(430, 400);
	glVertex2i(330, 300);
	glEnd();
}

void draw_quads_strip(void) {
	/* 면의 색상을 회색으로 */
	glColor3f(0.2f, 0.3f, 0.3f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // 앞면만 보여주기
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 앞뒤면을 선으로 그리기

	glBegin(GL_QUAD_STRIP);
	glVertex2i(50, 80);
	glVertex2i(40, 300);
	glVertex2i(80, 70);
	glVertex2i(80, 280);

	glVertex2i(150, 80);
	glVertex2i(130, 300);
	glVertex2i(200, 100);
	glVertex2i(230, 350);
	glEnd();
}

/* 폴리곤 그리기 */
void draw_polygon(void) {
	/* 면의 색상을 회색으로 */
	glColor3f(0.2f, 0.3f, 0.3f);

	glCullFace(GL_CW); // 왼손으로 폴리곤을 감쌀 때 엄지 손가락 방향이 앞면
	glPolygonMode(GL_BACK, GL_LINE); // 뒷면을 선으로 표시

	glBegin(GL_QUAD_STRIP); // 폴리곤 그리기
	/* 시계방향으로 vertex 정의*/
	glVertex2i(150, 300);
	glVertex2i(200, 200);
	glVertex2i(330, 250);
	glVertex2i(400, 350);
	glVertex2i(250, 400);
	glEnd();
}

/* 그리기 함수 */
void draw(void)
{
	/* 화면을 깨끗하게 지우기 */
	glClear(GL_COLOR_BUFFER_BIT);

	//draw_point();
	//draw_line();
	//draw_triangle();

	glFlush();
}

/* MENU 처리 함수 – 메뉴 선택시 수행됨 */
void sub_menu_function1(int option)
{
	glClear(GL_COLOR_BUFFER_BIT);
	/* 선 그리기 */
	if (option == 1) {
		glDisable(GL_LINE_STIPPLE);
		draw_line();
	}
	/* 점선 그리기 */
	else if (option == 2) {
		glLineWidth(2.0f);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(3, 0xAAAA);
		draw_line();
	}
	/* 루프 선 그리기 */
	else if (option == 3) {
		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
		glVertex2i(5, 5);
		glVertex2i(250, 450);
		glVertex2i(300, 250);
		glVertex2i(400, 100);
		glEnd();
	}
	glFlush();

	printf("Submenu %d has been selected\n", option);

}

void sub_menu_function2(int option)
{
	glClear(GL_COLOR_BUFFER_BIT);
	/* 삼각형 그리기 */
	if (option == 1) {
		draw_triangle();
	}
	/* 삼각형 조각 그리기 */
	else if (option == 2) {
		draw_triangle_strip();
	}
	/* 삼각형 팬 그리기 */
	else if (option == 3) {
		draw_triangle_fan();
	}
	/* 쿼드 그리기 */
	else if (option == 4) {
		draw_quads();
	}
	/* 쿼드 조각 그리기 */
	else if (option == 5) {
		draw_quads_strip();
	}
	/* 플리곤 그리기 */
	else if (option == 6) {
		draw_polygon();
	}

	glFlush();
	printf("Submenu %d has been selected\n", option);

}

void main_menu_function(int option)
{
	/* 점 그리기 */
	if (option == 1) {
		glClear(GL_COLOR_BUFFER_BIT);
		draw_point();
		glFlush();
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
	int submenu1, submenu2;

	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("hello world");
	init(); // -> 사용자 초기화 함수

	/* Popup menu 생성 및 추가 */
	/* Line 하위 메뉴*/
	submenu1 = glutCreateMenu(sub_menu_function1);
	glutAddMenuEntry("LIne", 1);
	glutAddMenuEntry("Stipple Line", 2);
	glutAddMenuEntry("Loop Line", 3);

	/* Polygon 하위 메뉴*/
	submenu2 = glutCreateMenu(sub_menu_function2);
	glutAddMenuEntry("Triangle", 1);
	glutAddMenuEntry("Triangle strip", 2);
	glutAddMenuEntry("Triangle fan", 3);
	glutAddMenuEntry("Quads", 4);
	glutAddMenuEntry("Quad strip", 5);
	glutAddMenuEntry("Polygon", 6);

	/* 메인 메뉴*/
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Point", 1);
	glutAddSubMenu("Line", submenu1);
	glutAddSubMenu("Polygon", submenu2);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* 콜백 함수 정의 */
	glutDisplayFunc(draw); // 실제 그리기 함수

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}
