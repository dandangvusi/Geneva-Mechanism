#if !defined (_MESH_CLASS)
#define _MESH_CLASS

#include "supportClass.h"
#include "float.h"


class VertexID
{
public:
	int		vertIndex;
	int		colorIndex;
};

class Face
{
public:
	int		nVerts;
	VertexID* vert;
	Vector3 facenorm; // vector phap tuyen cua mat

	Face()
	{
		nVerts = 0;
		vert = NULL;
	}
	~Face()
	{
		if (vert != NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		nVerts = 0;
	}
};

class Mesh
{
public:
	int		numVerts;
	Point3* pt;

	int		numFaces;
	Face* face;

	// For rotation
	float fRotateAngleY;
public:
	Mesh()
	{
		numVerts = 0;
		pt = NULL;
		numFaces = 0;
		face = NULL;
		fRotateAngleY = 0.0;
	}
	~Mesh()
	{
		if (pt != NULL)
		{
			delete[] pt;
		}
		if (face != NULL)
		{
			delete[] face;
		}
		numVerts = 0;
		numFaces = 0;
	}
	void DrawWireframe();
	void DrawColor();
	void SetColor(int colorIdx);

	// lab 2
	void CreateTetrahedron();
	void CreateCube(float fSize);
	void CreateCuboid(float fSizeX, float fSizeY, float fSizeZ);
	void CreateCylinder(int nSegment, float fHeight, float fRadius);
	void CreateCylinderWithHole(int nSegment, float fHeight, float fORadius, float fIRadius);
	void CreateShape1(int nSegment, float O1_O2_len, Point3 O1, float fO1InRadius, float fO1OutRadius, float fO2Radius, float fHeight, int iOpenAngle1, int iOpenAngle2);
	void CreateShape2(int nSegment, Point3 O1, float fLowerHeight, float fHigherHeight, float fInRadius, float fOutRadius, int iOpenAngle);
	Point3 findIntersection(Point3 p1, Point3 p2, Point3 p3, Point3 p4);

	// lab 3
	void CreateBasePlate(float fSize, float fHeight, float fBaseObjectHeight);
	void CreateDrivenWheel(int nSegment, float O1_O2_len, float fO1InRadius, float fO1OutRadius, float fO2Radius, float fHeight, int iOpenAngle1, int iOpenAngle2, float fBaseObjectHeight);
	void CreateRotateWheel(int nSegment, Point3 Shape2_O1, float fShape2HigherHeight, float fShape2LowerHeight, float fShape2InRadius, float fShape2OutRadius, int iShape2OpenAngle, int iShape2CentralLeanAngle, float fCylinderHeight, float fCylinderRadius, float O1_O2_len, float fShape2BaseHeight, float fCylinderBaseHeight);

	// lab 5
	void CalculateFacesNorm();
	void Draw();
	void setupMaterial(float ambient[], float diffuse[], float specular[], float shiness);
};

#endif