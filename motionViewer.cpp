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
//vector<Joint*> joints;
int numFrames = 0; int *numFramesPointer = &numFrames;
int currentFrameIndex = 0;
float frameTime = 0.0; float *frameTimePointer = &frameTime;
vector<vector<float>> frames;
static float Fleft=-20.0, Fright=20.0, Fbottom=-20.0, Ftop=20.0, Fnear=5.0, Ffar=100.0; //frustum parameters
bool playMode = false;
bool initialPose = true;


//Function Prototypes
static unsigned int human;
void keyInput(unsigned char key, int x, int y);
void drawScene();
void resize(int w, int h);
void setup();
void reset();
float deg_to_rad(float degree);
void draw_figure(Joint *joint, float parentX, float parentY, float parentZ);
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
  read(fileName, root, numFramesPointer, frameTimePointer, frames);
  // cout << "*numframes =" << numFrames << endl;
  // cout << "*frameTime =" << frameTime << endl;

  ////////////////////////////////////////////////////////
  glutInit(&argc, argv);
  glutInitContextVersion(4, 3);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[1]);

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
  //glClear (GL_COLOR_BUFFER_BIT);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  // glOrtho(-30.0, 30.0, -30.0, 30.0, -100.0, 100.0);
  glFrustum(Fleft, Fright, Fbottom, Ftop, Fnear, Ffar);
  //gluLookAt(0.0, 20.0 , -10, 0.0, 20.0, -15.0, 0.0, 1.0, 0.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //display current frame
  glTranslatef(0.0,0.0,-10);
  translate_figure();


  glColor3f(1.0, 1.0, 1.0); //make lines white
  glLineWidth(1.5);
  glPushMatrix();
  //glCallList(human); //call display list created in setup

  glBegin(GL_LINES);
  draw_figure(root, root->getXoffset(), root->getYoffset(), root->getZoffset());
  glEnd();

  glPopMatrix();
  glutSwapBuffers();
}

float deg_to_rad(float degree){
  float radian = 3.141592/180.0 * degree;
  return radian;
}

void draw_figure(Joint *joint, float parentX, float parentY, float parentZ){
  //figure is drawn depending on current frame
  //create a line between point p and q,
  float jointX, jointY, jointZ;

  if (initialPose){
    jointX = parentX + joint->getXoffset();
    jointY = parentY + joint->getYoffset();
    jointZ = parentZ + joint->getZoffset();
  }

  else{
    jointX =  joint->getXoffset();
    jointY =  joint->getYoffset();
    jointZ =  joint->getZoffset();


    //rotating current point by xrot, yrot, zrot
    //find corresponding rotation values for the current frame and joint

    vector<float> currentFrame = frames[currentFrameIndex];
    int jointID = joint->getID(); //joint id is the same index in joints
    float zrot = deg_to_rad(currentFrame[3+3*jointID]);
    float yrot = deg_to_rad(currentFrame[3+3*jointID+1]);
    float xrot = deg_to_rad(currentFrame[3+3*jointID+2]);
    //rotate along z axis
    jointX = cos(zrot)*jointX - sin(zrot)*jointY;
    jointY = sin(zrot)*jointX + cos(zrot)*jointY;
    //rotate along y axis
    jointX = cos(yrot)*jointX + sin(yrot)*jointZ;
    jointZ = -sin(yrot)*jointX + cos(yrot)*jointZ;
    //rotate along x axis
    jointY = cos(xrot)*jointY - sin(xrot)*jointZ;
    jointZ = sin(xrot)*jointY + cos(xrot)*jointZ;

    jointX += parentX;
    jointY += parentY;
    jointZ += parentZ;
  }

  //draw line between joint and parent
  glVertex3f(jointX, jointY, jointZ);
  glVertex3f(parentX, parentY, parentZ);


  if (joint->isEndSite()){
    return;
  }
  else{
    vector<Joint*> children = joint->getChildren();
    int numCh = joint->getCount();
    for (int i=0; i<numCh; i++){
      draw_figure(children[i], jointX, jointY, jointZ);
    }
  }
}



void translate_figure(){
  //depending on the current frame
  //get current xpos, ypos, and zpos of root
  //read channels of root, then match frame numbers to channels
  float xpos=0, ypos=0, zpos=0;
  vector<string> rootChannels = root->getChannelNames();
  for(int i=0; i<6; i++){
    if(rootChannels[i] == "Xposition") xpos = frames[currentFrameIndex][i];
    if(rootChannels[i] == "Yposition") ypos = frames[currentFrameIndex][i];
    if(rootChannels[i] == "Zposition") zpos = frames[currentFrameIndex][i];
  }
  //cout << "translate (" << xpos << ", " << ypos << ", " << zpos << ")\n";
  glTranslatef(xpos, ypos, zpos);

}




void timer_func(int t){
  // Do something every X milliseconds
  // move to next frame
  if (playMode){
    next_frame();
    glutPostRedisplay();
    int msecFrameTime = frameTime*1000;
    glutTimerFunc(msecFrameTime, timer_func, 0);
    // glutTimerFunc(100, timer_func, 0);
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
          initialPose = false;
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
        Fleft -= 1.0;
        Fright -= 1.0;
        break;
      case 'D':
        Fleft += 1.0;
        Fright += 1.0;
        break;
      case 'c': //crane
        Fbottom -= 1.0;
        Ftop -= 1.0;
        break;
      case 'C':
        Fbottom += 1.0;
        Ftop += 1.0;
        break;
      case 'z': //zoom
        Fnear -= 1.0;
        Ffar -= 1.0;
        break;
      case 'Z':
        Fnear += 1.0;
        Ffar += 1.0;
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
  playMode = false;
  initialPose = true;
  Fleft=-20.0; Fright=20.0; Fbottom=-20.0; Ftop=20.0; Fnear=5.0; Ffar=100.0;
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  currentFrameIndex = 0;
  setup();
  glutPostRedisplay();

}

void resize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

}


void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

  //create object
  // human = glGenLists(2);
  // glNewList(human, GL_COMPILE);
  // glBegin(GL_LINES);
  // draw_figure(root, root->getXoffset(), root->getYoffset(), root->getZoffset());
  // glEnd();
  // glEndList();

  // adjust camera position depending on first frame's x, y, z position
  // change frustum parameters
  // float xpos=0, ypos=0, zpos=0;
  // vector<string> rootChannels = root->getChannelNames();
  // for(int i=0; i<6; i++){
  //   if(rootChannels[i] == "Xposition") xpos = frames[currentFrameIndex][i];
  //   if(rootChannels[i] == "Yposition") ypos = frames[currentFrameIndex][i];
  //   if(rootChannels[i] == "Zposition") zpos = frames[currentFrameIndex][i];
  // }
  // Fleft = xpos - 20.0;
  // Fright = xpos + 20.0;
  // Fbottom = ypos - 20.0;
  // Ftop = ypos + 20.0;
  // Fnear = -zpos - 20.0;
  // Ffar = -zpos + 100.0;
  // cout << xpos << " " << ypos << " " << zpos << " " ;
  // cout << "frustum (" << Fleft << ", " << Fright << ", "<< Fbottom << ", "<< Ftop << ", "<< Fnear << ", "<< Ffar << ")\n ";

}
