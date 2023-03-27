#include <math.h>
#include <iostream>
#include "supportClass.h"
#include "Mesh.h"

using namespace std;

#define PI								3.14159265358979323846
#define	COLORNUM						14
#define DRIVEN_WHEEL_E					0.3
#define ROTATE_WHEEL_ADJUST_FACTOR		1.6


float	ColorArr[COLORNUM][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, { 0.0,  0.0, 1.0},
								{1.0, 1.0,  0.0}, { 1.0, 0.0, 1.0},{ 0.0, 1.0, 1.0},
								 {0.3, 0.3, 0.3}, {0.5, 0.5, 0.5}, { 0.9,  0.9, 0.9},
								{1.0, 0.5,  0.5}, { 0.5, 1.0, 0.5},{ 0.5, 0.5, 1.0},
									{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0} };



void Mesh::CreateCylinder(int nSegment, float fHeight, float fRadius)
{
	numVerts = nSegment * 2 + 2;
	pt = new Point3[numVerts];

	int		i;
	int		idx;
	float	fAngle = 2 * PI / nSegment;
	float	x, y, z;

	pt[0].set(0, fHeight / 2, 0);
	for (i = 0; i < nSegment; i++)
	{
		x = fRadius * cos(fAngle * i);
		z = fRadius * sin(fAngle * i);
		y = fHeight / 2;
		pt[i + 1].set(x, y, z);

		y = -fHeight / 2;
		pt[i + 1 + nSegment].set(x, y, z);
	}
	pt[numVerts - 1].set(0, -fHeight / 2, 0);

	numFaces = nSegment * 3;
	face = new Face[numFaces];

	idx = 0;
	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 0;
		if (i < nSegment - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = i + 1;
		idx++;
	}

	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 1;
		if (i < nSegment - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + nSegment;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + nSegment;

		idx++;
	}

	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = numVerts - 1;
		if (i < nSegment - 1)
			face[idx].vert[2].vertIndex = i + 2 + nSegment;
		else
			face[idx].vert[2].vertIndex = 1 + nSegment;
		face[idx].vert[1].vertIndex = i + 1 + nSegment;
		idx++;
	}

}

void Mesh::CreateCylinderWithHole(int nSegment, float fHeight, float fORadius, float fIRadius) {
	this->numVerts = nSegment * 4;
	this->pt = new Point3[numVerts];
	int i, idx;
	float fAngle = 2 * PI / nSegment;
	float x1, y1, z1, x2, y2, z2;

	//draw vertices
	for (i = 0; i < nSegment; i++) {
		x1 = fORadius * cos(fAngle * i);
		z1 = fORadius * sin(fAngle * i);
		y1 = fHeight / 2;
		x2 = fIRadius * cos(fAngle * i);
		z2 = fIRadius * sin(fAngle * i);
		y2 = fHeight / 2;

		pt[i].set(x1, y1, z1);
		pt[i + 2 * nSegment].set(x2, y2, z2);

		y1 = y2 = -fHeight / 2;
		pt[i + nSegment].set(x1, y1, z1);
		pt[i + 3 * nSegment].set(x2, y2, z2);
	}

	this->numFaces = nSegment * 4;
	this->face = new Face[numFaces];

	//draw faces
	idx = 0;

	//outside faces
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i;
		if (i < nSegment - 1) {
			face[idx].vert[1].vertIndex = i + 1;
		}
		else {
			face[idx].vert[1].vertIndex = 0;
		}
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + nSegment;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + nSegment;
		idx++;
	}

	//inside faces
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 2 * nSegment + i;
		if (i < nSegment - 1) {
			face[idx].vert[1].vertIndex = 2 * nSegment + i + 1;
		}
		else {
			face[idx].vert[1].vertIndex = 2 * nSegment;
		}
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + nSegment;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + nSegment;
		idx++;
	}

	//top faces
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i;
		if (i < nSegment - 1) {
			face[idx].vert[1].vertIndex = i + 1;
		}
		else {
			face[idx].vert[1].vertIndex = 0;
		}
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + 2 * nSegment;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + 2 * nSegment;
		idx++;
	}

	//bottom faces
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + nSegment;
		if (i < nSegment - 1) {
			face[idx].vert[1].vertIndex = i + nSegment + 1;
		}
		else {
			face[idx].vert[1].vertIndex = nSegment;
		}
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + 2 * nSegment;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + 2 * nSegment;
		idx++;
	}
}

Point3 Mesh::findIntersection(Point3 p1, Point3 p2, Point3 p3, Point3 p4) {
	// Line p1-p2 represented as a1x + b1y = c1
	double a1 = p2.z - p1.z;
	double b1 = p1.x - p2.x;
	double c1 = a1 * p1.x + b1 * p1.z;
	// Line p2-p3 represented as a2x + b2y = c2
	double a2 = p4.z - p3.z;
	double b2 = p3.x - p4.x;
	double c2 = a2 * p3.x + b2 * p3.z;

	double determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		// The lines are parallel. This is simplified
		// by returning a pair of FLT_MAX
		return Point3(FLT_MAX, 0, FLT_MAX);
	}
	else
	{
		double x = (b2 * c1 - b1 * c2) / determinant;
		double z = (a1 * c2 - a2 * c1) / determinant;
		return Point3(x, 0, z);
	}
}

