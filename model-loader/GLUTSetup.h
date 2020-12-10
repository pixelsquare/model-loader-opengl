#ifndef __BASPRO2GLUTSetup_H__
#define __BASPRO2GLUTSetup_H__

#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include <math.h>
#include <string>
#include<glut.h>

#include "ModelData.h"
#include "ModelParser.h"


void GameScene();
void Keys(unsigned char key, int x, int y);
void SpecialKeys(int key, int x, int y);
void ResizeWindow(int w, int h);
void Timer(int extra);

void GLUTInit(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
 
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 500);
	glutCreateWindow("Model Loader");
 
	glutReshapeFunc(ResizeWindow);
	glutDisplayFunc(GameScene);
	//glutIdleFunc(GameScene);

	 glutTimerFunc(0, Timer, 0);
	 //glutSpecialFunc(SpecialKeys);
	 glutKeyboardFunc(Keys);

	 glEnable(GL_DEPTH_TEST);
	 glEnable(GL_COLOR_MATERIAL);
	 glEnable(GL_LIGHTING); //Enable lighting
	 glEnable(GL_LIGHT0); //Enable light #0
	 glEnable(GL_NORMALIZE); //Automatically normalize normals
	 glShadeModel(GL_SMOOTH); //Enable smooth shading
	
	glutMainLoop();
}

#endif