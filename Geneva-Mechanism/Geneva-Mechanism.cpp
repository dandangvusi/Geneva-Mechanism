#include <math.h>
#include <iostream>
#include "supportClass.h"
#include "Mesh.h"

using namespace std;

#define PI				3.14159265358979323846
#define DRIVEN_WHEEL_E	0.3

// Tham so quan he giua goc quay cua "rotate wheel" va "driven wheel"
float drivenWheelAngle[121] = 
{
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000,
0.000000000, 
0.050000000,
0.071428571,
0.107142857,
0.142857143,
0.185714286,
0.228571429,
0.492857143,
0.850000000,
1.357142857,
1.928571429,
2.428571429,
3.000000000,
3.500000000,
4.178571429,
4.750000000,
5.428571429,
6.142857143,
6.928571429,
7.678571429,
8.500000000,
9.285714286,
10.14285714,
10.96428571,
11.78571429,
12.64285714,
13.50000000,
14.39285714,
15.32142857,
16.28571429,
17.28571429,
18.21428571,
19.21428571,
20.21428571,
21.21428571,
22.14285714,
23.10714286,
24.03571429,
25.00000000,
25.96428571,
26.96428571,
28.00000000,
29.07142857,
30.00000000,
30.92857143,
32.00000000,
33.03571429,
34.03571429,
35.00000000,
35.96428571,
36.89285714,
37.85714286,
38.78571429,
39.78571429,
40.78571429,
41.78571429,
42.71428571,
43.71428571,
44.67857143,
45.60714286,
46.50000000,
47.35714286,
48.21428571,
49.03571429,
49.85714286,
50.71428571,
51.50000000,
52.32142857,
53.07142857,
53.85714286,
54.57142857,
55.25000000,
55.82142857,
56.50000000,
57.00000000,
57.57142857,
58.07142857,
58.64285714,
59.15000000,
59.50714286,
59.77142857,
59.81428571,
59.85714286,
59.89285714,
59.92857143,
59.95000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000,
60.00000000
};

// Tham so cho cua so hien thi
int		screenWidth = 1000;
int		screenHeight = 1000;

// Meshes
Mesh	basePlate;
Mesh	drivenWheel;
Mesh	rotateWheel;

// Tham so cho camera
float camera_angle;
float camera_height;
float camera_distance;
float camera_X, camera_Y, camera_Z;
float lookAt_X, lookAt_Y, lookAt_Z;

// Tham so chung cua vat the
int  nSegment = 20;
bool bDrawWireFrame = false;
bool bTopView = false;
bool bAnimate = false;
bool bSecondLight = false;

// Tham so kich thuoc cua base plate
float BasePlate_Size = 5.0;
float BasePlate_Height = 0.5;
float BasePlate_BaseHeight = 0.0;

// Tham so kich thuoc cua driven wheel
float DW_O1_O2_len = 1.1;
float DW_fO1InRadius = 0.9;
float DW_fO1OutRadius = 2.0;
float DW_fO2Radius = 0.7;
float DW_fHeight = 1.0;
int DW_iOpenAngle1 = 60;
int DW_iOpenAngle2 = 100;
float DW_fBaseObjectHeight = 0.5;

// Tham so kich thuoc cua rotate wheel
Point3 RW_O1{ 0.0, 0.0, 0.0 };	// tam cua shape2
float RW_fShape2HigherHeight = 0.5;
float RW_fShape2LowerHeight = 0.25;
float RW_fShape2InRadius = 0.5;
float RW_fShape2OutRadius = 0.7;
int RW_iShape2OpenAngle = 90;
int RW_iShape2CentralLeanAngle = 0;
float RW_fCylinderHeight = 1.0;
float RW_fCylinderRadius = DRIVEN_WHEEL_E * sin((DW_iOpenAngle1 / 2) * ((2 * PI) / 360));
float RW_O1_O2_len = 0.7; // khoang cach cua tam shape2 (O1) va tam cua cylinder (O2)
float RW_fShape2BaseHeight = 1.0;
float RW_fCylinderBaseHeight = 0.5;

void drawAxis()
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(5, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 5, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 5);
	glEnd();
}

void drawBasePlate() {
	// dich chuyen truc toa do den tam quay cua "rotate wheel"
	glPushMatrix();
	glTranslated(0, 0, 0);
	glRotatef(basePlate.fRotateAngleY, 0, 1, 0);

	// cac thong so mau va anh sang cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat shininess = 100.0;
	basePlate.setupMaterial(ambient, diffuse, specular, shininess);

	if (bDrawWireFrame) {
		basePlate.DrawWireframe();
	}
	else {
		basePlate.Draw();
	}
	glPopMatrix();
}

