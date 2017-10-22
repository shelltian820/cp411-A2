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

//Class Definitions

// class Joint {
//     public:
//         string name;
//         vector<Joint*> children;
//         Joint *parent = 0;
//         float Xoffset, Yoffset, Zoffset;
//         int numChannels = 0;
//         float Xposition, Yposition, Zpostition;
//         float Zrotation, Yrotation, Xrotation;
//
//     string getName(){
//         return name;
//     }
//     void setName(string n){
//         name = n;
//     }
//     vector<Joint*> getChildren(){
//         return children;
//     }
//     int getCount(){
//         return children.size();
//     }
//     void addChild(Joint *child){
//         children.push_back(child);
//     }
//     Joint* getParent(){
//         return parent;
//     }
//     void setParent(Joint* p){
//         parent = p;
//     }
//     bool isRoot(){
//         if (parent == NULL){
//             return true;
//         } else {
//             return false;
//         }
//     }
//     int getNumChannels(){
//       return numChannels;
//     }
//     int setNumChannels(int n){
//       numChannels = n;
//     }
//
//
//
// };

///////////////////////////////////////////////////////////////////////////////
//////////////////////F  U  N  C  T  I  O  N  S////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// string toUpper(string s){
//   for (auto& x: s) x.toupper(x);
//   return s;
// }

// void read(string filename, Joint root){
//     ifstream infile;
//     infile.open(filename);
//     if (!infile) {
//           cout << "Unable to open file. Exiting.\n";
//           exit(2); //terminate with error
//     }else{
//         //TODO: Read file and store data
//         cout << "reading...\n";
//         ifstream infile;
//         infile.open(filename);
//         bool hierarchy = false;
//         bool motion = false;
//         bool open_bracket = false;
//         bool closed_bracket = false;
//
//         string line;
//         while (getline(infile,line)){
//           //convert string to char*
//           const char *str = line.c_str();
//           char beginning[10];
//           sscanf(str, "%s", beginning);
//           if (strcmp(beginning, "HIERARCHY")) == 0 ){
//             hierarchy = true;
//             motion = false;
//             continue;
//           }
//           if (hierarchy = true){
//             char firstword[10];
//             //scan at first word of line
//             if (strcmp(beginning, "ROOT") == 0){
//
//             }
//             else if(strcmp(beginning, "JOINT") == 0){
//
//             }
//             else if(strcmp(beginning, "OFFSET") == 0){
//
//             }
//             else if(strcmp(beginning, "CHANNELS") == 0){
//
//             }
//             else if(strcmp(beginning, "JOINT") == 0){
//
//             }
//
//           }
//
//
//
//         }
//
//
//
//
//     }
//     infile.close();
// }

void write(){
    ofstream outfile;
    outfile.open ("output.bvh");

    //TODO: write file


    outfile.close();
    cout << "\n Data written to: output.bvh \n";
}













///////////////////////////////////////////////////////////////////////////////
/////////////////////////////M   A    I   N////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]){
  //check file format
  if (argc != 2){
    cout << "Invalid. Please input one .bvh file.\n";
    exit(1);
  }
  string input = argv[1];
  string obj = ".bvh";
  string input_end = input.substr(input.size()-4);
  if ( input_end.compare(obj) ){
    cout << "Invalid. Please input \".bvh\" file.\n";
    exit(1);
  }

  Joint *root = new Joint();
  read(input);
}
