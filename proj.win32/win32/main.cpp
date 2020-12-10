#include <iostream>
#include <fstream>
#include <Windows.h>

#define PI 3.14159265

#include "ModelData.h"
#include "ModelParser.h"
#include "GLUTSetup.h"

using namespace std;

// Color3 Class is used to manipulate the Color of the object
class Color3
{
public:
	// Default Constructor that sets the color to WHITE (1, 1, 1)
	Color3()
	{
		R = 1.0f;
		G = 1.0f;
		B = 1.0f;
	}

	// Constructor that can Set the values for R, G, B values
	Color3(float r, float g, float b)
	{
		R = r;
		G = g;
		B = b;
	}

	float R;
	float G;
	float B;
private:
protected:
};

enum Transformation { rotX, rotY, rotZ } transform;

// Class that handles objects being loaded
class ModelLoader
{
public:
	vector<Vertex3D> vertices;
	vector<Normal3D> normals;
	vector<Face> faces;

	// Default Constructor
	ModelLoader() {	
		this->angleOffset = 57.5f;
	} 

	void Translate(float x, float y, float z) {
		for(int i = 0; i < this->vertices.size(); i++) {
			this->vertices[i].x += x;
			this->vertices[i].y += y;
			this->vertices[i].z += z;
		}
	}

	void RotateX(float angle) {
		this->rotAngle = angle;
		transform = rotX;
	}

	void RotateY(float angle) {
		this->rotAngle = angle;
		transform = rotY;
	}

	void RotateZ(float angle) {
		this->rotAngle = angle;
		transform = rotZ;
	}

	void Scale(float scaleFactor, float scaleX, float scaleY, float scaleZ) {
		for(int i = 0; i < this->vertices.size(); i++) {
			this->vertices[i].x = this->vertices[i].x * scaleFactor + (1 - scaleFactor) * scaleX;
			this->vertices[i].y = this->vertices[i].y * scaleFactor + (1 - scaleFactor) * scaleY;
			this->vertices[i].z = this->vertices[i].z * scaleFactor + (1 - scaleFactor) * scaleZ;
		}
	}

	void LoadObj(const char *OBJFile)
	{
		char pwd[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, pwd);

		std::string path(pwd);

		if (IsDebuggerPresent())
		{
#if DEBUG
			path = path + "\\Debug.win32\\" + OBJFile;
#else
			path = path + "\\Release.win32\\" + OBJFile;
#endif
		}
		else
		{
			path = path + "\\" + OBJFile;
		}

		printf("Loading .. %s\n", path.c_str());
		ifstream ifs(path);

		ifstream file(path, ios::binary);
		long len = file.tellg();
		file.close();

		printf("%lu\n", len);

		char data[100];
		while(ifs >> data) // eof = End of File
		{
			if(strcmp(data, "v") == 0) // strcmp = string compare
			{
				float x, y, z;

				// Extract x, y, z
				ifs >> x;
				ifs >> y;
				ifs >> z;

				switch(transform) {
				case rotX:
					this->RotX(x, y, z);
					break;

				case rotY:
					this->RotY(x, y, z);
					break;

				case rotZ:
					this->RotZ(x, y, z);
					break;
				}

				Vertex3D vert; // create vertex3D Object; look at ModelData.h
				vert.x = x;
				vert.y = y;
				vert.z = z;
				vertices.push_back(vert);
			}
			else if(strcmp(data, "vn") == 0)
			{
				float x, y, z;
				ifs >> x;
				ifs >> y;
				ifs >> z;

				switch(transform) {
				case rotX:
					this->RotX(x, y, z);
					break;

				case rotY:
					this->RotY(x, y, z);
					break;

				case rotZ:
					this->RotZ(x, y, z);
					break;
				}

				Normal3D norm;
				norm.x = x;
				norm.y = y;
				norm.z = z;
				normals.push_back(norm);
			}
			else if(strcmp(data, "f") == 0)
			{
				char FaceData[100];
				Face face;

				while(true)
				{
					ifs >> FaceData; // f 1//1 2//1 3//1 4//1
				
					if(ContainSlash(FaceData, 100))
					{
						char Vertexindx[15];
						char Normalindx[15];

						ExtractVertexIdx(FaceData, Vertexindx);
						ExtractNormalIdx(FaceData, Normalindx);

						int vIndex = atoi(Vertexindx); // atoi = convert char to int
						int nIndex = atoi(Normalindx);

						face.VertexIndex.push_back(vIndex);
						face.NormalIndex.push_back(nIndex);
					}
					else
					{
						faces.push_back(face);
						ifs.unget();
						break;
					}

				}
			}
		}

		ifs.close();
	}

	void RenderModel()
	{
		glLineWidth(2.0);
		glPushMatrix();
		glColor3f(this->Color.R, this->Color.G, this->Color.B);

		for(int i = 0; i < faces.size(); i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < faces[i].NormalIndex.size(); j++)
			{

			int nIndx = faces[i].NormalIndex[j] - 1;
				float x, y ,z;

				x = normals[nIndx].x;
				y = normals[nIndx].y;
				z = normals[nIndx].z;

				glNormal3f(x, y, z);

			int vIndx = faces[i].VertexIndex[j] - 1;

				x = vertices[vIndx].x;
				y = vertices[vIndx].y;
				z = vertices[vIndx].z;

				glVertex3f(x, y, z);
			}
			glEnd();
		}
		glPopMatrix();
	}