void Mesh::CreateShape1(int nSegment, float O1_O2_len, Point3 O1, float fO1InRadius, float fO1OutRadius, float fO2Radius, float fHeight, int iOpenAngle1, int iOpenAngle2) {
	float fCentralLeanAngle = atan2(O1.z, O1.x);
	float fOpenAngle1 = (iOpenAngle1 * 2.0 * PI) / 360;
	float fOpenAngle2 = (iOpenAngle2 * 2.0 * PI) / 360;
	float fSegmentAngle1 = fOpenAngle1 / nSegment;
	float fSegmentAngle2 = fOpenAngle2 / nSegment;
	float fLeanAngle1 = fCentralLeanAngle - fOpenAngle1 / 2;
	float fLeanAngle2 = fCentralLeanAngle - fOpenAngle2 / 2;
	float O_O1_len = sqrt(pow(O1.x, 2) + pow(O1.z, 2));
	float O_O2_len = sqrt(pow(O1.x, 2) + pow(O1.z, 2)) + O1_O2_len;
	float O2_x = O1.x * (O_O2_len / O_O1_len);
	float O2_z = O1.z * (O_O2_len / O_O1_len);
	Point3 O2 = Point3{ O2_x, 0, O2_z };
	this->numVerts = (nSegment + 1) * 6 + 6;
	this->pt = new Point3[numVerts];
	int i, idx;
	float x1, y1, z1, x2, y2, z2, x3, y3, z3;
	// draw vertices
	for (i = 0; i <= nSegment; i++) {
		x1 = O1.x + fO1OutRadius * cos(fLeanAngle1 + fSegmentAngle1 * i);
		z1 = O1.z + fO1OutRadius * sin(fLeanAngle1 + fSegmentAngle1 * i);
		y1 = fHeight;
		x2 = O1.x + fO1InRadius * cos(fLeanAngle1 + fSegmentAngle1 * i);
		z2 = O1.z + fO1InRadius * sin(fLeanAngle1 + fSegmentAngle1 * i);
		y2 = fHeight / 2;
		x3 = O2.x + fO2Radius * cos(fLeanAngle2 + fSegmentAngle2 * i);
		z3 = O2.z + fO2Radius * sin(fLeanAngle2 + fSegmentAngle2 * i);
		y3 = fHeight;
		pt[i + 0 * (nSegment + 1)].set(x1, y1, z1);
		pt[i + 2 * (nSegment + 1)].set(x2, y2, z2);
		pt[i + 4 * (nSegment + 1)].set(x3, y3, z3);
		y1 = 0;
		y2 = 0;
		y3 = fHeight / 2;
		pt[i + 1 * (nSegment + 1)].set(x1, y1, z1);
		pt[i + 3 * (nSegment + 1)].set(x2, y2, z2);
		pt[i + 5 * (nSegment + 1)].set(x3, y3, z3);
	}
	Point3 p1 = findIntersection(pt[0 + 3 * (nSegment + 1)], pt[0 + 1 * (nSegment + 1)], pt[0 + 4 * (nSegment + 1)], pt[nSegment + 4 * (nSegment + 1)]);
	pt[6 * (nSegment + 1)].set(p1.x, fHeight, p1.z);
	pt[6 * (nSegment + 1) + 1].set(p1.x, fHeight / 2, p1.z);
	pt[6 * (nSegment + 1) + 2].set(p1.x, 0, p1.z);
	Point3 p2 = findIntersection(pt[nSegment + 1 * (nSegment + 1)], pt[nSegment + 3 * (nSegment + 1)], pt[0 + 4 * (nSegment + 1)], pt[nSegment + 4 * (nSegment + 1)]);
	pt[6 * (nSegment + 1) + 3].set(p2.x, fHeight, p2.z);
	pt[6 * (nSegment + 1) + 4].set(p2.x, fHeight / 2, p2.z);
	pt[6 * (nSegment + 1) + 5].set(p2.x, 0, p2.z);
	//draw faces
	this->numFaces = nSegment * 6 + 10;
	this->face = new Face[numFaces];
	idx = 0;
	//far vertical rectangle faces
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 1 * (nSegment + 1);
		face[idx].vert[1].vertIndex = i + 0 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 0 * (nSegment + 1) + 1;
		face[idx].vert[3].vertIndex = i + 1 * (nSegment + 1) + 1;
		idx++;
	}
	//middle vertical rectangle faces
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 5 * (nSegment + 1);
		face[idx].vert[1].vertIndex = i + 4 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 4 * (nSegment + 1) + 1;
		face[idx].vert[3].vertIndex = i + 5 * (nSegment + 1) + 1;
		idx++;
	}
	//near vertical rectangle faces
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 3 * (nSegment + 1);
		face[idx].vert[1].vertIndex = i + 2 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 2 * (nSegment + 1) + 1;
		face[idx].vert[3].vertIndex = i + 3 * (nSegment + 1) + 1;
		idx++;
	}
	//top horizontal rectangle faces
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 4 * (nSegment + 1);
		face[idx].vert[1].vertIndex = i + 0 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 0 * (nSegment + 1) + 1;
		face[idx].vert[3].vertIndex = i + 4 * (nSegment + 1) + 1;
		idx++;
	}
	//bottom horizontal rectangle faces
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 3 * (nSegment + 1);
		face[idx].vert[1].vertIndex = i + 1 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 1 * (nSegment + 1) + 1;
		face[idx].vert[3].vertIndex = i + 3 * (nSegment + 1) + 1;
		idx++;
	}
	//middle horizontal rectangle faces
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 2 * (nSegment + 1);
		face[idx].vert[1].vertIndex = i + 5 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 5 * (nSegment + 1) + 1;
		face[idx].vert[3].vertIndex = i + 2 * (nSegment + 1) + 1;
		idx++;
	}
	//rear faces
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0 + 3 * (nSegment + 1);
	face[idx].vert[1].vertIndex = 0 + 2 * (nSegment + 1);
	face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + 1;
	face[idx].vert[3].vertIndex = 6 * (nSegment + 1) + 2;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 6 * (nSegment + 1) + 2;
	face[idx].vert[1].vertIndex = 6 * (nSegment + 1);
	face[idx].vert[2].vertIndex = 0 + 0 * (nSegment + 1);
	face[idx].vert[3].vertIndex = 0 + 1 * (nSegment + 1);
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0 + 5 * (nSegment + 1);
	face[idx].vert[1].vertIndex = 0 + 4 * (nSegment + 1);
	face[idx].vert[2].vertIndex = 6 * (nSegment + 1);
	face[idx].vert[3].vertIndex = 6 * (nSegment + 1) + 1;
	idx++;
	face[idx].nVerts = 3;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0 + 4 * (nSegment + 1);
	face[idx].vert[1].vertIndex = 0 + 0 * (nSegment + 1);
	face[idx].vert[2].vertIndex = 6 * (nSegment + 1);
	idx++;
	face[idx].nVerts = 3;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0 + 2 * (nSegment + 1);
	face[idx].vert[1].vertIndex = 0 + 5 * (nSegment + 1);
	face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + 1;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = nSegment + 3 * (nSegment + 1);
	face[idx].vert[1].vertIndex = nSegment + 2 * (nSegment + 1);
	face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + 4;
	face[idx].vert[3].vertIndex = 6 * (nSegment + 1) + 5;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 6 * (nSegment + 1) + 5;
	face[idx].vert[1].vertIndex = 6 * (nSegment + 1) + 3;
	face[idx].vert[2].vertIndex = nSegment + 0 * (nSegment + 1);
	face[idx].vert[3].vertIndex = nSegment + 1 * (nSegment + 1);
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = nSegment + 5 * (nSegment + 1);
	face[idx].vert[1].vertIndex = nSegment + 4 * (nSegment + 1);
	face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + 3;
	face[idx].vert[3].vertIndex = 6 * (nSegment + 1) + 4;
	idx++;
	face[idx].nVerts = 3;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = nSegment + 4 * (nSegment + 1);
	face[idx].vert[1].vertIndex = nSegment + 0 * (nSegment + 1);
	face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + 3;
	idx++;
	face[idx].nVerts = 3;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = nSegment + 2 * (nSegment + 1);
	face[idx].vert[1].vertIndex = nSegment + 5 * (nSegment + 1);
	face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + 4;
	idx++;
}

