#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include <vector>

//Struct Vertices (Vertex)
struct Vertex3D
{
 float x;
 float y;
 float z;

 	void assign (float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	Vertex3D operator+(Vertex3D &Other)
	{
		Vertex3D Temp;
		Temp.x = x + Other.x;
		Temp.y = y + Other.y;
		Temp.z = z + Other.z;
		return Temp;
	}
};

//Struct Normals
struct Normal3D
{
 float x;
 float y;
 float z;
};

struct Face
{
 std::vector<int> VertexIndex;
 std::vector<int> NormalIndex;
};

#endif
