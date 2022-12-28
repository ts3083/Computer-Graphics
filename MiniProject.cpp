#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "ObjParser.h"
#include "bmpfuncs.h"

#define M_PI 3.1415926535897
using std::cos;
using std::sin;
using std::sqrt;

// global variable for counting fps
volatile int frame = 0, timebase = 0;
volatile float fps;

unsigned char light = '1';

int dx = 0, dy = 0, dz = 0; // adwx 키 누를 시 translate 정도 저장
int spin_mode = 3; // default는 x1.0의 속도로 오르골 재생
bool spin_state = false; // 회전여부 저장
double spin = 0; // 회전각도 저장
double spin_handle = 0; // 손잡이 회전각도 저장

int imgWidth, imgHeight, channels;
uchar* img;
int texNum = 1;

/* 각 오브젝트들의 위치 좌표 */
float box_pos[3];
float upbox_pos[3];
float lp_pos[3];
float napal_pos[3];
float base_pos[3];
float cylinder_pos[3];
float wheel_pos[3];
float handle_pos[3];

/* box와 upbox 조립에 필요한 변수 */
bool assemble_upbox = false;

/* upbox와 lp 조립에 필요한 변수 */
float init_disty_upbox = 7;
float init_disty_lp = 8;

float disty_upbox = init_disty_upbox;
float disty_lp = init_disty_lp;
bool assemble_lp = false;

/* napal과 box 조립에 필요한 변수 */
float initx_napal = -2;
float inity_napal = 8;
float initz_napal = -5;

float distx_napal = initx_napal;
float disty_napal = inity_napal;
float distz_napal = initz_napal;
bool assemble_napal = false;

/* handle과 wheel 조립에 필요한 변수 */
float initx_handle = 11;
float inity_handle = 3;
float initz_handle = -2;

float distx_handle = initx_handle;
float disty_handle = inity_handle;
float distz_handle = initz_handle;

/* wheel과 cylinder 조립에 필요한 변수 */
float initx_wheel = 11;
float inity_wheel = 2;
float initz_wheel = -2;

float distx_wheel = initx_wheel;
float disty_wheel = inity_wheel;
float distz_wheel = initz_wheel;
bool assemble_handle = false;

/* cylinder와 base 조립에 필요한 변수 */
float initx_cylinder = 11;
float inity_cylinder = 2;
float initz_cylinder = 0;

float distx_cylinder = initx_cylinder;
float disty_cylinder = inity_cylinder;
float distz_cylinder = initz_cylinder;
bool assemble_wheel = false;

/* base와 box 조립에 필요한 변수 */
float initx_base = 10;
float inity_base = 0;
float initz_base = 0;

float distx_base = initx_base;
float disty_base = inity_base;
float distz_base = initz_base;
bool assemble_cylinder = false;
bool assemble_base = false;

/* texture mapping set variable */
GLuint textureBox;
GLuint textureUpbox;
GLuint textureNapal;
GLuint textureLp;
GLuint textureBase;
GLuint textureCylinder;
GLuint textureWheel;
GLuint textureHandle;

bool antialiase_on = true;
double radius = 30;
double theta = 45, phi = 45;
double cam[3];
double center[3] = { 0, 0, 0 };
double up[3] = { 0, 1, 0 };

// object var
ObjParser* napal;
ObjParser* wheel;
ObjParser* upbox;
ObjParser* lp;
ObjParser* handle;
ObjParser* cylinder;
ObjParser* box;
ObjParser* base;

// user-defined function
void init(void);
void light_default();
void w_light_setting();
void red_light_setting();
void green_light_setting();
void add_menu();
void mouse(int, int, int, int);
void mouseWheel(int, int, int, int);
void motion(int, int);
void passiveMotion(int, int);
void keyboard(unsigned char, int, int);
void special_keyboard(int, int, int);
void draw(void);
void resize(int, int);
void idle(void);
void draw_obj(ObjParser*);
//void draw_axis(void);
void check_wheel(void);
void check_cylinder(void);
void check_base(void);
void check_upbox(void);
void draw_obj_with_texture(GLint textureObj, ObjParser* objParser);
void setTextureMapping(void);
void printInstruction(void);
//...

GLUquadric* quadric;

void check_base(void) {
	if (assemble_cylinder) {
		distx_cylinder = distx_base + 0.5;
		disty_cylinder = disty_base + 0.8;
		distz_cylinder = distz_base - 0.23;
		check_cylinder();
	}
}