void Mesh::CreateDrivenWheel(int nSegment, float O1_O2_len, float fO1InRadius, float fO1OutRadius, float fO2Radius, float fHeight, int iOpenAngle1, int iOpenAngle2, float fBaseObjectHeight) {
	// Tam cua cung tron thu nhat
	Point3 O1;
	// Cac goc mo cua 2 cung tron
	float fOpenAngle1 = (iOpenAngle1 * 2.0 * PI) / 360;
	float fOpenAngle2 = (iOpenAngle2 * 2.0 * PI) / 360;
	float fSegmentAngle1 = fOpenAngle1 / nSegment;
	float fSegmentAngle2 = fOpenAngle2 / nSegment;
	// so dinh cua vat the
	int shape1_obj_num = 6;
	this->numVerts = ((nSegment + 1) * 6 + 6) * shape1_obj_num;
	// so mat cua vat the
	this->numFaces = (nSegment * 6 + 10) * shape1_obj_num;
	this->face = new Face[numFaces];
	// mang chua cac dinh cua vat the
	this->pt = new Point3[numVerts];
	for (int j = 0; j < 6; j++) {
		int deg = j * 60;
		O1.set(DRIVEN_WHEEL_E * sin(deg * (2 * PI / 360)), fBaseObjectHeight, DRIVEN_WHEEL_E * cos(deg * (2 * PI / 360)));
		float fCentralLeanAngle = atan2(O1.z, O1.x);
		float fLeanAngle1 = fCentralLeanAngle - fOpenAngle1 / 2;
		float fLeanAngle2 = fCentralLeanAngle - fOpenAngle2 / 2;
		float O_O1_len = sqrt(pow(O1.x, 2) + pow(O1.z, 2));
		float O_O2_len = sqrt(pow(O1.x, 2) + pow(O1.z, 2)) + O1_O2_len;
		float O2_x = O1.x * (O_O2_len / O_O1_len);
		float O2_z = O1.z * (O_O2_len / O_O1_len);
		Point3 O2 = Point3{ O2_x, 0, O2_z };
		int i, idx;
		float x1, y1, z1, x2, y2, z2, x3, y3, z3;

		// buoc nhay cua cac dinh trong moi shape1 trong object
		int offset = ((nSegment + 1) * 6 + 6) * j;

		// xac dinh toa do cac dinh
		for (i = 0; i <= nSegment; i++) {
			// cac diem ben ngoai cua cung tron O1
			x1 = O1.x + fO1OutRadius * cos(fLeanAngle1 + fSegmentAngle1 * i);
			z1 = O1.z + fO1OutRadius * sin(fLeanAngle1 + fSegmentAngle1 * i);
			y1 = fHeight + fBaseObjectHeight;
			// cac diem ben trong cua cung tron O1
			x2 = O1.x + fO1InRadius * cos(fLeanAngle1 + fSegmentAngle1 * i);
			z2 = O1.z + fO1InRadius * sin(fLeanAngle1 + fSegmentAngle1 * i);
			y2 = fHeight / 2 + fBaseObjectHeight;
			// cac diem nam tren cung tron O2
			x3 = O2.x + fO2Radius * cos(fLeanAngle2 + fSegmentAngle2 * i);
			z3 = O2.z + fO2Radius * sin(fLeanAngle2 + fSegmentAngle2 * i);
			y3 = fHeight + fBaseObjectHeight;
			pt[i + 0 * (nSegment + 1) + offset].set(x1, y1, z1);
			pt[i + 2 * (nSegment + 1) + offset].set(x2, y2, z2);
			pt[i + 4 * (nSegment + 1) + offset].set(x3, y3, z3);
			y1 = fBaseObjectHeight;
			y2 = fBaseObjectHeight;
			y3 = fHeight / 2 + fBaseObjectHeight;
			pt[i + 1 * (nSegment + 1) + offset].set(x1, y1, z1);
			pt[i + 3 * (nSegment + 1) + offset].set(x2, y2, z2);
			pt[i + 5 * (nSegment + 1) + offset].set(x3, y3, z3);
		}
		// cac diem dac biet
		Point3 p1 = findIntersection(pt[0 + 3 * (nSegment + 1) + offset], pt[0 + 1 * (nSegment + 1) + offset], pt[0 + 4 * (nSegment + 1) + offset], pt[nSegment + 4 * (nSegment + 1) + offset]);
		pt[6 * (nSegment + 1) + offset].set(p1.x, fHeight + fBaseObjectHeight, p1.z);
		pt[6 * (nSegment + 1) + 1 + offset].set(p1.x, fHeight / 2 + fBaseObjectHeight, p1.z);
		pt[6 * (nSegment + 1) + 2 + offset].set(p1.x, fBaseObjectHeight, p1.z);
		Point3 p2 = findIntersection(pt[nSegment + 1 * (nSegment + 1) + offset], pt[nSegment + 3 * (nSegment + 1) + offset], pt[0 + 4 * (nSegment + 1) + offset], pt[nSegment + 4 * (nSegment + 1) + offset]);
		pt[6 * (nSegment + 1) + 3 + offset].set(p2.x, fHeight + fBaseObjectHeight, p2.z);
		pt[6 * (nSegment + 1) + 4 + offset].set(p2.x, fHeight / 2 + fBaseObjectHeight, p2.z);
		pt[6 * (nSegment + 1) + 5 + offset].set(p2.x, fBaseObjectHeight, p2.z);

		// xac dinh cac mat cua vat the
		// buoc nhay cua cac mat trong moi shape1 trong object
		int face_offset = (nSegment * 6 + 10) * j;
		idx = face_offset;
		//far vertical rectangle faces
		for (i = 0; i < nSegment; i++) {
			face[idx].nVerts = 4;
			face[idx].vert = new VertexID[face[idx].nVerts];
			face[idx].vert[0].vertIndex = i + 1 * (nSegment + 1) + offset;
			face[idx].vert[1].vertIndex = i + 0 * (nSegment + 1) + offset;
			face[idx].vert[2].vertIndex = i + 0 * (nSegment + 1) + 1 + offset;
			face[idx].vert[3].vertIndex = i + 1 * (nSegment + 1) + 1 + offset;
			idx++;
		}
		//middle vertical rectangle faces
		for (i = 0; i < nSegment; i++) {
			face[idx].nVerts = 4;
			face[idx].vert = new VertexID[face[idx].nVerts];
			face[idx].vert[0].vertIndex = i + 5 * (nSegment + 1) + offset;
			face[idx].vert[1].vertIndex = i + 4 * (nSegment + 1) + offset;
			face[idx].vert[2].vertIndex = i + 4 * (nSegment + 1) + 1 + offset;
			face[idx].vert[3].vertIndex = i + 5 * (nSegment + 1) + 1 + offset;
			idx++;
		}
		//near vertical rectangle faces
		for (i = 0; i < nSegment; i++) {
			face[idx].nVerts = 4;
			face[idx].vert = new VertexID[face[idx].nVerts];
			face[idx].vert[0].vertIndex = i + 3 * (nSegment + 1) + offset;
			face[idx].vert[1].vertIndex = i + 2 * (nSegment + 1) + offset;
			face[idx].vert[2].vertIndex = i + 2 * (nSegment + 1) + 1 + offset;
			face[idx].vert[3].vertIndex = i + 3 * (nSegment + 1) + 1 + offset;
			idx++;
		}
		//top horizontal rectangle faces
		for (i = 0; i < nSegment; i++) {
			face[idx].nVerts = 4;
			face[idx].vert = new VertexID[face[idx].nVerts];
			face[idx].vert[0].vertIndex = i + 4 * (nSegment + 1) + offset;
			face[idx].vert[1].vertIndex = i + 0 * (nSegment + 1) + offset;
			face[idx].vert[2].vertIndex = i + 0 * (nSegment + 1) + 1 + offset;
			face[idx].vert[3].vertIndex = i + 4 * (nSegment + 1) + 1 + offset;
			idx++;
		}
		//bottom horizontal rectangle faces
		for (i = 0; i < nSegment; i++) {
			face[idx].nVerts = 4;
			face[idx].vert = new VertexID[face[idx].nVerts];
			face[idx].vert[0].vertIndex = i + 3 * (nSegment + 1) + offset;
			face[idx].vert[1].vertIndex = i + 1 * (nSegment + 1) + offset;
			face[idx].vert[2].vertIndex = i + 1 * (nSegment + 1) + 1 + offset;
			face[idx].vert[3].vertIndex = i + 3 * (nSegment + 1) + 1 + offset;
			idx++;
		}
		//middle horizontal rectangle faces
		for (i = 0; i < nSegment; i++) {
			face[idx].nVerts = 4;
			face[idx].vert = new VertexID[face[idx].nVerts];
			face[idx].vert[0].vertIndex = i + 2 * (nSegment + 1) + offset;
			face[idx].vert[1].vertIndex = i + 5 * (nSegment + 1) + offset;
			face[idx].vert[2].vertIndex = i + 5 * (nSegment + 1) + 1 + offset;
			face[idx].vert[3].vertIndex = i + 2 * (nSegment + 1) + 1 + offset;
			idx++;
		}
		//rear faces
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 0 + 3 * (nSegment + 1) + offset;
		face[idx].vert[1].vertIndex = 0 + 2 * (nSegment + 1) + offset;
		face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + 1 + offset;
		face[idx].vert[3].vertIndex = 6 * (nSegment + 1) + 2 + offset;
		idx++;
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 6 * (nSegment + 1) + 2 + offset;
		face[idx].vert[1].vertIndex = 6 * (nSegment + 1) + offset;
		face[idx].vert[2].vertIndex = 0 + 0 * (nSegment + 1) + offset;
		face[idx].vert[3].vertIndex = 0 + 1 * (nSegment + 1) + offset;
		idx++;
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 0 + 5 * (nSegment + 1) + offset;
		face[idx].vert[1].vertIndex = 0 + 4 * (nSegment + 1) + offset;
		face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + offset;
		face[idx].vert[3].vertIndex = 6 * (nSegment + 1) + 1 + offset;
		idx++;
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 0 + 4 * (nSegment + 1) + offset;
		face[idx].vert[1].vertIndex = 0 + 0 * (nSegment + 1) + offset;
		face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + offset;
		idx++;
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 0 + 2 * (nSegment + 1) + offset;
		face[idx].vert[1].vertIndex = 0 + 5 * (nSegment + 1) + offset;
		face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + 1 + offset;
		idx++;

		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = nSegment + 3 * (nSegment + 1) + offset;
		face[idx].vert[1].vertIndex = nSegment + 2 * (nSegment + 1) + offset;
		face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + 4 + offset;
		face[idx].vert[3].vertIndex = 6 * (nSegment + 1) + 5 + offset;
		idx++;
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 6 * (nSegment + 1) + 5 + offset;
		face[idx].vert[1].vertIndex = 6 * (nSegment + 1) + 3 + offset;
		face[idx].vert[2].vertIndex = nSegment + 0 * (nSegment + 1) + offset;
		face[idx].vert[3].vertIndex = nSegment + 1 * (nSegment + 1) + offset;
		idx++;
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = nSegment + 5 * (nSegment + 1) + offset;
		face[idx].vert[1].vertIndex = nSegment + 4 * (nSegment + 1) + offset;
		face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + 3 + offset;
		face[idx].vert[3].vertIndex = 6 * (nSegment + 1) + 4 + offset;
		idx++;
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = nSegment + 4 * (nSegment + 1) + offset;
		face[idx].vert[1].vertIndex = nSegment + 0 * (nSegment + 1) + offset;
		face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + 3 + offset;
		idx++;
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = nSegment + 2 * (nSegment + 1) + offset;
		face[idx].vert[1].vertIndex = nSegment + 5 * (nSegment + 1) + offset;
		face[idx].vert[2].vertIndex = 6 * (nSegment + 1) + 4 + offset;
		idx++;
	}
}

