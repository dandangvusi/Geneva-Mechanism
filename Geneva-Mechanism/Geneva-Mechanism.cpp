#include <math.h>
#include <iostream>
#include "supportClass.h"
#include "Mesh.h"

using namespace std;

#define PI				3.14159265358979323846
#define DRIVEN_WHEEL_E	0.3

int		screenWidth = 1800;
int		screenHeight = 900;

Mesh	tetrahedron;
Mesh	cube;
Mesh	cuboid;
Mesh	cylinder;
Mesh	cylinderWithHole;
Mesh	shape2;
Mesh	shape1;
Mesh	basePlate;
Mesh	drivenWheel;
Mesh	rotateWheel;

int		nChoice = 1;


// Tham so chung cua vat the
int nSegment = 20;
bool bDrawWireFrame = false;

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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5, 5, 2, 0, 0, 0, 0, 1, 0);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screenWidth / 2, screenHeight);

	drawAxis();

	glColor3f(0, 0, 0);
	if (nChoice == 1)
		tetrahedron.DrawWireframe();
	else if (nChoice == 2)
		cube.DrawWireframe();
	else if (nChoice == 3) {
		cuboid.DrawWireframe();
	}
	else if (nChoice == 4) {
		cylinder.DrawWireframe();
	}
	else if (nChoice == 5) {
		cylinderWithHole.DrawWireframe();
	}
	else if (nChoice == 6) {
		shape2.DrawWireframe();
	}
	else if (nChoice == 7) {
		shape1.DrawWireframe();
	}
	else if (nChoice == 8) {
		basePlate.DrawWireframe();
	}
	else if (nChoice == 9) {
		drivenWheel.DrawWireframe();
	}
	else if (nChoice == 10) {
		rotateWheel.DrawWireframe();
	}
	else if (nChoice == 11) {
		bDrawWireFrame = true;
		drawObject();
	}

	glViewport(screenWidth / 2, 0, screenWidth / 2, screenHeight);

	drawAxis();
	if (nChoice == 1)
		tetrahedron.DrawColor();
	else if (nChoice == 2)
		cube.DrawColor();
	else if (nChoice == 3) {
		cuboid.DrawColor();
	}
	else if (nChoice == 4) {
		cylinder.DrawColor();
	}
	else if (nChoice == 5) {
		cylinderWithHole.DrawColor();
	}
	else if (nChoice == 6) {
		shape2.DrawColor();
	}
	else if (nChoice == 7) {
		shape1.DrawColor();
	}
	else if (nChoice == 8) {
		basePlate.DrawColor();
	}
	else if (nChoice == 9) {
		drivenWheel.DrawColor();
	}
	else if (nChoice == 10) {
		rotateWheel.DrawColor();
	}
	else if (nChoice == 11) {
		bDrawWireFrame = false;
		drawObject();
	}
	glFlush();
	glutSwapBuffers();
}

void myInit()
{
	float	fHalfSize = 4;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-fHalfSize, fHalfSize, -fHalfSize, fHalfSize, -1000, 1000);
}

int main(int argc, char* argv[])
{
	cout << "1. Tetrahedron" << endl;
	cout << "2. Cube" << endl;
	cout << "3. Cuboid" << endl;
	cout << "4. Cylinder" << endl;
	cout << "5. Cylinder with hole" << endl;
	cout << "6. Shape 2" << endl;
	cout << "7. Shape 1" << endl;
	cout << "8. Base plate" << endl;
	cout << "9. Driven wheel" << endl;
	cout << "10. Rotate wheel" << endl;
	cout << "11. Entire object" << endl;
	cout << "Input the choice: " << endl;
	cin >> nChoice;

	glutInit(&argc, (char**)argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//set the display mode
	glutInitWindowSize(screenWidth, screenHeight); //set window size
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("Lab 2"); // open the screen window

	tetrahedron.CreateTetrahedron();
	cube.CreateCube(1);
	cuboid.CreateCuboid(2.0, 1.0, 1.0);
	cylinder.CreateCylinder(10, 5.0, 2.0);
	cylinderWithHole.CreateCylinderWithHole(10, 5.0, 2.0, 1.5);
	Point3 O1_1{ 0.86603, 0, 0.5 };
	shape2.CreateShape2(6, O1_1, 0.6, 1.0, 2.8, 3.5, 90);
	Point3 O1_2{ 1, 0, 1 };
	shape1.CreateShape1(30, 1.7, O1_2, 1.3, 3.0, 1.1, 1.0, 60, 100);
	createObject();
	myInit();
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	return 0;
}

