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
//#include "joint.h"
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


//Function Prototypes
void read(string filename);
Joint* build_tree(ifstream& file, Joint *joint);
void write(Joint* joint);
void writeHierarchy(ofstream& outfile, Joint* joint);






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

  cout << "Reading...\n";
  Joint *root = new Joint();
  read_hierarchy(infile, root);

  vector<char*> channels = root->getChannelNames();
  for (int i=0; i<6; i++){
    cout << "ok ";
    cout << channels[i] << " ";
  }
  infile.close();
  cout << "done reading.\n-------------------------------\n\n";

  //vector motion = read_motion();




  //print_tree(root);
  cout << "Writing...\n";
  write(root);
  cout << "done writing.\n-------------------------------\n\n";
}