void Mesh::CreateRotateWheel(int nSegment, Point3 O1, float fShape2HigherHeight, float fShape2LowerHeight, float fShape2InRadius, float fShape2OutRadius, int iShape2OpenAngle, int iShape2CentralLeanAngle, float fCylinderHeight, float fCylinderRadius, float O1_O2_len, float fShape2BaseHeight, float fCylinderBaseHeight) {
	// so dinh cua shape 2
	int shape2NumVerts = (nSegment + 1) * 4 + 2;;
	// so dinh cua cylinder
	int cylinderNumVerts = nSegment * 2 + 2;
	// so dinh cua hinh hop
	int cubeNumVerts = 8;
	// so dinh cua vat the
	this->numVerts = shape2NumVerts + cylinderNumVerts + cubeNumVerts;
	this->pt = new Point3[numVerts];
	// so mat cua shape 2
	int shape2NumFaces = 5 * nSegment + 4;
	// so mat cua cylinder
	int cylinderNumFaces = nSegment * 3;
	// so mat cua hinh hop
	int cubeNumFaces = 6;
	// so mat cua vat the
	this->numFaces = shape2NumFaces + cylinderNumFaces + cubeNumFaces;
	this->face = new Face[numFaces];

	// cac thong so goc cua shape 2
	float fShape2CentralLeanAngle = (iShape2CentralLeanAngle * 2.0 * PI) / 360;
	float fShape2OpenAngle = (iShape2OpenAngle * 2.0 * PI) / 360;
	float fShape2SegmentAngle = fShape2OpenAngle / nSegment;
	float fShape2LeanAngle = fShape2CentralLeanAngle - fShape2OpenAngle / 2;

	// cac thong so cua cylinder
	Point3 O2{ O1.x + O1_O2_len * (float)sin(fShape2CentralLeanAngle + PI), 0, O1.z + O1_O2_len * (float)cos(fShape2CentralLeanAngle + PI) };

	/*
	Xac dinh cac dinh cua vat the
	*/

	int i, idx;
	float x1, y1, z1, x2, y2, z2;

	// xac dinh cac dinh cua shape 2
	for (i = 0; i <= nSegment; i++) {
		// cac diem nam tren cung tron ben ngoai
		x1 = O1.x + fShape2OutRadius * sin(fShape2LeanAngle + fShape2SegmentAngle * i);
		z1 = O1.z + fShape2OutRadius * cos(fShape2LeanAngle + fShape2SegmentAngle * i);
		y1 = fShape2HigherHeight + fShape2BaseHeight;
		// cac diem nam tren cung tron ben trong
		x2 = O1.x + fShape2InRadius * sin(fShape2LeanAngle + fShape2SegmentAngle * i);
		z2 = O1.z + fShape2InRadius * cos(fShape2LeanAngle + fShape2SegmentAngle * i);
		y2 = fShape2HigherHeight + fShape2BaseHeight;
		pt[i].set(x1, y1, z1);
		pt[i + 2 * (nSegment + 1)].set(x2, y2, z2);
		y1 = fShape2BaseHeight;
		y2 = fShape2LowerHeight + fShape2BaseHeight;
		pt[i + 1 * (nSegment + 1)].set(x1, y1, z1);
		pt[i + 3 * (nSegment + 1)].set(x2, y2, z2);
	}
	pt[shape2NumVerts - 2].set(O1.x, fShape2LowerHeight + fShape2BaseHeight, O1.z);
	pt[shape2NumVerts - 1].set(O1.x, fShape2BaseHeight, O1.z);

	// xac dinh cac dinh cua cylinder
	float	fCylinderSegmentAngle = 2 * PI / nSegment;
	float	x, y, z;

	pt[shape2NumVerts].set(O2.x, fCylinderHeight + fCylinderBaseHeight, O2.z);
	for (i = 0; i < nSegment; i++)
	{
		x = O2.x + fCylinderRadius * sin(fCylinderSegmentAngle * i);
		z = O2.z + fCylinderRadius * cos(fCylinderSegmentAngle * i);
		y = O2.y + fCylinderHeight + fCylinderBaseHeight;
		pt[i + 1 + shape2NumVerts].set(x, y, z);

		y = fCylinderBaseHeight;
		pt[i + 1 + nSegment + shape2NumVerts].set(x, y, z);
	}
	pt[shape2NumVerts + cylinderNumVerts - 1].set(O2.x, fCylinderBaseHeight, O2.z);

	// xac dinh cac dinh cua hinh hop
	Point3 p1{ float(O2.x + fCylinderRadius * sin(fShape2CentralLeanAngle + 3 * PI / 2)), fShape2BaseHeight, float(O2.z + fCylinderRadius * cos(fShape2CentralLeanAngle + 3 * PI / 2)) };
	Point3 p4{ float(O2.x + fCylinderRadius * sin(fShape2CentralLeanAngle + PI / 2)), fShape2BaseHeight, float(O2.z + fCylinderRadius * cos(fShape2CentralLeanAngle + PI / 2)) };
	Point3 p2{ float(p1.x + (O1_O2_len * ROTATE_WHEEL_ADJUST_FACTOR) * sin(fShape2CentralLeanAngle)), fShape2BaseHeight, float(p1.z + (O1_O2_len * ROTATE_WHEEL_ADJUST_FACTOR)*cos(fShape2CentralLeanAngle)) };
	Point3 p3{ float(p4.x + (O1_O2_len * ROTATE_WHEEL_ADJUST_FACTOR) * sin(fShape2CentralLeanAngle)), fShape2BaseHeight, float(p4.z + (O1_O2_len * ROTATE_WHEEL_ADJUST_FACTOR)*cos(fShape2CentralLeanAngle)) };
	pt[shape2NumVerts + cylinderNumVerts + 0].set(p1.x, p1.y, p1.z);
	pt[shape2NumVerts + cylinderNumVerts + 1].set(p2.x, p2.y, p2.z);
	pt[shape2NumVerts + cylinderNumVerts + 2].set(p3.x, p3.y, p3.z);
	pt[shape2NumVerts + cylinderNumVerts + 3].set(p4.x, p4.y, p4.z);
	pt[shape2NumVerts + cylinderNumVerts + 4].set(p1.x, fShape2BaseHeight + fShape2LowerHeight, p1.z);
	pt[shape2NumVerts + cylinderNumVerts + 5].set(p2.x, fShape2BaseHeight + fShape2LowerHeight, p2.z);
	pt[shape2NumVerts + cylinderNumVerts + 6].set(p3.x, fShape2BaseHeight + fShape2LowerHeight, p3.z);
	pt[shape2NumVerts + cylinderNumVerts + 7].set(p4.x, fShape2BaseHeight + fShape2LowerHeight, p4.z);

	/*
	Xac dinh cac mat cua vat the
	*/

	// cac mat thuoc shape2
	idx = 0;
	//upper triangle
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = shape2NumVerts - 2;
		face[idx].vert[1].vertIndex = i + 3 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 3 * (nSegment + 1) + 1;
		idx++;
	}
	//near rectangle
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 3 * (nSegment + 1);
		face[idx].vert[1].vertIndex = i + 2 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 2 * (nSegment + 1) + 1;
		face[idx].vert[3].vertIndex = i + 3 * (nSegment + 1) + 1;
		idx++;
	}
	//top rectangle
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 2 * (nSegment + 1);
		face[idx].vert[1].vertIndex = i + 0 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 0 * (nSegment + 1) + 1;
		face[idx].vert[3].vertIndex = i + 2 * (nSegment + 1) + 1;
		idx++;
	}
	//far rectangle
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 1 * (nSegment + 1);
		face[idx].vert[1].vertIndex = i + 0 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 0 * (nSegment + 1) + 1;
		face[idx].vert[3].vertIndex = i + 1 * (nSegment + 1) + 1;
		idx++;
	}
	//lower triangle
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = shape2NumVerts - 1;
		face[idx].vert[1].vertIndex = i + 1 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 1 * (nSegment + 1) + 1;
		idx++;
	}
	//side faces
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = shape2NumVerts - 1;
	face[idx].vert[1].vertIndex = shape2NumVerts - 2;
	face[idx].vert[2].vertIndex = 3 * (nSegment + 1);
	face[idx].vert[3].vertIndex = 1 * (nSegment + 1);
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 3 * (nSegment + 1);
	face[idx].vert[1].vertIndex = 2 * (nSegment + 1);
	face[idx].vert[2].vertIndex = 0 * (nSegment + 1);
	face[idx].vert[3].vertIndex = 1 * (nSegment + 1);
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = shape2NumVerts - 1;
	face[idx].vert[1].vertIndex = shape2NumVerts - 2;
	face[idx].vert[2].vertIndex = 4 * (nSegment + 1) - 1;
	face[idx].vert[3].vertIndex = 2 * (nSegment + 1) - 1;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 4 * (nSegment + 1) - 1;
	face[idx].vert[1].vertIndex = 3 * (nSegment + 1) - 1;
	face[idx].vert[2].vertIndex = 1 * (nSegment + 1) - 1;
	face[idx].vert[3].vertIndex = 2 * (nSegment + 1) - 1;
	idx++;

	// cac mat thuoc cylinder
	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = shape2NumVerts + 0;
		if (i < nSegment - 1)
			face[idx].vert[1].vertIndex = shape2NumVerts + i + 2;
		else
			face[idx].vert[1].vertIndex = shape2NumVerts + 1;
		face[idx].vert[2].vertIndex = shape2NumVerts + i + 1;
		idx++;
	}

	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = shape2NumVerts + i + 1;
		if (i < nSegment - 1)
			face[idx].vert[1].vertIndex = shape2NumVerts + i + 2;
		else
			face[idx].vert[1].vertIndex = shape2NumVerts + 1;
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + nSegment;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + nSegment;

		idx++;
	}

	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = shape2NumVerts + cylinderNumVerts - 1;
		if (i < nSegment - 1)
			face[idx].vert[2].vertIndex = shape2NumVerts + i + 2 + nSegment;
		else
			face[idx].vert[2].vertIndex = shape2NumVerts + 1 + nSegment;
		face[idx].vert[1].vertIndex = shape2NumVerts + i + 1 + nSegment;
		idx++;
	}

	// cac mat thuoc hinh lap phuong
	//Left face
	face[shape2NumFaces + cylinderNumFaces + 0].nVerts = 4;
	face[shape2NumFaces + cylinderNumFaces + 0].vert = new VertexID[face[shape2NumFaces + cylinderNumFaces + 0].nVerts];
	face[shape2NumFaces + cylinderNumFaces + 0].vert[0].vertIndex = shape2NumVerts + cylinderNumVerts + 0;
	face[shape2NumFaces + cylinderNumFaces + 0].vert[1].vertIndex = shape2NumVerts + cylinderNumVerts + 4;
	face[shape2NumFaces + cylinderNumFaces + 0].vert[2].vertIndex = shape2NumVerts + cylinderNumVerts + 7;
	face[shape2NumFaces + cylinderNumFaces + 0].vert[3].vertIndex = shape2NumVerts + cylinderNumVerts + 3;
	for (int i = 0; i < face[shape2NumFaces + cylinderNumFaces + 0].nVerts; i++) {
		face[shape2NumFaces + cylinderNumFaces + 0].vert[i].colorIndex = 0;
	}

	//Bottom face
	face[shape2NumFaces + cylinderNumFaces + 1].nVerts = 4;
	face[shape2NumFaces + cylinderNumFaces + 1].vert = new VertexID[face[shape2NumFaces + cylinderNumFaces + 1].nVerts];
	face[shape2NumFaces + cylinderNumFaces + 1].vert[0].vertIndex = shape2NumVerts + cylinderNumVerts + 0;
	face[shape2NumFaces + cylinderNumFaces + 1].vert[1].vertIndex = shape2NumVerts + cylinderNumVerts + 1;
	face[shape2NumFaces + cylinderNumFaces + 1].vert[2].vertIndex = shape2NumVerts + cylinderNumVerts + 2;
	face[shape2NumFaces + cylinderNumFaces + 1].vert[3].vertIndex = shape2NumVerts + cylinderNumVerts + 3;
	for (int i = 0; i < face[shape2NumFaces + cylinderNumFaces + 1].nVerts; i++) {
		face[shape2NumFaces + cylinderNumFaces + 1].vert[i].colorIndex = 1;
	}

	//Right face
	face[shape2NumFaces + cylinderNumFaces + 2].nVerts = 4;
	face[shape2NumFaces + cylinderNumFaces + 2].vert = new VertexID[face[shape2NumFaces + cylinderNumFaces + 2].nVerts];
	face[shape2NumFaces + cylinderNumFaces + 2].vert[0].vertIndex = shape2NumVerts + cylinderNumVerts + 1;
	face[shape2NumFaces + cylinderNumFaces + 2].vert[1].vertIndex = shape2NumVerts + cylinderNumVerts + 5;
	face[shape2NumFaces + cylinderNumFaces + 2].vert[2].vertIndex = shape2NumVerts + cylinderNumVerts + 6;
	face[shape2NumFaces + cylinderNumFaces + 2].vert[3].vertIndex = shape2NumVerts + cylinderNumVerts + 2;
	for (int i = 0; i < face[shape2NumFaces + cylinderNumFaces + 2].nVerts; i++) {
		face[shape2NumFaces + cylinderNumFaces + 2].vert[i].colorIndex = 2;
	}

	//Top face
	face[shape2NumFaces + cylinderNumFaces + 3].nVerts = 4;
	face[shape2NumFaces + cylinderNumFaces + 3].vert = new VertexID[face[shape2NumFaces + cylinderNumFaces + 3].nVerts];
	face[shape2NumFaces + cylinderNumFaces + 3].vert[0].vertIndex = shape2NumVerts + cylinderNumVerts + 4;
	face[shape2NumFaces + cylinderNumFaces + 3].vert[1].vertIndex = shape2NumVerts + cylinderNumVerts + 5;
	face[shape2NumFaces + cylinderNumFaces + 3].vert[2].vertIndex = shape2NumVerts + cylinderNumVerts + 6;
	face[shape2NumFaces + cylinderNumFaces + 3].vert[3].vertIndex = shape2NumVerts + cylinderNumVerts + 7;
	for (int i = 0; i < face[shape2NumFaces + cylinderNumFaces + 3].nVerts; i++) {
		face[shape2NumFaces + cylinderNumFaces + 3].vert[i].colorIndex = 3;
	}

	//Near face
	face[shape2NumFaces + cylinderNumFaces + 4].nVerts = 4;
	face[shape2NumFaces + cylinderNumFaces + 4].vert = new VertexID[face[shape2NumFaces + cylinderNumFaces + 4].nVerts];
	face[shape2NumFaces + cylinderNumFaces + 4].vert[0].vertIndex = shape2NumVerts + cylinderNumVerts + 1;
	face[shape2NumFaces + cylinderNumFaces + 4].vert[1].vertIndex = shape2NumVerts + cylinderNumVerts + 5;
	face[shape2NumFaces + cylinderNumFaces + 4].vert[2].vertIndex = shape2NumVerts + cylinderNumVerts + 4;
	face[shape2NumFaces + cylinderNumFaces + 4].vert[3].vertIndex = shape2NumVerts + cylinderNumVerts + 0;
	for (int i = 0; i < face[shape2NumFaces + cylinderNumFaces + 4].nVerts; i++) {
		face[shape2NumFaces + cylinderNumFaces + 4].vert[i].colorIndex = 4;
	}

	//Far face
	face[shape2NumFaces + cylinderNumFaces + 5].nVerts = 4;
	face[shape2NumFaces + cylinderNumFaces + 5].vert = new VertexID[face[shape2NumFaces + cylinderNumFaces + 5].nVerts];
	face[shape2NumFaces + cylinderNumFaces + 5].vert[0].vertIndex = shape2NumVerts + cylinderNumVerts + 2;
	face[shape2NumFaces + cylinderNumFaces + 5].vert[1].vertIndex = shape2NumVerts + cylinderNumVerts + 6;
	face[shape2NumFaces + cylinderNumFaces + 5].vert[2].vertIndex = shape2NumVerts + cylinderNumVerts + 7;
	face[shape2NumFaces + cylinderNumFaces + 5].vert[3].vertIndex = shape2NumVerts + cylinderNumVerts + 3;
	for (int i = 0; i < face[shape2NumFaces + cylinderNumFaces + 5].nVerts; i++) {
		face[shape2NumFaces + cylinderNumFaces + 5].vert[i].colorIndex = 5;
	}
}

