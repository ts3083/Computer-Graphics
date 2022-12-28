#include <GL/freeglut.h>
#include <GL/glext.h>
#include <gl/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "bmpfuncs.h"

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700
#define PI 3.14159265

double theta = 0;
double phi = 0;
int radius = 60;
double upVector = 1;
double g_nSkySize = 100;

/* camera location */
double eyex, eyey, eyez;

/* quadric object 생성 */
GLUquadricObj* qobj = gluNewQuadric();

/* texture mapping set variable */
GLuint cubeTex;

void init(void);
void resize(int, int);

void draw(void);
void draw_sphere(void);
void draw_skybox(void);


void mouseWheel(int, int, int, int);
void specialKeyboard(int, int, int);

bool inverse_matrix_4x4(GLfloat**, GLfloat**);

int main(int argc, char** argv) {
	/* Window 초기화 */
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // GLUT_DOUBLE, GLUT_RGBA로 권장

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);// 창의 크기 설정
	glutInitWindowPosition(400, 100);	// 창의 좌상단 좌표를 (0,0)로 설정
	glutCreateWindow("hello world");	// 창 이름 설정
	init();

	// CPU 유후 시간 처리
	//glutIdleFunc(idle);

	// Define Draw Callback
	glutDisplayFunc(draw);

	// 특수 키보드 방향키 처리
	glutSpecialFunc(specialKeyboard);

	// mouse wheel callback 함수
	glutMouseWheelFunc(mouseWheel);

	// main loop
	glutMainLoop();

	// delete object
	gluDeleteQuadric(qobj);
	return 0;
}

void draw(void) {
	/* 화면을 깨끗하게 지우기 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	eyex = radius * sin(theta * PI / 180) * cos(phi * PI / 180);
	eyey = radius * sin(phi * PI / 180);
	eyez = radius * cos(theta * PI / 180) * cos(phi * PI / 180);

	// 차례대로 x에서 보는 위치, y에서 보는 위치, z에서 보는 위치
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, upVector, 0);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();	//현재의 행렬을 저장

	GLfloat cam[16];
	GLfloat inverse_cam[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, cam);
	cam[14] = 0;

	GLfloat** cam_matrix, ** inverse_cam_matrix;
	cam_matrix = new GLfloat * [4];
	inverse_cam_matrix = new GLfloat * [4];
	for (int i = 0; i < 4; i++) {
		cam_matrix[i] = new GLfloat[4];
		inverse_cam_matrix[i] = new GLfloat[4];
	}
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			cam_matrix[row][col] = cam[row * 4 + col];
		}
	}
	inverse_matrix_4x4(cam_matrix, inverse_cam_matrix);
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			inverse_cam[row * 4 + col] = inverse_cam_matrix[row][col];
		}
	}

	glMultMatrixf(inverse_cam);
	glMatrixMode(GL_MODELVIEW);

	draw_skybox();

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();

	delete[] cam_matrix;
	delete[] inverse_cam_matrix;
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutReshapeFunc(resize);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	// light0 조명 관련 설정
	GLfloat ambientLight[] = { 0.2f,0.2f,0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f,0.7f,0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f,0.9f,0.9f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	GLfloat light_position[] = { eyex, eyey, eyez, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Material setting
	GLfloat specularMaterial[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void draw_sphere(void) {
	gluQuadricTexture(qobj, GL_TRUE);
	gluSphere(qobj, 10, 30, 30);

	glFlush();
}

void draw_skybox(void) {
	/* 텍스쳐 선언과 바인드 */
	glGenTextures(1, &cubeTex);
	int tex_width[6], tex_height[6], nrChannels[6];
	uchar* img[6];

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
	/*img[0] = stbi_load("img/256nx.bmp", &tex_width[0], &tex_height[0], &nrChannels[0], 0);
	img[1] = stbi_load("img/256px.bmp", &tex_width[1], &tex_height[1], &nrChannels[1], 0);
	img[2] = stbi_load("img/256py.bmp", &tex_width[2], &tex_height[2], &nrChannels[2], 0);
	img[3] = stbi_load("img/256ny.bmp", &tex_width[3], &tex_height[3], &nrChannels[3], 0);
	img[4] = stbi_load("img/256nz.bmp", &tex_width[4], &tex_height[4], &nrChannels[4], 0);
	img[5] = stbi_load("img/256pz.bmp", &tex_width[5], &tex_height[5], &nrChannels[5], 0);*/
	img[0] = stbi_load("img/256nz.bmp", &tex_width[0], &tex_height[0], &nrChannels[0], 0);
	img[1] = stbi_load("img/256pz.bmp", &tex_width[1], &tex_height[1], &nrChannels[1], 0);
	img[2] = stbi_load("img/256py.bmp", &tex_width[2], &tex_height[2], &nrChannels[2], 0);
	img[3] = stbi_load("img/256ny.bmp", &tex_width[3], &tex_height[3], &nrChannels[3], 0);
	img[4] = stbi_load("img/256nx.bmp", &tex_width[4], &tex_height[4], &nrChannels[4], 0);
	img[5] = stbi_load("img/256px.bmp", &tex_width[5], &tex_height[5], &nrChannels[5], 0);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, tex_width[0], tex_height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, img[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, tex_width[1], tex_height[1], 0, GL_RGB, GL_UNSIGNED_BYTE, img[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, tex_width[2], tex_height[2], 0, GL_RGB, GL_UNSIGNED_BYTE, img[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, tex_width[3], tex_height[3], 0, GL_RGB, GL_UNSIGNED_BYTE, img[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, tex_width[4], tex_height[4], 0, GL_RGB, GL_UNSIGNED_BYTE, img[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, tex_width[5], tex_height[5], 0, GL_RGB, GL_UNSIGNED_BYTE, img[5]);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

	draw_sphere();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_CUBE_MAP);


	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

	glBegin(GL_QUADS);
	glTexCoord3d(1, -1, -1);
	glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(1, -1, 1);
	glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(1, 1, 1);
	glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1, 1, -1);
	glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord3d(-1, -1, -1);
	glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1, -1, 1);
	glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1, 1, 1);
	glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(-1, 1, -1);
	glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord3d(1, 1, 1);
	glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1, 1, -1);
	glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1, 1, -1);
	glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1, 1, 1);
	glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord3d(1, -1, 1);
	glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(1, -1, -1);
	glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1, -1, -1);
	glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1, -1, 1);
	glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord3d(-1, -1, 1);
	glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1, 1, 1);
	glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1, 1, 1);
	glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1, -1, 1);
	glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord3d(-1, -1, -1);
	glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1, 1, -1);
	glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(1, 1, -1);
	glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(1, -1, -1);
	glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glEnd();
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	printf("WIDTH: %d HEIGHT: %d\n", width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}

