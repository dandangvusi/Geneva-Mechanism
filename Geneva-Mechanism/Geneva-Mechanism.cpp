#include <math.h>
#include <iostream>
#include "supportClass.h"
#include "Mesh.h"

using namespace std;

#define PI				3.14159265358979323846
#define DRIVEN_WHEEL_E	0.3

// Tham so cho cua so hien thi
int		screenWidth = 900;
int		screenHeight = 900;

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
Point3 RW_O1{ 0.0, 0.0, 1.4 };
float RW_fShape2HigherHeight = 0.5;
float RW_fShape2LowerHeight = 0.25;
float RW_fShape2InRadius = 0.5;
float RW_fShape2OutRadius = 0.7;
int RW_iShape2OpenAngle = 60;
int RW_iShape2CentralLeanAngle = 0;
float RW_fCylinderHeight = 1.0;
float RW_fCylinderRadius = DRIVEN_WHEEL_E * sin((DW_iOpenAngle1 / 2) * ((2 * PI) / 360));
float RW_O1_O2_len = 0.6;
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
	if (bDrawWireFrame) {
		basePlate.DrawWireframe();
	}
	else {
		basePlate.DrawColor();
	}
}

void drawDrivenWheel() {
	if (bDrawWireFrame) {
		drivenWheel.DrawWireframe();
	}
	else {
		drivenWheel.DrawColor();
	}
}

void drawRotateWheel() {
	if (bDrawWireFrame) {
		rotateWheel.DrawWireframe();
	}
	else {
		rotateWheel.DrawColor();
	}
}

void drawObject() {
	drawBasePlate();
	drawDrivenWheel();
	drawRotateWheel();
}

void createObject() {
	basePlate.CreateBasePlate(BasePlate_Size, BasePlate_Height, BasePlate_BaseHeight);
	drivenWheel.CreateDrivenWheel(nSegment, DW_O1_O2_len, DW_fO1InRadius, DW_fO1OutRadius, DW_fO2Radius, DW_fHeight, DW_iOpenAngle1, DW_iOpenAngle2, DW_fBaseObjectHeight);
	rotateWheel.CreateRotateWheel(nSegment, RW_O1, RW_fShape2HigherHeight, RW_fShape2LowerHeight, RW_fShape2InRadius, RW_fShape2OutRadius, RW_iShape2OpenAngle, RW_iShape2CentralLeanAngle, RW_fCylinderHeight, RW_fCylinderRadius, RW_O1_O2_len, RW_fShape2BaseHeight, RW_fCylinderBaseHeight);
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
	// Draw object
	drawAxis();
	drawObject();

	glFlush();
	glutSwapBuffers();
}

void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
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
			break;
		case '2':
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
		/*rotor.rotateY += 1.5;
		if (rotor.rotateY > 360)
			rotor.rotateY -= 360;*/
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
	glutTimerFunc(50, Timer, 0);

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
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat light_model_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_model_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	GLfloat light_ambient0[] = { 0.0, 0.0, 0.0, 1.0 };
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
}

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//set the display mode
	glutInitWindowSize(screenWidth, screenHeight); //set window size
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("Computer Graphic Assignment - 2020017"); // open the screen window
	printMenu();
	createObject();
	myInit();
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecialKeyboard);
	glutMainLoop();
	return 0;
}