void Mesh::CreateShape2(int nSegment, Point3 O1, float fLowerHeight, float fHigherHeight, float fInRadius, float fOutRadius, int iOpenAngle) {
	float fCentralLeanAngle = atan2(O1.z, O1.x);
	float fOpenAngle = (iOpenAngle * 2.0 * PI) / 360;
	float fSegmentAngle = fOpenAngle / nSegment;
	float fLeanAngle = fCentralLeanAngle - fOpenAngle / 2;
	this->numVerts = (nSegment + 1) * 4 + 2;
	this->pt = new Point3[numVerts];
	int i, idx;
	float x1, y1, z1, x2, y2, z2;

	//draw vertices
	for (i = 0; i <= nSegment; i++) {
		x1 = O1.x + fOutRadius * cos(fLeanAngle + fSegmentAngle * i);
		z1 = O1.z + fOutRadius * sin(fLeanAngle + fSegmentAngle * i);
		y1 = fHigherHeight;
		x2 = O1.x + fInRadius * cos(fLeanAngle + fSegmentAngle * i);
		z2 = O1.z + fInRadius * sin(fLeanAngle + fSegmentAngle * i);
		y2 = fHigherHeight;
		pt[i].set(x1, y1, z1);
		pt[i + 2 * (nSegment + 1)].set(x2, y2, z2);
		y1 = 0;
		y2 = fLowerHeight;
		pt[i + 1 * (nSegment + 1)].set(x1, y1, z1);
		pt[i + 3 * (nSegment + 1)].set(x2, y2, z2);
	}
	pt[numVerts - 2].set(O1.x, fLowerHeight, O1.z);
	pt[numVerts - 1].set(O1.x, 0, O1.z);

	//draw faces
	this->numFaces = 5 * nSegment + 4;
	this->face = new Face[numFaces];
	idx = 0;
	//upper triangle
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = this->numVerts - 2;
		face[idx].vert[1].vertIndex = i + 3 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 3 * (nSegment + 1) + 1;
		idx++;
	}
	//near rectangle
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 3 * (nSegment + 1);
		face[idx].vert[1].vertIndex = i + 2 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 2 * (nSegment + 1) + 1;
		face[idx].vert[3].vertIndex = i + 3 * (nSegment + 1) + 1;
		idx++;
	}
	//top rectangle
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 2 * (nSegment + 1);
		face[idx].vert[1].vertIndex = i + 0 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 0 * (nSegment + 1) + 1;
		face[idx].vert[3].vertIndex = i + 2 * (nSegment + 1) + 1;
		idx++;
	}
	//far rectangle
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 1 * (nSegment + 1);
		face[idx].vert[1].vertIndex = i + 0 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 0 * (nSegment + 1) + 1;
		face[idx].vert[3].vertIndex = i + 1 * (nSegment + 1) + 1;
		idx++;
	}
	//lower triangle
	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = this->numVerts - 1;
		face[idx].vert[1].vertIndex = i + 1 * (nSegment + 1);
		face[idx].vert[2].vertIndex = i + 1 * (nSegment + 1) + 1;
		idx++;
	}
	//side faces
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = this->numVerts - 1;
	face[idx].vert[1].vertIndex = this->numVerts - 2;
	face[idx].vert[2].vertIndex = 3 * (nSegment + 1);
	face[idx].vert[3].vertIndex = 1 * (nSegment + 1);
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 3 * (nSegment + 1);
	face[idx].vert[1].vertIndex = 2 * (nSegment + 1);
	face[idx].vert[2].vertIndex = 0 * (nSegment + 1);
	face[idx].vert[3].vertIndex = 1 * (nSegment + 1);
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = this->numVerts - 1;
	face[idx].vert[1].vertIndex = this->numVerts - 2;
	face[idx].vert[2].vertIndex = 4 * (nSegment + 1) - 1;
	face[idx].vert[3].vertIndex = 2 * (nSegment + 1) - 1;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 4 * (nSegment + 1) - 1;
	face[idx].vert[1].vertIndex = 3 * (nSegment + 1) - 1;
	face[idx].vert[2].vertIndex = 1 * (nSegment + 1) - 1;
	face[idx].vert[3].vertIndex = 2 * (nSegment + 1) - 1;
}