void drawDrivenWheel() {
	// dich chuyen truc toa do den tam quay cua "rotate wheel"
	glPushMatrix();
	glTranslated(0, 0, 0);
	glRotatef(drivenWheel.fRotateAngleY, 0, 1, 0);

	// cac thong so mau va anh sang cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 0.2, 0.2, 1.0, 1.0 };
	GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat shininess = 100.0;
	drivenWheel.setupMaterial(ambient, diffuse, specular, shininess);

	if (bDrawWireFrame) {
		drivenWheel.DrawWireframe();
	}
	else {
		drivenWheel.Draw();
	}
	glPopMatrix();
}

void drawRotateWheel() {
	// dich chuyen truc toa do den tam quay cua "rotate wheel"
	glPushMatrix();
	glTranslated(0, 0, 1.4);
	// xoay "rotate wheel"
	glRotatef(rotateWheel.fRotateAngleY, 0, 1, 0);

	// cac thong so mau va anh sang cua vat the
	GLfloat diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat shininess = 100.0;
	rotateWheel.setupMaterial(ambient, diffuse, specular, shininess);

	if (bDrawWireFrame) {
		rotateWheel.DrawWireframe();
	}
	else {
		rotateWheel.Draw();
	}
	glPopMatrix();
}

void drawObject() {
	drawRotateWheel();
	drawBasePlate();
	drawDrivenWheel();
}