void check_cylinder(void) {
	if (assemble_wheel) {
		distx_wheel = distx_cylinder;
		disty_wheel = disty_cylinder;
		distz_wheel = distz_cylinder - 0.85;
		check_wheel();
	}
}

void check_wheel(void) {
	if (assemble_handle) {
		distx_handle = distx_wheel + 0.3;
		disty_handle = disty_wheel + 0.5;
		distz_handle = distz_wheel;
	}
}

void check_upbox(void) {
	if (assemble_lp) { // lp가 조립되어 있다면
		disty_lp = disty_upbox + 0.1;
	}
}

/* Display callback function */
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	cam[0] = radius * sin(theta * M_PI / 180) * sin(phi * M_PI / 180);
	cam[1] = radius * cos(theta * M_PI / 180);
	cam[2] = radius * sin(theta * M_PI / 180) * cos(phi * M_PI / 180);

	gluLookAt(cam[0], cam[1], cam[2], center[0], center[1], center[2], up[0], up[1], up[2]);
	/*GLfloat light_position_up[] = { 0, 100, 0, 1 };
	GLfloat light_position_down[] = { 0, -100, 0, 1 };*/
	GLfloat light_position_up[] = { 20, 20, 20, 1 };
	GLfloat light_position_down[] = { -20, -20, -20, 1 };
	if (light == '1') {
		glLightfv(GL_LIGHT0, GL_POSITION, light_position_up);
		glLightfv(GL_LIGHT1, GL_POSITION, light_position_down);
	}
	else if (light == '2') {
		glLightfv(GL_LIGHT2, GL_POSITION, light_position_up);
		glLightfv(GL_LIGHT3, GL_POSITION, light_position_down);
	}
	else if (light == '3') {
		glLightfv(GL_LIGHT4, GL_POSITION, light_position_up);
		glLightfv(GL_LIGHT5, GL_POSITION, light_position_down);
	}

	box_pos[0] = dx; box_pos[1] = dy; box_pos[2] = dz;
	upbox_pos[0] = dx; upbox_pos[1] = dy + disty_upbox; upbox_pos[2] = dz;
	lp_pos[0] = dx; lp_pos[1] = dy + disty_lp; lp_pos[2] = dz;
	napal_pos[0] = dx + distx_napal; napal_pos[1] = dy + disty_napal; napal_pos[2] = dz + distz_napal;
	base_pos[0] = dx + distx_base; base_pos[1] = dy + disty_base; base_pos[2] = dz + distz_base;
	cylinder_pos[0] = dx + distx_cylinder; cylinder_pos[1] = dy + disty_cylinder; cylinder_pos[2] = dz + distz_cylinder;
	wheel_pos[0] = dx + distx_wheel; wheel_pos[1] = dy + disty_wheel; wheel_pos[2] = dz + distz_wheel;
	handle_pos[0] = dx + distx_handle; handle_pos[1] = dy + disty_handle; handle_pos[2] = dz + distz_handle;

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	//draw_axis();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();

	/* box 그리기 */
	glTranslatef(box_pos[0], box_pos[1], box_pos[2]);
	draw_obj_with_texture(textureBox, box);

	glPopMatrix();

	/* upbox 그리기 */
	glTranslatef(upbox_pos[0], upbox_pos[1], upbox_pos[2]);
	draw_obj_with_texture(textureUpbox, upbox);

	glPopMatrix();

	/* lp 그리기 */
	glTranslatef(lp_pos[0], lp_pos[1], lp_pos[2]);
	glRotatef(spin, 0, 1, 0);

	draw_obj_with_texture(textureLp, lp);

	glPopMatrix();

	/* napal 그리기 */
	glTranslatef(napal_pos[0], napal_pos[1], napal_pos[2]);
	draw_obj_with_texture(textureNapal, napal);

	glPopMatrix();

	/* base 그리기 */
	glTranslatef(base_pos[0], base_pos[1], base_pos[2]);
	draw_obj_with_texture(textureBase, base);

	glPopMatrix();

	/* cylinder 그리기 */
	glTranslatef(cylinder_pos[0], cylinder_pos[1], cylinder_pos[2]);
	glRotatef(-spin, 0, 0, 1);

	draw_obj_with_texture(textureCylinder, cylinder);

	glPopMatrix();

	/* wheel 그리기 */
	glTranslatef(wheel_pos[0], wheel_pos[1], wheel_pos[2]);
	glRotatef(-spin, 0, 0, 1);

	draw_obj_with_texture(textureWheel, wheel);

	glPopMatrix();

	/* handel 그리기 */
	glTranslatef(handle_pos[0], handle_pos[1], handle_pos[2]);
	glRotatef(-spin_handle, 1, 0, 0);

	draw_obj_with_texture(textureHandle, handle);

	glutSwapBuffers();
	glFlush();
}