void Mesh::CreateBasePlate(float fSize, float fHeight, float fBaseObjectHeight) {
	int i;
	this->numVerts = 8;
	this->pt = new Point3[numVerts];

	// points array
	pt[0].set(-fSize / 2, fBaseObjectHeight, -fSize / 2);
	pt[1].set(fSize / 2, fBaseObjectHeight, -fSize / 2);
	pt[2].set(fSize / 2, fBaseObjectHeight, fSize / 2);
	pt[3].set(-fSize / 2, fBaseObjectHeight, fSize / 2);
	pt[4].set(-fSize / 2, fHeight + fBaseObjectHeight, -fSize / 2);
	pt[5].set(fSize / 2, fHeight + fBaseObjectHeight, -fSize / 2);
	pt[6].set(fSize / 2, fHeight + fBaseObjectHeight, fSize / 2);
	pt[7].set(-fSize / 2, fHeight + fBaseObjectHeight, fSize / 2);

	this->numFaces = 6;
	this->face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 0;
	face[0].vert[1].vertIndex = 4;
	face[0].vert[2].vertIndex = 7;
	face[0].vert[3].vertIndex = 3;
	for (int i = 0; i < face[0].nVerts; i++) {
		face[0].vert[i].colorIndex = 0;
	}

	//Bottom face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 1;
	face[1].vert[2].vertIndex = 2;
	face[1].vert[3].vertIndex = 3;
	for (int i = 0; i < face[1].nVerts; i++) {
		face[1].vert[i].colorIndex = 1;
	}

	//Right face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 1;
	face[2].vert[1].vertIndex = 5;
	face[2].vert[2].vertIndex = 6;
	face[2].vert[3].vertIndex = 2;
	for (int i = 0; i < face[2].nVerts; i++) {
		face[2].vert[i].colorIndex = 2;
	}

	//Top face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 4;
	face[3].vert[1].vertIndex = 5;
	face[3].vert[2].vertIndex = 6;
	face[3].vert[3].vertIndex = 7;
	for (int i = 0; i < face[3].nVerts; i++) {
		face[3].vert[i].colorIndex = 3;
	}

	//Near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 1;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 4;
	face[4].vert[3].vertIndex = 0;
	for (int i = 0; i < face[4].nVerts; i++) {
		face[4].vert[i].colorIndex = 4;
	}

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 2;
	face[5].vert[1].vertIndex = 6;
	face[5].vert[2].vertIndex = 7;
	face[5].vert[3].vertIndex = 3;
	for (int i = 0; i < face[5].nVerts; i++) {
		face[5].vert[i].colorIndex = 5;
	}
}