void mouseWheel(int but, int dir, int x, int y) {
	if (dir > 0) {
		if (radius > 2) { radius--; printf("\'zoom in!\', radius: %d\n", radius); }
	}
	else {
		if (radius < 100) { radius++; printf("\'zoom out!\', radius: %d\n", radius); }
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
	default:
		printf("%d is pressed\n", key);
		break;
	}
	glutPostRedisplay();
}

bool inverse_matrix_4x4(GLfloat** a, GLfloat** result)
{
	float a_arr[16], inv[16], invout[16], det;
	int i, j;

	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 4; i++)
		{
			a_arr[i + 4 * j] = a[j][i];
		}
	}

	// 역행렬 계산 : https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
	inv[0] = a_arr[5] * a_arr[10] * a_arr[15] -
		a_arr[5] * a_arr[11] * a_arr[14] -
		a_arr[9] * a_arr[6] * a_arr[15] +
		a_arr[9] * a_arr[7] * a_arr[14] +
		a_arr[13] * a_arr[6] * a_arr[11] -
		a_arr[13] * a_arr[7] * a_arr[10];

	inv[4] = -a_arr[4] * a_arr[10] * a_arr[15] +
		a_arr[4] * a_arr[11] * a_arr[14] +
		a_arr[8] * a_arr[6] * a_arr[15] -
		a_arr[8] * a_arr[7] * a_arr[14] -
		a_arr[12] * a_arr[6] * a_arr[11] +
		a_arr[12] * a_arr[7] * a_arr[10];

	inv[8] = a_arr[4] * a_arr[9] * a_arr[15] -
		a_arr[4] * a_arr[11] * a_arr[13] -
		a_arr[8] * a_arr[5] * a_arr[15] +
		a_arr[8] * a_arr[7] * a_arr[13] +
		a_arr[12] * a_arr[5] * a_arr[11] -
		a_arr[12] * a_arr[7] * a_arr[9];

	inv[12] = -a_arr[4] * a_arr[9] * a_arr[14] +
		a_arr[4] * a_arr[10] * a_arr[13] +
		a_arr[8] * a_arr[5] * a_arr[14] -
		a_arr[8] * a_arr[6] * a_arr[13] -
		a_arr[12] * a_arr[5] * a_arr[10] +
		a_arr[12] * a_arr[6] * a_arr[9];

	inv[1] = -a_arr[1] * a_arr[10] * a_arr[15] +
		a_arr[1] * a_arr[11] * a_arr[14] +
		a_arr[9] * a_arr[2] * a_arr[15] -
		a_arr[9] * a_arr[3] * a_arr[14] -
		a_arr[13] * a_arr[2] * a_arr[11] +
		a_arr[13] * a_arr[3] * a_arr[10];

	inv[5] = a_arr[0] * a_arr[10] * a_arr[15] -
		a_arr[0] * a_arr[11] * a_arr[14] -
		a_arr[8] * a_arr[2] * a_arr[15] +
		a_arr[8] * a_arr[3] * a_arr[14] +
		a_arr[12] * a_arr[2] * a_arr[11] -
		a_arr[12] * a_arr[3] * a_arr[10];

	inv[9] = -a_arr[0] * a_arr[9] * a_arr[15] +
		a_arr[0] * a_arr[11] * a_arr[13] +
		a_arr[8] * a_arr[1] * a_arr[15] -
		a_arr[8] * a_arr[3] * a_arr[13] -
		a_arr[12] * a_arr[1] * a_arr[11] +
		a_arr[12] * a_arr[3] * a_arr[9];

	inv[13] = a_arr[0] * a_arr[9] * a_arr[14] -
		a_arr[0] * a_arr[10] * a_arr[13] -
		a_arr[8] * a_arr[1] * a_arr[14] +
		a_arr[8] * a_arr[2] * a_arr[13] +
		a_arr[12] * a_arr[1] * a_arr[10] -
		a_arr[12] * a_arr[2] * a_arr[9];

	inv[2] = a_arr[1] * a_arr[6] * a_arr[15] -
		a_arr[1] * a_arr[7] * a_arr[14] -
		a_arr[5] * a_arr[2] * a_arr[15] +
		a_arr[5] * a_arr[3] * a_arr[14] +
		a_arr[13] * a_arr[2] * a_arr[7] -
		a_arr[13] * a_arr[3] * a_arr[6];

	inv[6] = -a_arr[0] * a_arr[6] * a_arr[15] +
		a_arr[0] * a_arr[7] * a_arr[14] +
		a_arr[4] * a_arr[2] * a_arr[15] -
		a_arr[4] * a_arr[3] * a_arr[14] -
		a_arr[12] * a_arr[2] * a_arr[7] +
		a_arr[12] * a_arr[3] * a_arr[6];

	inv[10] = a_arr[0] * a_arr[5] * a_arr[15] -
		a_arr[0] * a_arr[7] * a_arr[13] -
		a_arr[4] * a_arr[1] * a_arr[15] +
		a_arr[4] * a_arr[3] * a_arr[13] +
		a_arr[12] * a_arr[1] * a_arr[7] -
		a_arr[12] * a_arr[3] * a_arr[5];

	inv[14] = -a_arr[0] * a_arr[5] * a_arr[14] +
		a_arr[0] * a_arr[6] * a_arr[13] +
		a_arr[4] * a_arr[1] * a_arr[14] -
		a_arr[4] * a_arr[2] * a_arr[13] -
		a_arr[12] * a_arr[1] * a_arr[6] +
		a_arr[12] * a_arr[2] * a_arr[5];

	inv[3] = -a_arr[1] * a_arr[6] * a_arr[11] +
		a_arr[1] * a_arr[7] * a_arr[10] +
		a_arr[5] * a_arr[2] * a_arr[11] -
		a_arr[5] * a_arr[3] * a_arr[10] -
		a_arr[9] * a_arr[2] * a_arr[7] +
		a_arr[9] * a_arr[3] * a_arr[6];

	inv[7] = a_arr[0] * a_arr[6] * a_arr[11] -
		a_arr[0] * a_arr[7] * a_arr[10] -
		a_arr[4] * a_arr[2] * a_arr[11] +
		a_arr[4] * a_arr[3] * a_arr[10] +
		a_arr[8] * a_arr[2] * a_arr[7] -
		a_arr[8] * a_arr[3] * a_arr[6];

	inv[11] = -a_arr[0] * a_arr[5] * a_arr[11] +
		a_arr[0] * a_arr[7] * a_arr[9] +
		a_arr[4] * a_arr[1] * a_arr[11] -
		a_arr[4] * a_arr[3] * a_arr[9] -
		a_arr[8] * a_arr[1] * a_arr[7] +
		a_arr[8] * a_arr[3] * a_arr[5];

	inv[15] = a_arr[0] * a_arr[5] * a_arr[10] -
		a_arr[0] * a_arr[6] * a_arr[9] -
		a_arr[4] * a_arr[1] * a_arr[10] +
		a_arr[4] * a_arr[2] * a_arr[9] +
		a_arr[8] * a_arr[1] * a_arr[6] -
		a_arr[8] * a_arr[2] * a_arr[5];

	det = a_arr[0] * inv[0] + a_arr[1] * inv[4] + a_arr[2] * inv[8] + a_arr[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
	{
		invout[i] = inv[i] * det;
	}

	// 1차원 배열 -> 2차원 배열
	// 다시 되돌리기.
	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 4; i++)
		{
			result[j][i] = invout[i + 4 * j];
		}
	}

	return true;
}
