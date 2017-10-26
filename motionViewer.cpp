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
int numFrames;
float frameTime;
vector<vector<float>> frames;


//Function Prototypes
void keyInput(unsigned char key, int x, int y);
void reset();















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

  ifstream infile;
  infile.open(fileName);
  if (!infile) {
        cout << "Unable to open file. Exiting.\n";
        exit(2); //terminate with error
  }

  //read hierarchy
  cout << "Reading...\n";
  // Joint *root = new Joint();
  read_hierarchy(infile, root);
  list_tree(root, joints);

  //read motion
  string line;
  getline(infile, line); //MOTION
  getline(infile, line); //Frames: 20
  const char *framesLine = line.c_str();
  char w1[10];
  sscanf(framesLine, "%s %d", w1, &numFrames);
  getline(infile, line); // Frame Time: 0.012312
  const char *frameTimeLine = line.c_str();
  char w2[10];
  sscanf(frameTimeLine, "%s %s %f", w1, w2, &frameTime);
  read_motion(infile, frames);
  //print_motion(frames);
  infile.close();
  cout << "Done.\n-------------------------------\n\n";

  ////////////////////////////////////////////////////////
  glutInit(&argc, argv);
  glutInitContextVersion(4, 3);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Animation");

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
  glColor3f(1.0, 1.0, 1.0);
  glPushMatrix();
  //glCallList(anObject);???
  glPopMatrix();

  glutSwapBuffers();
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
        cout << "Writing...\n";
        ofstream outfile;
        outfile.open("output.bvh");
        //write hierarchy
        outfile << "HIERARCHY\n";
        write_hierarchy(outfile, root);
        outfile << "}\n"; //matches root bracket

        //write motion
        write_motion(outfile, frames, numFrames, frameTime);
        outfile.close();
        cout << "Done.\n-------------------------------\n\n";
        break;
      case 'p': //pause
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


void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

}
