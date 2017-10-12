//Author: Shelley Song Tian
//CMPUT 411 Assignment 2
//Oct 11, 2017

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <regex>
#include <vector>
#include <iomanip>
#include <cmath>

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

class Joint {
    private:
        string name;
        vector<Joint> children;
        Joint parent = NULL;
        
    string getName(){
        return name;
    }
    vector<Joint> getChildren(){
        return children;
    }
    int getCount(){
        return children.size();
    }
    Joint getParent(){
        return parent;
    }
    void addChild(Joint child){
        children.push_back(child);
    }
    void setParent(Joint p){
        parent = p;
    }
    bool isRoot(){
        if (parent = NULL){
            return true;
        } else {
            return false;
        }
    }
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
  
  read();
}



