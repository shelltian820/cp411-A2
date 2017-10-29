//Author: Shelley Song Tian
//CMPUT 411 Assignment 2
//Oct 11, 2017

//Assumptions: This program does not do error checks within bvh files

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <regex>
#include <vector>
#include <iomanip>
#include <cmath>
#include <cctype>
#include "readwrite.h"


#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

using namespace std;

//Globals
Joint *root = new Joint();
vector<Joint*> joints;
int numFrames = 0; int *numFramesPointer = &numFrames;
int currentFrameIndex = 0;
float frameTime = 0.0; float *frameTimePointer = &frameTime;
vector<vector<float>> frames;
float eyeX = 0.0, eyeY = 0.0, eyeZ = -30.0, centerX = 0.0, centerY = 0.0, centerZ = 0.0, upX = 0.0, upY = 0.1, upZ = 0.0;
static float Pleft = -10.0, Pright = 10.0, Pbottom = -10.0, Ptop = 10.0, Pnear = -100.0, Pfar = 100.0; //camera for Perspective mode
bool playMode = false;


//Function Prototypes
static unsigned int human;
void keyInput(unsigned char key, int x, int y);
void drawScene();
void resize(int w, int h);
void setup();
void reset();
void create_lines(Joint *joint, float parentX, float parentY, float parentZ);
void timer_func(int t);
void next_frame();
void translate_figure();














///////////////////////////////////////////////////////////////////////////////
///////////////////////////  M   A    I   N  //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]){
  //check file format
  if (argc != 2){
    cout << "Invalid. Please input one .bvh file.\n";
    exit(1);
  }
  string fileName = argv[1];
  string bvh = ".bvh";
  string input_end = fileName.substr(fileName.size()-4);
  if ( input_end.compare(bvh) ){
    cout << "Invalid. Please input \".bvh\" file.\n";
    exit(1);
  }

  // read file
  read(fileName, root, joints, numFramesPointer, frameTimePointer, frames);
  cout << "*numframes =" << numFrames << endl;
  cout << "*frameTime =" << frameTime << endl;

  ////////////////////////////////////////////////////////
  glutInit(&argc, argv);
  glutInitContextVersion(4, 3);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);

  setup(); //called once

  glutDisplayFunc(drawScene); //called every time something changes
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyInput);

  glewExperimental = GL_TRUE;
  glewInit();
  glutMainLoop();

  return 0;

}













///////////////////////////////////////////////////////////////////////////////
//////////////////////  F  U  N  C  T  I  O  N  S  ////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void drawScene(){
  cout << "drawScene" << endl;
  glClear (GL_COLOR_BUFFER_BIT);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //?????

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-30.0, 30.0, -30.0, 30.0, -100.0, 100.0);
  //glFrustum(-30.0, 30.0, -30.0, 30.0, -1.0, 10.0);
  //gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //TODO: display current frame
  // glBegin(GL_LINES);
  // create_lines(root, root->getXoffset(), root->getYoffset(), root->getZoffset());
  // glEnd();
  translate_figure();
  //move limbs



  glColor3f(1.0, 1.0, 1.0); //make lines white
  glLineWidth(1.5);
  glPushMatrix();
  glCallList(human); //call display list created in setup
  glPopMatrix();
  glutSwapBuffers();
}







void translate_figure(){
  //depending on the current frame
  //get current xpos, ypos, and zpos of root
  //read channels of root, then match frame numbers to channels
  float xpos=0, ypos=0, zpos=0;
  vector<string> rootChannels = root->getChannelNames();
  for(int i=0; i<6; i++){
    if(rootChannels[i] == "Xposition") xpos = frames[currentFrameIndex][i];
    if(rootChannels[i] == "Xposition") ypos = frames[currentFrameIndex][i];
    if(rootChannels[i] == "Xposition") zpos = frames[currentFrameIndex][i];
  }
  cout << "translate (" << xpos << ", " << ypos << ", " << zpos << ")\n";
  glTranslatef(xpos, ypos, zpos);
}



void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

  human = glGenLists(2);
  glNewList(human, GL_COMPILE);
  glBegin(GL_LINES);
  //add lines
  create_lines(root, root->getXoffset(), root->getYoffset(), root->getZoffset());
  //
  glEnd();
  glEndList();
}


void create_lines(Joint *joint, float parentX, float parentY, float parentZ){
  //create a line between point p and q,
  float jointX = parentX + joint->getXoffset();
  float jointY = parentY + joint->getYoffset();
  float jointZ = parentZ + joint->getZoffset();

  //create line between joint and parent
  glVertex3f(jointX, jointY, jointZ);
  glVertex3f(parentX, parentY, parentZ);

  if (joint->isEndSite()){
    return; //????
  }
  else{
    vector<Joint*> children = joint->getChildren();
    int numCh = joint->getCount();
    for (int i=0; i<numCh; i++){
      create_lines(children[i], jointX, jointY, jointZ);
    }
  }
}


void timer_func(int t){
  // Do something every X milliseconds
  // move to next frame
  if (playMode){
    next_frame();
    cout << currentFrameIndex << endl;
    glutPostRedisplay();
    int msecFrameTime = frameTime*1000;
    // glutTimerFunc(msecFrameTime, timer_func, 0);
    glutTimerFunc(msecFrameTime, timer_func, 0);
  }
}


void next_frame(){
  if (currentFrameIndex < numFrames-1) currentFrameIndex++;
  else currentFrameIndex = 0;
}





void keyInput(unsigned char key, int x, int y){
   switch(key)
   {
      case 'q':
        exit(0);
        break;
      case 'x': //reset
        reset();
        break;
      case 'w':
        write(root, frames, numFrames, frameTime);
        break;
      case 'p': //play
        if (playMode == false){
          playMode = true;
          timer_func(0);
        }
        glutPostRedisplay();
        break;
      case 'P': //pause
        if (playMode == true){
          playMode = false;
        }
        glutPostRedisplay();
        break;

      //camera control
      case 'd': //dolly
        break;
      case 'D':
        break;
      case 'c': //crane
        break;
      case 'C':
        break;
      case 'z': //zoom
        break;
      case 'Z':
        break;
      case 't': //tilt
        break;
      case 'T':
        break;
      case 'a': //pan
        break;
      case 'A':
        break;
      case 'l': //roll
        break;
      case 'L':
        break;

      //speed control
      case 43: // +
        break;
      case 45: // -
        break;

      default:
        break;
  }
  glutPostRedisplay();
}


void reset(){
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glutPostRedisplay();
}

void resize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

}
