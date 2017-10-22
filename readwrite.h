#include "joint.h"
#include <string>
#include <stdio.h>
#include <iostream>


using namespace std;

//FUNCTION PROTOTYPES
void read(string filename);
Joint* build_tree(ifstream& file, Joint *joint);






//starts reading joint at {
Joint* build_tree(ifstream& file, Joint *joint){
  cout << "build tree\n";
  string line;

  //read opening bracket
  getline(file,line); // {
  cout <<line << endl;

  //read OFFSET
  getline(file,line); // OFFSET - - -
  cout <<line << endl;
  const char *offsetLine = line.c_str();
  char offsetWord[10];
  float x_offset, y_offset, z_offset;
  sscanf(offsetLine, "%s %f %f %f", offsetWord, &x_offset, &y_offset, &z_offset);
  if(strcmp(offsetWord, "OFFSET")) exit(1); //error check
  joint->setXoffset(x_offset);
  joint->setYoffset(y_offset);
  joint->setZoffset(z_offset);
  if(joint->isEndSite()){
    getline(file, line); // }
    cout << line << endl;
    return joint;
  }


  //read CHANNELS
  getline(file,line); // CHANNELS - - - - -
  cout <<line << endl;
  const char *channelsLine = line.c_str();
  char channelsWord[10];
  int numCh;
  sscanf(channelsLine, "%s %d", channelsWord, &numCh);
  if(strcmp(channelsWord, "CHANNELS")) exit(2); //error check
  joint->setNumChannels(numCh);
  vector<char*> cnames; //list of channel names
  if(numCh == 3){
    char cname1[10], cname2[10], cname3[10], cname4[10], cname5[10], cname6[10];
    sscanf(channelsLine, "%s %d %s %s %s %s %s %s", channelsWord, &numCh,
      cname1, cname2, cname3, cname4, cname5, cname6);
    cnames.push_back(cname1);
    cnames.push_back(cname2);
    cnames.push_back(cname3);
    cnames.push_back(cname4);
    cnames.push_back(cname5);
    cnames.push_back(cname6);
  }
  else if(numCh == 6){
    char cname1[10], cname2[10], cname3[10];
    sscanf(channelsLine, "%s %d %s %s %s", channelsWord, &numCh, cname1, cname2, cname3);
    cnames.push_back(cname1);
    cnames.push_back(cname2);
    cnames.push_back(cname3);
  }
  else exit(3);

  //Next line is either JOINT or End Site follows CHANNELS
  //JOINT
  getline(file,line); // JOINT --- OR End Site ---
  cout << line << endl;
  const char *jointLine = line.c_str();
  char firstWord[10];
  sscanf(jointLine, "%s", firstWord);

  if (strcmp(firstWord, "JOINT") == 0){
    Joint *child = new Joint();
    char jointName[20];
    sscanf(jointLine, "%s %s", firstWord, jointName);
    child->setName(jointName);
    child->setParent(joint);
    //RECURSIVE CALL
    joint->addChild(build_tree(file,child));
  }
  //End Site
  else if(strcmp(firstWord, "End") == 0){
    Joint *endsite = new Joint();
    endsite->setEndSite();
    endsite->setParent(joint);
    joint->addChild(build_tree(file, endsite));
  }

  //return at closing bracket
  getline(file, line); // }
  cout << line << endl;
  const char *nextLine = line.c_str();
  char nextWord[10];
  sscanf(nextLine, "%s", nextWord);
  if (strcmp(nextWord, "}") == 0){
    cout << "return\n";
    return joint;
  }
  else if(strcmp(nextWord, "JOINT") == 0) {
    cout << "new child\n";
    char jointName[20];
    sscanf(nextLine, "%s %s", nextWord, jointName);
    Joint *child = new Joint();
    child->setName(jointName);
    child->setParent(joint);
    joint->addChild(build_tree(file, child));
  }
  cout << "final return\n";
  return joint;
}





void read(ifstream& infile){
    cout << "reading...\n";
    string line;
    getline(infile, line); //HIERARCHY
    cout << line << "\n";
    const char *str = line.c_str();
    char word[10];
    sscanf(str, "%s", word);
    if (strcmp(word, "HIERARCHY")) exit(-1); //error check
    getline(infile, line); //ROOT hips
    cout << line << "\n";
    char rootName[10];
    sscanf(str, "%s %s", word, rootName);
    if (strcmp(word, "ROOT") == 0){
      Joint *root = new Joint();
      root->setName(rootName);
      root = build_tree(infile,root);
    }
}