void Mesh::CreateCuboid(float fSizeX, float fSizeY, float fSizeZ) {
	int i;
	this->numVerts = 8;
	this->pt = new Point3[numVerts];

	// points array
	pt[0].set(-fSizeX, -fSizeY, -fSizeZ);
	pt[1].set(fSizeX, -fSizeY, -fSizeZ);
	pt[2].set(fSizeX, fSizeY, -fSizeZ);
	pt[3].set(-fSizeX, fSizeY, -fSizeZ);
	pt[4].set(-fSizeX, -fSizeY, fSizeZ);
	pt[5].set(fSizeX, -fSizeY, fSizeZ);
	pt[6].set(fSizeX, fSizeY, fSizeZ);
	pt[7].set(-fSizeX, fSizeY, fSizeZ);

	this->numFaces = 6;
	this->face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 4;
	face[0].vert[1].vertIndex = 0;
	face[0].vert[2].vertIndex = 3;
	face[0].vert[3].vertIndex = 7;
	for (int i = 0; i < face[0].nVerts; i++) {
		face[0].vert[i].colorIndex = 0;
	}

	//Bottom face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 4;
	face[1].vert[1].vertIndex = 5;
	face[1].vert[2].vertIndex = 1;
	face[1].vert[3].vertIndex = 0;
	for (int i = 0; i < face[1].nVerts; i++) {
		face[1].vert[i].colorIndex = 1;
	}

	//Right face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 5;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 6;
	for (int i = 0; i < face[2].nVerts; i++) {
		face[2].vert[i].colorIndex = 2;
	}

	//Top face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 2;
	face[3].vert[3].vertIndex = 3;
	for (int i = 0; i < face[3].nVerts; i++) {
		face[3].vert[i].colorIndex = 3;
	}

	//Near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 6;
	face[4].vert[3].vertIndex = 7;
	for (int i = 0; i < face[4].nVerts; i++) {
		face[4].vert[i].colorIndex = 4;
	}

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 0;
	face[5].vert[1].vertIndex = 1;
	face[5].vert[2].vertIndex = 2;
	face[5].vert[3].vertIndex = 3;
	for (int i = 0; i < face[5].nVerts; i++) {
		face[5].vert[i].colorIndex = 5;
	}
}

