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
    public:
        string name;
        vector<Joint> children;
        Joint *parent;
        
    string getName(){
        return name;
    }
    void setName(string n){
        name = n;
    }
    vector<Joint> getChildren(){
        return children;
    }
    int getCount(){
        return children.size();
    }
    void addChild(Joint child){
        children.push_back(child);
    }
    Joint* getParent(){
        return parent;
    }
    void setParent(Joint* p){
        parent = p;
    }
    bool isRoot(){
        if (parent = NULL){
            return true;
        } else {
            return false;
        }
    }
};

///////////////////////////////////////////////////////////////////////////////
//////////////////////F  U  N  C  T  I  O  N  S////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void read(string filename, Joint root){
    ifstream infile;
    infile.open(filename);
    if (!infile) {
          cout << "Unable to open file. Exiting.\n";
          exit(2); //terminate with error
    }else{
        //TODO: Read file and store data
        cout << "blah\n";
        
        
        
        
        
        
    }
    infile.close();
}

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
  Joint hips;
  hips.setName("Hips");
  read(input, hips);
}