void createObject() {
	// tao base plate
	basePlate.CreateBasePlate(BasePlate_Size, BasePlate_Height, BasePlate_BaseHeight);
	basePlate.SetColor(3);
	basePlate.CalculateFacesNorm();

	// tao driven wheel
	drivenWheel.CreateDrivenWheel(nSegment, DW_O1_O2_len, DW_fO1InRadius, DW_fO1OutRadius, DW_fO2Radius, DW_fHeight, DW_iOpenAngle1, DW_iOpenAngle2, DW_fBaseObjectHeight);
	drivenWheel.SetColor(7);
	drivenWheel.CalculateFacesNorm();

	// tao rotate wheel
	rotateWheel.CreateRotateWheel(nSegment, RW_O1, RW_fShape2HigherHeight, RW_fShape2LowerHeight, RW_fShape2InRadius, RW_fShape2OutRadius, RW_iShape2OpenAngle, RW_iShape2CentralLeanAngle, RW_fCylinderHeight, RW_fCylinderRadius, RW_O1_O2_len, RW_fShape2BaseHeight, RW_fCylinderBaseHeight);
	rotateWheel.SetColor(10);
	rotateWheel.CalculateFacesNorm();
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GLfloat light_position0[] = { 6.0, 6.0, 6.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	if (bSecondLight)
	{
		glEnable(GL_LIGHT1);

		GLfloat diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat specular1[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat position1[] = { -6.0, 6.0, -6.0, 0.0 };

		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
		glLightfv(GL_LIGHT1, GL_POSITION, position1);
	}
	else {
		glDisable(GL_LIGHT1);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// che do nhin tu tren xuong voi phep chieu truc giao
	if (bTopView) {
		glOrtho(-3, 3, -3, 3, 1, 10);
		gluLookAt(0, 6, 0, 0, 0, 0, 0, 0, 1);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, screenWidth, screenHeight);

		drawAxis();
		drawObject();

		glFlush();
		glutSwapBuffers();
	}
	// che do nhin camera
	else {
		/*float screen_rat = (float)screenWidth / (float)screenHeight;
		glFrustum(-screen_rat, screen_rat, -1.0, 1.0, 1.5, 100.0);*/
		camera_X = camera_distance * sinf(camera_angle * PI / 180);
		camera_Y = camera_height;
		camera_Z = camera_distance * cosf(camera_angle * PI / 180);
		if (camera_distance == 0)
		{
			gluLookAt(camera_X, camera_Y, camera_Z, lookAt_X, lookAt_Y, lookAt_Z, sinf(camera_angle * PI / 180), 0, cosf(camera_angle * PI / 180));
		}
		else
		{
			gluLookAt(camera_X, camera_Y, camera_Z, lookAt_X, lookAt_Y, lookAt_Z, 0, 1, 0);
		}
		glViewport(0, 0, screenWidth, screenHeight);

		// Clear the stencil buffers
		glClearStencil(0);
		// Clear depth
		glClearDepth(1.0f);
		// Draw
		drawAxis();
		drawObject();

		glFlush();
		glutSwapBuffers();
	}
}

float findDrivenAngle(float RotateAngle) {
	int rotate_cycle_nums = (int)RotateAngle / 360;
	int rotate_remain = (int)RotateAngle % 360;
	int index = rotate_remain / 3;
	return rotate_cycle_nums * 60 + drivenWheelAngle[index];
}

void myKeyboard(unsigned char key, int x, int y) {
	Point3 O{0, 0, 0};
	float O_Ox_angle = 0;
	int index = 0;
	switch (key)
	{
		case 'd':
		case 'D':
			bSecondLight = !bSecondLight;
			break;
		case 'w':
		case 'W':
			bDrawWireFrame = !bDrawWireFrame;
			break;
		case 'v':
		case 'V':
			bTopView = !bTopView;
			break;
		case '+':
			camera_distance += 0.2;
			break;
		case '-':
			camera_distance -= 0.2;
			break;
		case '1':
			rotateWheel.fRotateAngleY += 3;
			if (rotateWheel.fRotateAngleY > 2160) {
				rotateWheel.fRotateAngleY -= 2160;
			}
			drivenWheel.fRotateAngleY = findDrivenAngle(rotateWheel.fRotateAngleY);
			basePlate.fRotateAngleY = findDrivenAngle(rotateWheel.fRotateAngleY);
			break;
		case '2':
			rotateWheel.fRotateAngleY -= 3;
			if (rotateWheel.fRotateAngleY < 0) {
				rotateWheel.fRotateAngleY += 2160;
			}
			drivenWheel.fRotateAngleY = findDrivenAngle(rotateWheel.fRotateAngleY);
			basePlate.fRotateAngleY = findDrivenAngle(rotateWheel.fRotateAngleY);
			break;
	}
	glutPostRedisplay();
}

void mySpecialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		camera_height += 0.2;
		break;
	case GLUT_KEY_DOWN:
		camera_height -= 0.2;
		break;
	case GLUT_KEY_RIGHT:
		camera_angle += 3;
		break;
	case GLUT_KEY_LEFT:
		camera_angle -= 3;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void Timer(int value)
{
	if (bAnimate)
	{
		rotateWheel.fRotateAngleY += 3;
		if (rotateWheel.fRotateAngleY > 2160) {
			rotateWheel.fRotateAngleY -= 2160;
		}
		drivenWheel.fRotateAngleY = findDrivenAngle(rotateWheel.fRotateAngleY);
		basePlate.fRotateAngleY = findDrivenAngle(rotateWheel.fRotateAngleY);
	}
	glutTimerFunc(10, Timer, 0);
	glutPostRedisplay();
}

void myInit()
{
	// Khoi tao thong so cho camera
	camera_angle = -45;		// Goc quay cua camera so voi truc Oy
	camera_height = 4;		// Chieu cao cua camera so voi mat phang xOz
	camera_distance = 6;	// Khoang cach cua camera so voi truc Oy
	lookAt_X = 0;
	lookAt_Y = 1;
	lookAt_Z = 0;

	// Timer for animation
	//glutTimerFunc(50, Timer, 0);

	// View config
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float screen_rat = (float)screenWidth / (float)screenHeight;
	glFrustum(-screen_rat, screen_rat, -1.0, 1.0, 1.5, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// lightning
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat light_model_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_model_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	GLfloat light_ambient0[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
}

void printMenu() {
	cout << "1\t\tQuay nguoc chieu kim dong ho" << endl;
	cout << "2\t\tQuay cung chieu kim dong ho" << endl;
	cout << "->\t\tXoay khung canh sang trai" << endl;
	cout << "<-\t\tXoay khung canh sang phai" << endl;
	cout << "up arrow\t\tTang chieu cao camera" << endl;
	cout << "down arrow\t\tGiam chieu cao camera" << endl;
	cout << "+\t\tTang khoang cach camera" << endl;
	cout << "-\t\tGiam khoang cach camera" << endl;
	cout << "V-v\t\tChuyen doi giua 2 che do nhin" << endl;
	cout << "W-w\t\tChuyen doi qua lai giua che do khung day va to mau" << endl;
	cout << "D-d\t\tBat/tat nguon sang thu hai" << endl;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //set the display mode
	glutInitWindowSize(screenWidth, screenHeight); //set window size
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("Computer Graphic Assignment - 2020017"); // open the screen window
	printMenu(); // sprint command line interface
	createObject(); // create the object
	myInit();
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecialKeyboard);
	glutMainLoop();
	return 0;
}