void Mesh::CreateCube(float	fSize)
{
	int i;

	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-fSize, fSize, fSize);
	pt[1].set(fSize, fSize, fSize);
	pt[2].set(fSize, fSize, -fSize);
	pt[3].set(-fSize, fSize, -fSize);
	pt[4].set(-fSize, -fSize, fSize);
	pt[5].set(fSize, -fSize, fSize);
	pt[6].set(fSize, -fSize, -fSize);
	pt[7].set(-fSize, -fSize, -fSize);


	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i < face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i < face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for (i = 0; i < face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i < face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i < face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i < face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;

}


void Mesh::CreateTetrahedron()
{
	int i;
	numVerts = 4;
	pt = new Point3[numVerts];
	pt[0].set(0, 0, 0);
	pt[1].set(1, 0, 0);
	pt[2].set(0, 1, 0);
	pt[3].set(0, 0, 1);

	numFaces = 4;
	face = new Face[numFaces];

	face[0].nVerts = 3;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 2;
	face[0].vert[2].vertIndex = 3;
	for (i = 0; i < face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;


	face[1].nVerts = 3;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 2;
	face[1].vert[2].vertIndex = 1;
	for (i = 0; i < face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;


	face[2].nVerts = 3;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 3;
	face[2].vert[2].vertIndex = 2;
	for (i = 0; i < face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;


	face[3].nVerts = 3;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 1;
	face[3].vert[1].vertIndex = 3;
	face[3].vert[2].vertIndex = 0;
	for (i = 0; i < face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;
}


void Mesh::DrawWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;

			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::DrawColor()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;
			int		ic = face[f].vert[v].colorIndex;

			ic = f % COLORNUM;

			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::SetColor(int colorIdx)
{
	for (int f = 0; f < numFaces; f++)
	{
		for (int v = 0; v < face[f].nVerts; v++)
		{
			face[f].vert[v].colorIndex = colorIdx;
		}
	}
}

