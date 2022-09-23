#include <gl/glut.h>
#include <stdio.h>
#include <windows.h>
//#include <GL/freeglut.h>

bool spin_state = true; // 회전여부 저장
double spin = 0; // 회전각도 저장
int draw_mode = 0; // 도형 선택
int x_trans = 0, y_trans = 0; // x, y 좌표 방향으로 평행 이동량
int rotation_direct = 0;

void init(void) {
    // 화면의 기본색으로 검은색 화면
    glClearColor(0.f, 0.f, 0.f, 1.0f);

    // 화면 좌표 정보 설정
    glMatrixMode(GL_PROJECTION);      // 3차원을 2차원으로 투영
    glLoadIdentity();
    gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

void idle(void) {
    if (spin_state) {
        spin += 0.1;      // 회전각도 0.1 증가
        if (spin > 360)
            spin -= 360;
    }
    glutPostRedisplay();
}

// 삼각형 그리기 함수
void draw_triangle(void) {
    // 면의 색상을 노란색으로
    glColor4f(0.0f, 1.0f, 0.0f, 0.75f);
    glBegin(GL_TRIANGLES);

    glVertex2i(100 + x_trans, 300 + y_trans);
    glVertex2i(100 + x_trans, 100 + y_trans);
    glVertex2i(300 + x_trans, 200 + y_trans);

    glEnd();
}

// 폴리곤 그리기 함수
void draw_polygon(void) {
    // 면의 색상을 노란색으로
    glColor4f(0.0f, 1.0f, 0.0f, 0.75f);
    glBegin(GL_POLYGON);

    glVertex2i(100, 300);
    glVertex2i(100, 100);
    glVertex2i(300, 100);
    glVertex2i(300, 200);

    glEnd();
}

void draw(void) {
    // 화면을 깨끗하게 지우기
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (rotation_direct == 0) { // 반시계 방향
        glTranslatef(250 + x_trans, 250 + y_trans, 0); // 평행이동
        glRotatef(spin, 0, 0, 1); // 회전
        glTranslatef(-250 - x_trans, -250 - y_trans, 0); // 평행이동
    }
    else { // 시계 방향
        glTranslatef(250 + x_trans, 250 + y_trans, 0); // 평행이동
        glRotatef(-spin, 0, 0, 1); // 회전
        glTranslatef(-250 - x_trans, -250 - y_trans, 0); // 평행이동
    }

    switch (draw_mode) {
    case 1:
        draw_triangle(); // 삼각형 그리기
        break;
    case 2:
        draw_polygon();
        break;
    default:
        break;
    }

    glFlush();
    glutSwapBuffers();
}

// Menu 처리 함수 - 메뉴 선택시 수행됨
void sub_menu_function(int option) {
    // Triangle 서브메뉴 선택 시
    if (option == 1) {
        printf("Submenu Triangle has been selected\n");
        draw_mode = 1;
    }
    // Polygon 서브메뉴 선택 시
    else if (option == 2) {
        printf("Submenu Polygon has been selected\n");
        draw_mode = 2;
    }
}

void main_menu_function(int option) {
    // Spin 메뉴 선택 시
    if (option == 1) {
        printf("You selected Spin menu\n");
        if (spin_state)
            spin_state = false;
        else
            spin_state = true;
    }
    // X Trans 메뉴 선택 시
    else if (option == 2) {
        printf("You selected X Trans menu\n");
        x_trans += 5;
    }
    // Y Trans 메뉴 선택 시
    else if (option == 3) {
        printf("You selected Y Trans menu\n");
        y_trans += 5;
    }
    // Quit 메뉴 선택 시
    else if (option == 999) {
        printf("You selected Quit menu\n");
        exit(0);
    }
    glutPostRedisplay();      // 윈도우 다시 그리기
}

// 키보드 입력 콜백 함수
void keyboard(unsigned char key, int x, int y) {
    printf("You pressed %c\n", key);
    if (key == 's')
        if (spin_state)
            spin_state = false;
        else
            spin_state = true;

    if (key == 'r')
        if (rotation_direct == 0)
            rotation_direct = 1;
        else
            rotation_direct = 0;

    glutPostRedisplay();
}

// 화살표 방향키 입력에 따른 도형 이동
void mySpecialKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT: //왼쪽 키
        x_trans -= 5;
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT: //오른쪽 키
        x_trans += 5;
        glutPostRedisplay();
        break;
    case GLUT_KEY_UP: //위 키
        y_trans += 5;
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN: //아래 키
        y_trans -= 5;
        glutPostRedisplay();
        break;
    }
}

int main(int argc, char** argv) {
    int submenu;

    // 윈도우 초기화
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("12181735 권혁규");
    init();      // 사용자 초기화 함수


    // 서브 메뉴 생성 및 추가
    submenu = glutCreateMenu(sub_menu_function);
    glutAddMenuEntry("Triangle", 1);
    glutAddMenuEntry("Polygon", 2);

    // 메인 메뉴 생성
    glutCreateMenu(main_menu_function);
    glutAddSubMenu("Shape", submenu);
    glutAddMenuEntry("Spin", 1);
    glutAddMenuEntry("X Trans", 2);
    glutAddMenuEntry("Y Trans", 3);
    glutAddMenuEntry("Quit", 999);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    /*// Create a single window with a keyboard and display callback
    glutMouseFunc(&mouse);
    glutMouseWheelFunc(&mouseWheel);
    glutMotionFunc(&motion);
    glutPassiveMotionFunc(&passiveMotion);
    glutSpecialFunc(&special_keyboard);
    glutDisplayFunc(&draw);*/

    // Idle 콜백 함수
    glutIdleFunc(idle);      // 컴퓨터의 유휴시간에 호출

    // 디스플레이 콜백 함수
    glutDisplayFunc(draw);      // draw() : 실제 그리기 함수

    // 키보드 콜백 함수
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(mySpecialKey);

    // Looping의 시작
    glutMainLoop();

    return 0;
}
