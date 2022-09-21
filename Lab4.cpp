#include <gl/glut.h>
#include <stdio.h>
#define NUMBER_OF_POINT 100 // 그릴 수 있는 점의 개수

GLfloat point_vertices[NUMBER_OF_POINT][2];
GLfloat line_vertices[NUMBER_OF_POINT][2];
GLfloat tri_vertices[NUMBER_OF_POINT][2];
int point_count = 0;
int line_count = 0;
int tri_count = 0;
int sub_option = 0;

int new_line_count = 0;
int new_tri_count = 0;

bool antialiase_on = false;
bool alpha_blending = false;

void init()
{
	/* clear background color */
	glClearColor(0.f, 0.f, 0.f, 1.0f);

	/* 화면 좌표 정보 설정 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 500, 500, 0); /* 우리가 보는 방향 */

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void draw_string(void* font, const char* str, int x, int y)
{
	unsigned int i;
	glRasterPos2i(x, y);
	for (i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
}

void display() {
	int i;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 0);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Hello world", 10, 10);
	glFlush();
	/* 점의 색상을 붉은 색으로 */
	glColor3f(1.0f, 0.0f, 0.0f);

	/* 점의 크기 (초기값은 1.0)*/
	glPointSize(4.0f);

	glEnableClientState(GL_VERTEX_ARRAY);

	if (point_count > 0) {
		if (alpha_blending == false) {
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else {
			glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
		}

		glBegin(GL_POINTS);
		for (i = 0; i < point_count; i++)
		{
			glVertex2i(point_vertices[i][0], point_vertices[i][1]);
		}
		glEnd();
	}
	if (line_count > 0) {
		if (alpha_blending == false) {
			glColor3f(1.0f, 1.0f, 0.0f);
		}
		else {
			glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
		}

		glBegin(GL_LINES);
		new_line_count = (line_count / 2) * 2;
		for (i = 0; i < new_line_count; i++)
		{
			glVertex2i(line_vertices[i][0], line_vertices[i][1]);
		}
		glEnd();
	}
	if (tri_count > 0) {
		if (alpha_blending == false) {
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		else {
			glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
		}

		glBegin(GL_TRIANGLES);
		new_tri_count = (tri_count / 3) * 3;
		for (i = 0; i < new_tri_count; i++)
		{
			glVertex2i(tri_vertices[i][0], tri_vertices[i][1]);
		}
		glEnd();
	}
	glFlush();
}

void sub_menu_function(int option)
{
	/* 점 그리기 */
	if (option == 1) {
		sub_option = 1;
	}
	/* 선 그리기 */
	else if (option == 2) {
		sub_option = 2;
	}
	/* 삼각형 그리기 */
	else if (option == 3) {
		sub_option = 3;
	}

	printf("Submenu %d has been selected\n", option);

}

void MouseClick(int button, int state, int x, int y) {
	if (sub_option == 1) {
		if (button == 0 && state == 0) {
			point_vertices[point_count][0] = x;
			point_vertices[point_count][1] = y;

			point_count++;

			display();
		}
	}
	else if (sub_option == 2) {
		if (button == 0 && state == 0) {
			line_vertices[line_count][0] = x;
			line_vertices[line_count][1] = y;

			line_count++;

			display();
		}
	}
	else if (sub_option == 3) {
		if (button == 0 && state == 0) {
			tri_vertices[tri_count][0] = x;
			tri_vertices[tri_count][1] = y;

			tri_count++;

			display();
		}
	}
}

void main_menu_function(int option)
{
	/* clear */
	if (option == 1) {
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
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

	if (key == 'a') {
		if (antialiase_on == false) {
			glEnable(GL_BLEND);
			glEnable(GL_POINT_SMOOTH);
			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_POLYGON_SMOOTH);

			antialiase_on = true;
			printf("antialiase on\n");
		}
		else {
			glDisable(GL_BLEND);
			glDisable(GL_POINT_SMOOTH);
			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_POLYGON_SMOOTH);

			antialiase_on = false;
			printf("antialiase off\n");
		}
	}
	else if (key == 'b') {
		if (alpha_blending == false) {
			glEnable(GL_BLEND);
			
			alpha_blending = true;
			printf("Alpha-Blending on\n");
		}
		else {
			glDisable(GL_BLEND);

			alpha_blending = false;
			printf("Alpha-Blending off\n");
		}
	}
	display();
}

int main(int argc, char** argv)
{
	int submenu1;

	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("hello world");
	init(); // -> 사용자 초기화 함수

	/* Popup menu 생성 및 추가 */
	submenu1 = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Triangle", 3); /* 3개의 항목이 있는 메뉴 생성 */

	/* 메인 메뉴*/
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Clear", 1);
	glutAddMenuEntry("Quit", 999);
	glutAddSubMenu("mode", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* 콜백 함수 정의 */
	glutDisplayFunc(display);
	glutMouseFunc(MouseClick);
	glutKeyboardFunc(keyboard);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}