private:
	Color3 Color;
	float rotAngle;
	float angleOffset;

	void RotX(float &x, float &y, float &z) {
		float angle = this->rotAngle * this->angleOffset;
		angle = angle * (PI / 180.0f);
		float tmp = y;
		y = y * cos(angle * (PI / 180.0f)) - z * sin(angle * (PI / 180.0f));
		z = tmp * sin(angle * (PI / 180.0f)) + z * cos(angle * (PI / 180.0f));
	}

	void RotY(float &x, float &y, float &z) {
		float angle = this->rotAngle * this->angleOffset;
		angle = angle * (PI / 180.0f);
		float tmp = z;
		z = z * cos(angle * (PI / 180.0f)) - x * sin(angle * (PI / 180.0f));
		x = tmp * sin(angle * (PI / 180.0f)) + x * cos(angle * (PI / 180.0f));
	}

	void RotZ(float &x, float &y, float &z) {
		float angle = this->rotAngle * this->angleOffset;
		angle = angle * (PI / 180.0f);
		float tmp = x;
		x = x * cos(angle * (PI / 180.0f)) - y * sin(angle * (PI / 180.0f));
		y = tmp * sin(angle * (PI / 180.0f)) + y * cos(angle * (PI / 180.0f));
	}
protected:
};

// ModelLoader Class Instance using Default Constructor
ModelLoader GalagaShip;

void GameScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Draw your Stuff below this line
	glPushMatrix();
		GalagaShip.RenderModel();
	glPopMatrix();

	glutSwapBuffers();
}

void Keys(unsigned char key, int x, int y)
{
	switch(key)
	{

	case 27:
		exit(0);
		break;
	}
}

void ResizeWindow(int w, int h)
{
  float ratio = 1.0 * w / h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glViewport(0, 0, w, h);
  
  gluPerspective(45.0, ratio, 1, 1000);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void Timer(int extra)
{
  glutPostRedisplay();
  glutTimerFunc(10, Timer, extra);
}

void main (int argc, char **argv)
{
	float translateX; float translateY; float translateZ;
	translateX = 0.0f; translateY = 0.0f; translateZ = 0.0f;

	float scaleFactor; float xFactor; float yFactor; float zFactor;
	scaleFactor = 0.0f; xFactor = 0.0f; yFactor = 0.0f; zFactor = 0.0f;

	int choose; float angle; string OBJname;
	choose = 0; angle = 0.0f; OBJname = "assets\\galaga ship.obj";

	//cout << "Model Name[filename]: ";
	//getline(cin, OBJname);
	//cout << endl;

	cout << "NOTE: picking values beyond this choices" << endl;
	cout << "will make the model to its default transformation" << endl << endl;
	cout << "Choose the following: " << endl;
	cout << "1. Translate" << endl;
	cout << "2. Rotate about X axis" << endl;
	cout << "3. Rotate about Y axis" << endl;
	cout << "4. Rotate about Z axis" << endl;
	cout << "5. Scale" << endl;
	cout << "Choose: ";
	cin >> choose;
	cout << endl;

	const char* modelName = OBJname.c_str();

	switch(choose) {
	case 1:
		cout << "Enter the value of tx, ty, and tz: ";
		cin >> translateX >> translateY >> translateZ;
		GalagaShip.LoadObj(OBJname.c_str());
		GalagaShip.Translate(translateX, translateY, translateZ);
		break;

	case 2:
		cout << "Enter the angle: ";
		cin >> angle;
		GalagaShip.RotateX(angle);
		GalagaShip.LoadObj(OBJname.c_str());
		break;

	case 3:
		cout << "Enter the angle: ";
		cin >> angle;
		GalagaShip.RotateY(angle);
		GalagaShip.LoadObj(OBJname.c_str());
		break;

	case 4:
		cout << "Enter the angle: ";
		cin >> angle;
		GalagaShip.RotateZ(angle);
		GalagaShip.LoadObj(OBJname.c_str());
		break;

	case 5:
		cout << "Enter the value of sf, xf, yf, and zf: ";
		cin >> scaleFactor >> xFactor >> yFactor >> zFactor;
		GalagaShip.LoadObj(OBJname.c_str());
		GalagaShip.Scale(scaleFactor, xFactor, yFactor, zFactor);
		break;

	default:
		GalagaShip.RotateX(90);
		GalagaShip.LoadObj(OBJname.c_str());
		break;
	}

	//GalagaShip.RotateX(90);
	//GalagaShip.LoadObj(OBJname.c_str());
	//GalagaShip.Translate(0.0f, -25.0f, 0.0f);
	//GalagaShip.Scale(0.5f, 0.0f, 0.0f, 0.0f);

	GLUTInit(argc, argv);
}

// Goodluck :)
// Have fun programming folks ;)