#include "joint.h"
#include <string>
#include <stdio.h>
#include <iostream>


using namespace std;

//FUNCTION PROTOTYPES
void read(string filename);
Joint* build_tree(ifstream& file, Joint *joint);
void write(Joint* joint);
void writeHierarchy(ofstream& outfile, Joint* joint);



//reads bvh file
void read_hierarchy(ifstream& infile, Joint *root){
    //read HIERARCHY
    string line;
    getline(infile, line); //HIERARCHY
     //cout << line << "\n";
    const char *str = line.c_str();
    char word[10];
    sscanf(str, "%s", word);
    if (strcmp(word, "HIERARCHY")) exit(-1); //error check
    getline(infile, line); //ROOT hips
     //cout << line << "\n";
    char rootName[20];
    sscanf(str, "%s %s", word, rootName);
    if (strcmp(word, "ROOT")) exit(-2);
    root->setName(rootName);

    root = build_tree(infile,root);

}


//starts reading joint at {
//returns joint at }
Joint* build_tree(ifstream& infile, Joint *joint){
  //cout << "build tree\n";
  //cout << joint->getName() << endl;
  string line;

  //read opening bracket
  getline(infile,line); // {
  //cout <<line << endl;

  //read OFFSET
  getline(infile,line); // OFFSET - - -
  //cout <<line << endl;
  const char *offsetLine = line.c_str();
  char offsetWord[10];
  float x_offset, y_offset, z_offset;
  sscanf(offsetLine, "%s %f %f %f", offsetWord, &x_offset, &y_offset, &z_offset);
  if(strcmp(offsetWord, "OFFSET")) exit(1); //error check
  joint->setXoffset(x_offset);
  joint->setYoffset(y_offset);
  joint->setZoffset(z_offset);
  if(joint->isEndSite()){
    getline(infile, line); // }
    //cout << line << endl;
    return joint;
  }


  //read CHANNELS
  getline(infile,line); // CHANNELS - - - - -
  //cout <<line << endl;
  const char *channelsLine = line.c_str();
  char channelsWord[10];
  int numCh;
  sscanf(channelsLine, "%s %d", channelsWord, &numCh);
  if(strcmp(channelsWord, "CHANNELS")) exit(2); //error check
  joint->setNumChannels(numCh);
  vector<char*> cnames; //list of channel names
  if(numCh == 6){
    char cname1[10], cname2[10], cname3[10], cname4[10], cname5[10], cname6[10];
    sscanf(channelsLine, "%s %d %s %s %s %s %s %s", channelsWord, &numCh,
      cname1, cname2, cname3, cname4, cname5, cname6);
    cnames.assign({cname1, cname2, cname3, cname4, cname5, cname6});
    //print channel names
    for (int i=0; i< cnames.size(); i++){
      cout << cnames[i] << " ";
    } cout << endl;
    joint->setChannelNames(cnames);
  }
  else if(numCh == 3){
    char cname1[10], cname2[10], cname3[10];
    sscanf(channelsLine, "%s %d %s %s %s", channelsWord, &numCh, cname1, cname2, cname3);
    cnames.assign({cname1, cname2, cname3});
    for (int i=0; i< cnames.size(); i++){
      cout << cnames[i] << " ";
    } cout << endl;
    joint->setChannelNames(cnames);
  }
  else exit(3);

  //Next line is either JOINT or End Site follows CHANNELS

  while(getline(infile,line)){ // JOINT --- OR End Site ---
    //cout << line << endl;
    const char *jointLine = line.c_str();
    char firstWord[10];
    sscanf(jointLine, "%s", firstWord);

    //JOINT
    if (strcmp(firstWord, "JOINT") == 0){
      Joint *child = new Joint();
      char jointName[20];
      sscanf(jointLine, "%s %s", firstWord, jointName);
      child->setName(jointName);
      //cout << child->getName() << endl;
      child->setParent(joint);
      //RECURSIVE CALL
      joint->addChild(build_tree(infile,child));

    }

    //End Site
    else if(strcmp(firstWord, "End") == 0){
      Joint *endsite = new Joint();
      endsite->setEndSite();
      endsite->setParent(joint);
      joint->addChild(build_tree(infile, endsite));
    }

    else{ //}
      return joint;
    }

  }
  cout << "should not reach here.\n";
  return joint;
}




















//write tree to file
void write(Joint* joint){
  ofstream outfile;
  outfile.open("output.bvh");

  //write hierarchy
  outfile << "HIERARCHY\n";
  writeHierarchy(outfile, joint);

  //write motion
  //writeMotion(vector motion)
  outfile.close();

}



//arguments: output file stream, root joint
void writeHierarchy(ofstream& outfile, Joint* joint){
  if (joint->isEndSite()){
    outfile << "End Site" << "\n{\n";
    outfile << "OFFSET "
      << joint->getXoffset() << " "
      << joint->getYoffset() << " "
      << joint->getZoffset() << "\n}\n";
      return;
  }
  else{
    if (joint->isRoot()){
    char *jointName = joint->getName();
    outfile << "ROOT " << jointName << "\n{\n";
    }
    else{
      char *jointName = joint->getName();
      outfile << "JOINT " << jointName << "\n{\n";

      //problem here?
      float xof, yof, zof;
      xof = joint->getXoffset();
      yof = joint->getYoffset();
      zof = joint->getZoffset();
      outfile << std::fixed;
      outfile << std::setprecision(6);
      outfile << "OFFSET "  << xof << " " << yof << " " << zof << "\n}\n";
      int numCh = joint->getNumChannels();
      outfile << "CHANNELS " << numCh << " ";
      vector<char*> channels = joint->getChannelNames();
      for(int i=0; i<numCh; i++){
        outfile << channels[i] << " ";
      }
    }
  }

  vector<Joint*> children = joint->getChildren();
  for(int i=0; i<joint->getCount(); i++){
    Joint *child = children[i];
    writeHierarchy(outfile, child);
    outfile << "}\n";
  }



}

//void writeMotion(ofstream& outfile, ???){}




void print_tree(Joint* joint){
  cout << joint->getName() << endl;
  vector<Joint*> children = joint->getChildren();
  for (int i=0; i<joint->getCount(); i++){
    print_tree(children[i]);
  }
}




//
