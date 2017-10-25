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
vector<Joint*> joints;
int numFrames;
float frameTime;
vector<vector<float>> frames;


//Function Prototypes







///////////////////////////////////////////////////////////////////////////////
/////////////////////////////M   A    I   N////////////////////////////////////
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
  Joint *root = new Joint();
  read_hierarchy(infile, root);
  list_tree(root, joints);

  //read motion
  string line;
  getline(infile, line); //MOTION
  getline(infile, line); //Frames: 20
  const char *framesLine = line.c_str();
  char w1[10];
  sscanf(framesLine, "%s %d", w1, &numFrames);
  cout << "numFrames = " << numFrames << endl;
  getline(infile, line); // Frame Time: 0.012312
  const char *frameTimeLine = line.c_str();
  char w2[10];
  sscanf(frameTimeLine, "%s %s %f", w1, w2, &frameTime);
  cout << "frameTime = " << frameTime << endl;
  read_motion(infile, frames);
  //print_motion(frames);
  infile.close();
  cout << "done reading.\n-------------------------------\n\n";






  //write
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
  cout << "done writing.\n-------------------------------\n\n";
}