/* Keyboard callback function */
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		/* Exit on escape key press */
	case 'A':
	{
		if (antialiase_on) {
			antialiase_on = false;
			glDisable(GL_POLYGON_SMOOTH);
		}
		else
		{
			antialiase_on = true;
			glEnable(GL_POLYGON_SMOOTH);
		}
		break;
	}
	case '1': // white light on
	{
		printf("white light ON\n");
		light = '1';

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		break;
	}
	case '2': // red light on
	{
		printf("red light ON\n");
		light = '2';

		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		break;
	}
	case '3': // green light on
	{
		printf("green light ON\n");
		light = '3';

		glEnable(GL_LIGHT4);
		glEnable(GL_LIGHT5);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		break;
	}
	case 'r':
	{
		if (spin_state) {
			spin_state = false;
			PlaySound(NULL, 0, 0);
		}
		else {
			if (assemble_handle && assemble_wheel && assemble_cylinder) {
				spin_state = true;
				if (spin_mode == 1) { // 0.5
					PlaySound(TEXT("sound/sound_option1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				}
				else if (spin_mode == 2) { // 0.75
					PlaySound(TEXT("sound/sound_option2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				}
				else if (spin_mode == 3) { // 1.0
					PlaySound(TEXT("sound/sound_option3.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				}
				else if (spin_mode == 4) { // 1.25
					PlaySound(TEXT("sound/sound_option4.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				}
				else if (spin_mode == 5) { // 1.5
					PlaySound(TEXT("sound/sound_option5.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				}
				else if (spin_mode == 6) { // 1.75
					PlaySound(TEXT("sound/sound_option6.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				}
				else if (spin_mode == 7) { // 2.0
					PlaySound(TEXT("sound/sound_option7.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				}
			}
		}
		break;
	}
	case 'a': // 물체 전체 -x방향 이동
	{
		dx -= 1;
		break;
	}
	case 'd': // 물체 전체 +x방향 이동
	{
		dx += 1;
		break;
	}
	case 'w': // 물체 전체 +y방향 이동
	{
		dy += 1;
		break;
	}
	case 'x': // 물체 전체 -y방향 이동
	{
		dy -= 1;
		break;
	}
	case '!': // base를 box의 위치로 조립
	{
		if (assemble_base) { // 분해하여 base 원치로 옮기기
			assemble_base = false;
			distx_base = initx_base;
			disty_base = inity_base;
			distz_base = initz_base;
		}
		else { // 조립
			assemble_base = true;
			distx_base = 2.5;
			disty_base = 0;
			distz_base = 1.1;
		}		
		check_base();
		break;
	}
	case '@': // cylinder를 base의 위치로 조립
	{
		if (assemble_cylinder) { // 분해하여 cylinder 원위치로 옮기기
			assemble_cylinder = false;
			distx_cylinder = initx_cylinder;
			disty_cylinder = inity_cylinder;
			distz_cylinder = initz_cylinder;
		}
		else { // 조립
			assemble_cylinder = true; // base에 cylinder를 조립 완료한 상태
			distx_cylinder = distx_base + 0.5;
			disty_cylinder = disty_base + 0.8;
			distz_cylinder = distz_base - 0.23;
		}
		check_cylinder();
		break;
	}
	case '#': // wheel을 cylinder의 위치로 조립
	{
		if (assemble_wheel) { // 분해하여 wheel 원위치로 옮기기
			assemble_wheel = false;
			distx_wheel = initx_wheel;
			disty_wheel = inity_wheel;
			distz_wheel = initz_wheel;
		}
		else { // 조립
			assemble_wheel = true;
			distx_wheel = distx_cylinder;
			disty_wheel = disty_cylinder;
			distz_wheel = distz_cylinder - 0.85;
		}
		check_wheel();
		break;
	}
	case '$': // handle을 wheel의 위치로 조립
	{
		if (assemble_handle) { // 분해하여 handle 원위치로 옮기기
			assemble_handle = false;
			distx_handle = initx_handle;
			disty_handle = inity_handle;
			distz_handle = initz_handle;
		}
		else { // 조립
			assemble_handle = true;
			distx_handle = distx_wheel + 0.3;
			disty_handle = disty_wheel + 0.5;
			distz_handle = distz_wheel;
		}
		break;
	}
	case '%': // box와 upbox 조립
	{
		if (assemble_upbox) { // 분해하여 upbox를 원위치로 옮기기
			assemble_upbox = false;
			disty_upbox = init_disty_upbox;
		}
		else {
			assemble_upbox = true;
			disty_upbox = 3.5;
		}
		check_upbox();
		break;
	}
	case '^': // upbox와 lp 조립
	{
		if (assemble_lp) { // 분해하여 lp를 원위치로 옮기기
			assemble_lp = false;
			disty_lp = init_disty_lp;
		}
		else { // 조립
			assemble_lp = true;
			disty_lp = disty_upbox + 0.1;
		}
		break;
	}
	case '&': // napal 조립
	{
		if (assemble_napal) { // 분해하여 napal을 원위치로 옮기기
			assemble_napal = false;
			distx_napal = initx_napal;
			disty_napal = inity_napal;
			distz_napal = initz_napal;
		}
		else { // 조립
			assemble_napal = true;
			distx_napal = -0.5;
			disty_napal = 2.8;
			distz_napal = -3.5;
		}
	}
	}
	glutPostRedisplay();
}

void printInstruction(void) {
	// 조작접 console 출력
	printf("\n------------ keyboard navigation ------------\n");
	printf("방향키: camera 위치\n");
	printf("마우스 휠: zoom in + zoom out\n");

	printf("\n------------ light navigation ------------\n");
	printf("\'1\': white light\n");
	printf("\'2\': red light\n");
	printf("\'3\': green light\n");

	printf("\n------------ 전체적으로 위치 변경 ------------\n");
	printf("\'a\': -x방향으로 이동\n");
	printf("\'w\': +y방향으로 이동\n");
	printf("\'d\': +x방향으로 이동\n");
	printf("\'x\': -x방향으로 이동\n");

	printf("\n------------ 오르골 조립 navigation ------------\n");
	printf("shift 누른 채로 1~7 : 1번 누르면 조립 다시 누르면 분해\n");
	printf("\'!\': 진동판과 상자 조립\n");
	printf("\'@\': 실린더와 진동판 조립\n");
	printf("\'#\': 톱니바퀴와 실린더 조립\n");
	printf("\'$\': 손잡이와 톱니바퀴 조립\n");
	printf("\'%\': 상자 윗부분 조립\n");
	printf("\'^\': lp판 조립\n");
	printf("\'&\': 트럼펫 조립\n");

	printf("\n------------ 오르골 재생 navigation ------------\n");
	printf("오르골을 조립해야만 재생 가능 + submenu에서 오르골 재생 속도 선택가능\n");
	printf("r: 1번 누르면 재생 다시 누르면 정지\n");

	printf("\n------------ 텍스처 변환 ------------\n");
	printf("submenu에서 trumpet과 lp의 텍스처 변환 가능\n");
}

void idle(void) {
	if (spin_state) {
		if (spin_mode == 1) { // 0.5
			spin += 0.01; spin_handle += 0.16;
		}
		else if (spin_mode == 2) { // 0.75
			spin += 0.03; spin_handle += 0.48;
		}
		else if (spin_mode == 3) { // 1.0
			spin += 0.05; spin_handle += 0.8;
		}
		else if (spin_mode == 4) { // 1.25
			spin += 0.08; spin_handle += 1.28;
		}
		else if (spin_mode == 5) { // 1.5
			spin += 0.12; spin_handle += 1.92;
		}
		else if (spin_mode == 6) { // 1.75
			spin += 0.2; spin_handle += 3.2;
		}
		else if (spin_mode == 7) { // 2.0
			spin += 0.3; spin_handle += 4.8;
		}
		if (spin > 360)
			spin -= 360;
		if (spin_handle > 360)
			spin_handle -= 360;
	}
	glutPostRedisplay();
}

void w_light_setting(void) {
	/* Light0 조명 관련 설정 */
	GLfloat ambientLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	/* Light1 조명 관련 설정 */
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
}

void red_light_setting() {
	/* Light2 조명 관련 설정 */
	GLfloat ambientLight[] = { 0.8f, 0.0f, 0.0f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight);

	/* Light3 조명 관련 설정 */
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specularLight);
}

void green_light_setting() {
	/* Light4 조명 관련 설정 */
	GLfloat ambientLight[] = { 0.0f, 0.8f, 0.0f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT4, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT4, GL_SPECULAR, specularLight);

	/* Light5 조명 관련 설정 */
	glLightfv(GL_LIGHT5, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT5, GL_SPECULAR, specularLight);
}

void light_default() {
	glClearColor(0.f, 0.f, 0.f, 1.0f);

	w_light_setting();
	red_light_setting();
	green_light_setting();	

	/************* spot position setting *************/
	/*GLfloat spot_direction[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);*/

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glDisable(GL_COLOR_MATERIAL);

	GLfloat specularMaterial[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	/************* Material  setting *************/
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 100);

	// 조명 스위치와 0번 조명 스위치 켜기
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);
	glDisable(GL_LIGHT5);

	GLfloat lmodel_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // 조명빛의 색을 조절
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	/* DEPTH TEST ENABLE */
	glFrontFace(GL_CW);	// CW CCW바꿔보면서 front face 변경해보기!
}

void setTextureMapping() {
	img = readImageData("image/yellow_metal.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &textureNapal);
	glBindTexture(GL_TEXTURE_2D, textureNapal);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, img);

	img = readImageData("image/lp_texture.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &textureLp);
	glBindTexture(GL_TEXTURE_2D, textureLp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	img = readImageData("image/wood2.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &textureUpbox);
	glBindTexture(GL_TEXTURE_2D, textureUpbox);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	img = readImageData("image/box_texture.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &textureBox);
	glBindTexture(GL_TEXTURE_2D, textureBox);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	img = readImageData("image/grey.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &textureCylinder);
	glBindTexture(GL_TEXTURE_2D, textureCylinder);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	img = readImageData("image/grey.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &textureWheel);
	glBindTexture(GL_TEXTURE_2D, textureWheel);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	img = readImageData("image/grey.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	img = readImageData("image/base_texture.bmp", &imgWidth, &imgHeight, &channels);
	glGenTextures(texNum, &textureBase);
	glBindTexture(GL_TEXTURE_2D, textureBase);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void init()
{
	printf("init func called\n");
	// clear background color
	glClearColor(0.f, 0.f, 0.f, 1.f);

	// set blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// set antialiasing
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_LINE_SMOOTH);

	light_default();

	/* TEXTURE MAPPING SET */
	glEnable(GL_TEXTURE_2D);
	setTextureMapping();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //GL_REPLACE : polygon의 원래 색상은 무시하고 texture로 덮음

	/* 조작법 출력 */
	printInstruction();
}

void special_keyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT) {
		phi -= 5;
		if (phi < 0) phi = 355;
	}
	else if (key == GLUT_KEY_RIGHT) {
		phi += 5;
		if (phi >= 360) phi = 0;
	}
	else if (key == GLUT_KEY_UP) {
		if (theta > 10) theta -= 5;
	}
	else if (key == GLUT_KEY_DOWN) {
		if (theta < 170) theta += 5;
	}

	//std::cout << "theta : " << theta << ", phi : " << phi << "\n";
	glutPostRedisplay();
}

//void draw_axis(void)
//{
//	glLineWidth(1.5f);
//	glBegin(GL_LINES);
//
//	glColor4f(1.f, 0.f, 0.f, 1.f);
//	glVertex3f(0.f, 0.f, 0.f);
//	glVertex3f(4.f, 0.f, 0.f);
//
//	glColor4f(0.f, 1.f, 0.f, 1.f);
//	glVertex3f(0.f, 0.f, 0.f);
//	glVertex3f(0.f, 4.f, 0.f);
//
//	glColor4f(0.f, 0.f, 1.f, 1.f);
//	glVertex3f(0.f, 0.f, 0.f);
//	glVertex3f(0.f, 0.f, 4.f);
//
//	glEnd();
//	glLineWidth(1);
//}

void draw_obj(ObjParser* objParser)
{
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
}

void mouse(int button, int state, int x, int y)
{
	if (state) {
		// printf("button(%d), state(%d), x(%d), y(%d)\n", button, state, x, y);
	}
	else {
		// printf("button(%d), state(%d), x(%d), y(%d)\n", button, state, x, y);
	}
	glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0) {
		// printf("button(%d), dir(%d), x(%d), y(%d)\n", button, dir, x, y);
		if (radius > 1) radius -= 0.5;
	}
	else {
		// printf("button(%d), dir(%d), x(%d), y(%d)\n", button, dir, x, y);
		if (radius < 100) radius += 0.5;
	}
	glutPostRedisplay();
}

void passiveMotion(int x, int y)
{
	//printf("Mouse movement x, y = (%d, %d)\n", x, y);
	glutPostRedisplay();
}

void motion(int x, int y)
{
	//printf("Mouse movement x, y = (%d, %d)\n", x, y);
	glutPostRedisplay();
}

void main_menu(int option)
{
	if (option == 99) exit(0);
	else if (option == 1) { // init
		radius = 30;
		theta = 45; phi = 45;
		spin_state = false;
		assemble_base = false;
		assemble_cylinder = false;
		assemble_handle = false;
		assemble_lp = false;
		assemble_upbox = false;
		assemble_wheel = false;
		assemble_napal = false;
		
		spin_mode = 3;

		dx = 0; dy = 0; dz = 0;
		disty_upbox = init_disty_upbox;
		disty_lp = init_disty_lp;
		distx_napal = initx_napal; disty_napal = inity_napal; distz_napal = initz_napal;
		distx_base = initx_base; disty_base = inity_base; distz_base = initz_base;
		distx_cylinder = initx_cylinder; disty_cylinder = inity_cylinder; distz_cylinder = initz_cylinder;
		distx_wheel = initx_wheel; disty_wheel = inity_wheel; distz_wheel = initz_wheel;
		distx_handle = initx_handle; disty_handle = inity_handle; distz_handle = initz_handle;
	}
	glutPostRedisplay();
}

void sub_menu1(int option)
{
	if (option == 1) { // 0.5
		spin_mode = 1;
		printf("음악 재생 속도 x0.5\n");
		if (spin_state) {
			PlaySound(NULL, 0, 0);
			PlaySound(TEXT("sound/sound_option1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
	}
	else if (option == 2) { // 0.75
		spin_mode = 2;
		printf("음악 재생 속도 x0.75\n");
		if (spin_state) {
			PlaySound(NULL, 0, 0);
			PlaySound(TEXT("sound/sound_option2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
	}
	else if (option == 3) { // 1.0
		spin_mode = 3;
		printf("음악 재생 속도 x1.0\n");
		if (spin_state) {
			PlaySound(NULL, 0, 0);
			PlaySound(TEXT("sound/sound_option3.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
	}
	else if (option == 4) { // 1.25
		spin_mode = 4;
		printf("음악 재생 속도 x1.25\n");
		if (spin_state) {
			PlaySound(NULL, 0, 0);
			PlaySound(TEXT("sound/sound_option4.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
	}
	else if (option == 5) { // 1.5
		spin_mode = 5;
		printf("음악 재생 속도 x1.5\n");
		if (spin_state) {
			PlaySound(NULL, 0, 0);
			PlaySound(TEXT("sound/sound_option5.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
	}
	else if (option == 6) { // 1.75
		spin_mode = 6;
		printf("음악 재생 속도 x1.75\n");
		if (spin_state) {
			PlaySound(NULL, 0, 0);
			PlaySound(TEXT("sound/sound_option6.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
	}
	else if (option == 7) { // 2.0
		spin_mode = 7;
		printf("음악 재생 속도 x2.0\n");
		if (spin_state) {
			PlaySound(NULL, 0, 0);
			PlaySound(TEXT("sound/sound_option7.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
	}
}

void sub_menu2(int option) { // trumpet texture 변경
	if (option == 1) { // yellow metal
		img = readImageData("image/yellow_metal.bmp", &imgWidth, &imgHeight, &channels);
		glGenTextures(texNum, &textureNapal);
		glBindTexture(GL_TEXTURE_2D, textureNapal);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else if (option == 2) { // rough yellow metal
		img = readImageData("image/rough_yellow_metal.bmp", &imgWidth, &imgHeight, &channels);
		glGenTextures(texNum, &textureNapal);
		glBindTexture(GL_TEXTURE_2D, textureNapal);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else if (option == 3) { // yellow
		img = readImageData("image/yellow.bmp", &imgWidth, &imgHeight, &channels);
		glGenTextures(texNum, &textureNapal);
		glBindTexture(GL_TEXTURE_2D, textureNapal);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}

void sub_menu3(int option) { // lp texture 변경
	if (option == 1) {
		img = readImageData("image/lp_texture.bmp", &imgWidth, &imgHeight, &channels);
		glGenTextures(texNum, &textureLp);
		glBindTexture(GL_TEXTURE_2D, textureLp);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else if (option == 2) {
		img = readImageData("image/yellow.bmp", &imgWidth, &imgHeight, &channels);
		glGenTextures(texNum, &textureLp);
		glBindTexture(GL_TEXTURE_2D, textureLp);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}

/* Main method */
int main(int argc, char** argv)
{
	int submenu1, submenu2, submenu3;

	// glut initialize
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1000, 800);
	glutInitWindowPosition(550, 0);
	glutCreateWindow("hello world");	// 위의 순서 무조건 고정

	// 사용자 초기화 함수
	init();
	quadric = gluNewQuadric();

	/* 회전 속도 결정 서브 메뉴 */
	submenu1 = glutCreateMenu(sub_menu1);
	glutAddMenuEntry("x0.5", 1);
	glutAddMenuEntry("x0.75", 2);
	glutAddMenuEntry("x1", 3);
	glutAddMenuEntry("x1.25", 4);
	glutAddMenuEntry("x1.5", 5);
	glutAddMenuEntry("x1.75", 6);
	glutAddMenuEntry("x2", 7);

	/* 나팔 색 결정 서브 메뉴 */
	submenu2 = glutCreateMenu(sub_menu2);
	glutAddMenuEntry("yellow metal", 1);
	glutAddMenuEntry("rough yellow metal", 2);
	glutAddMenuEntry("yellow", 3);

	/* lp 색 결정 서브 메뉴 */
	submenu3 = glutCreateMenu(sub_menu3);
	glutAddMenuEntry("lp", 1);
	glutAddMenuEntry("yellow", 2);

	// 메인 메뉴
	glutCreateMenu(&main_menu);
	glutAddMenuEntry("Init", 1);
	glutAddSubMenu("play mode", submenu1);
	glutAddSubMenu("trumpet texture change", submenu2);
	glutAddSubMenu("lp texture change", submenu3);
	glutAddMenuEntry("Quit", 99);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// 리소스 로드
	//obj1 = new ObjParser("1.obj");
	napal = new ObjParser("image/napal.obj");
	box = new ObjParser("image/box.obj");
	upbox = new ObjParser("image/upbox.obj");
	wheel = new ObjParser("image/wheel.obj");
	lp = new ObjParser("image/lp.obj");
	handle = new ObjParser("image/handle.obj");
	cylinder = new ObjParser("image/cylinder.obj");
	base = new ObjParser("image/base.obj");

	//monkey->write("test3.obj");
	/* Create a single window with a keyboard and display callback */
	glutMouseFunc(&mouse);
	glutMouseWheelFunc(&mouseWheel);
	glutMotionFunc(&motion);
	glutPassiveMotionFunc(&passiveMotion);
	glutKeyboardFunc(&keyboard);
	glutSpecialFunc(&special_keyboard);
	glutDisplayFunc(&draw);
	glutReshapeFunc(&resize);
	glutIdleFunc(&idle);

	/* Run the GLUT event loop */
	glutMainLoop();

	return EXIT_SUCCESS;
}

void resize(int width, int height)
{
	printf("resize func called\n");
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, 500, 0, 500);
	gluPerspective(60, (double)width / (double)height, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
}

void draw_obj_with_texture(GLint textureObj, ObjParser* objParser)
{
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);	// texture 색 보존을 위한 enable
	glBindTexture(GL_TEXTURE_2D, textureObj);
	//glBindTexture(GL_TEXTURE_2D, textureObj);
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glTexCoord2f(objParser->textures[objParser->textureIdx[n] - 1].x,
			objParser->textures[objParser->textureIdx[n] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 1] - 1].x,
			objParser->textures[objParser->textureIdx[n + 1] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 2] - 1].x,
			objParser->textures[objParser->textureIdx[n + 2] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
	glEnable(GL_BLEND);
}

