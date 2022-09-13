#include <gl/glut.h>
#include <stdio.h>

void init()
{
	/* 화면의 기본색으로 dark blue 설정 */
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
}

void draw(void)
{
	/* Teapot 1개를 그리는 임시 루틴 */
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f); /* 인자를 바꾸면 색이 변화 */
	glutWireTeapot(0.5f); /* 주전자를 하나 그림 */
	glFlush();
}

void mouse(int button, int state, int x, int y) {
	/* 인자들을 해석해서 원하는 기능을 구현 */
	printf("Mouse button is clicked! (%d, %d, %d, %d)\n",
		button, state, x, y);
}

void motion(int x, int y)
{
	/* 인자들을 해석해서 원하는 기능을 구현 */
	printf("Mouse is moving! (%d, %d)\n", x, y);
}

void keyboard(unsigned char key, int x, int y)
{
	/* 인자들을 해석해서 원하는 기능을 구현 */
	printf("You pressed %c\n", key);
}

/* MENU 처리 함수 – 메뉴 선택시 수행됨 */
void sub_menu_function(int option)
{
	printf("Submenu %d has been selected\n", option);
}
void main_menu_function(int option)
{
	printf("Main menu %d has been selected\n", option);
	if (option == 999) {
		exit(0);
	}
}

int main(int argc, char** argv)
{
	int submenu1;

	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Hello World");
	init(); // -> 사용자 초기화 함수

	/* Popup menu 생성 및 추가 */
	submenu1 = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("Sub1", 1);
	glutAddMenuEntry("Sub2", 2);
	glutAddMenuEntry("Sub3", 3); /* 3개의 항목이 있는 메뉴 생성 */

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Go!", 11);

	glutAddSubMenu("Sub Menu", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Callback 함수 정의 */
	glutDisplayFunc(draw); /* draw() -> 실제 그리기 함수 */
	glutMouseFunc(mouse); /* 마우스 입력처리 */
	glutMotionFunc(motion); /* 마우스 움직임 처리 */
	glutKeyboardFunc(keyboard); /* 키보드 입력처리 */

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}
