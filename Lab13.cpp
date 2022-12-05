#include <gl/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <windows.h>

/* Lab13 */
/* quadric object 생성 */
GLUquadricObj* qobj = gluNewQuadric();

float g_pPosition[6] = { 0.0f, 144.0f, -100.0f, -100.0f, 100.0f, -100.0f };
int g_nX, g_nY;
int g_nSelect = 0;
int g_nGLWidth = 500, g_nGLHeight = 500;

void init(void);
void resize(int, int);
void draw(void);
void DrawBackground();
void DrawSphere();
void Picking(int, int);
void mouse(int, int, int, int);
void motion(int, int);
void main_menu_function(int);

void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -100, 100);
}

void DrawBackground() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	int i;
	int offset = 50;
	glBegin(GL_LINES);
	for (i = -g_nGLWidth / 2.0f; i < g_nGLWidth / 2.0f; i += offset)
	{
		glVertex3f(i, -g_nGLHeight / 2.0f, 0.0f);
		glVertex3f(i, g_nGLHeight / 2.0f, 0.0f);
	}
	for (i = -g_nGLHeight / 2.0f; i < g_nGLHeight / 2.0f; i += offset)
	{
		glVertex3f(-g_nGLWidth / 2.0f, i, 0.0f);
		glVertex3f(g_nGLWidth / 2.0f, i, 0.0f);
	}
	glEnd();
	glPopAttrib();
}

void DrawSphere() {
	glLoadName(1);
	glPushMatrix();
	glTranslatef(g_pPosition[0], g_pPosition[1], 0.0f);
	glColor3f(1, 0, 0);
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();

	glLoadName(2);
	glPushMatrix();
	glTranslatef(g_pPosition[2], g_pPosition[3], 0.0f);
	glColor3f(0, 1, 0);
	glutSolidSphere(25.0f, 30, 30);
	//gluPartialDisk(qobj, 0, 20, 10, 10, 0, 270);
	glPopMatrix();

	glLoadName(3);
	glPushMatrix();
	glTranslatef(g_pPosition[4], g_pPosition[5], 0.0f);
	glColor3f(0, 0, 1);
	glutSolidSphere(25.0f, 30, 30);
	//glutSolidCube(50);
	glPopMatrix();
}

void draw(void) // 기존 함수 변경
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	DrawBackground();
	DrawSphere();
	glFlush();
	glutSwapBuffers();
}

void resize(int width, int height) {
	g_nGLWidth = width;
	g_nGLHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, -100, 100);
	glMatrixMode(GL_MODELVIEW);
}

void Picking(int x, int y) {
	GLuint selectBuf[256];
	// first step - glSelectBuffer()를 이용하여 hit record를 저장할 공간을 지정
	glSelectBuffer(256, selectBuf);
	// second step - glRenderMode()를 이용하여 렌더링 모드를 선택 모드 (GL_SELECT)로 설정
	glRenderMode(GL_SELECT);
	// third step - glInitNames()와 glPushName()을 이용하여 name stack을 초기화
	glMatrixMode(GL_PROJECTION);
	glInitNames();
	glPushName(-1);

	glPushMatrix();
	glLoadIdentity();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluPickMatrix(x, y, 0.1, 0.1, viewport);

	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	DrawSphere();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	GLint hits;
	// sixth step
	hits = glRenderMode(GL_RENDER);	
	if (hits <= 0) return;

	int stack = selectBuf[0];
	unsigned int zMin = selectBuf[1];
	unsigned int zMax = selectBuf[2];
	g_nSelect = selectBuf[3];

	int index = 3 + stack;
	int i;
	for (i = 1; i < hits; i++) {
		stack = selectBuf[index];
		if (zMax > selectBuf[index + 2]) {
			zMax = selectBuf[index + 2];
			g_nSelect = selectBuf[index + 3];
		}
		index += 3 + stack;
	}
}

void mouse(int button, int state, int x, int y) {
	y = g_nGLHeight - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Picking(x, y);
		g_nX = x;
		g_nY = y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_nSelect = 0;
	}
}

void motion(int x, int y) {
	y = g_nGLHeight - y;
	if (g_nSelect > 0) {
		g_pPosition[(g_nSelect - 1) * 2 + 0] += x - g_nX;
		g_pPosition[(g_nSelect - 1) * 2 + 1] += y - g_nY;
		g_nX = x;
		g_nY = y;
		glutPostRedisplay();
	}
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(g_nGLWidth, g_nGLHeight);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Picking");
	init();

	/* 메인 메뉴*/
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Init", 1);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* 콜백 함수 정의 */
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}
