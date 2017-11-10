#include "joint.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <regex>


using namespace std;

//FUNCTION PROTOTYPES
void read(string fileName, Joint *root, int *numFramesPointer, float *frameTimePointer, vector<vector<float>> &frames);
void read_hierarchy(ifstream& infile, Joint *root);
Joint* build_tree(ifstream& infile, Joint *joint);
void read_motion(ifstream& infile, vector<vector<float>> &frames);
vector<float> split(string str);
void write(Joint* joint, vector<vector<float>> frames, int numFrames, float frameTime);
void write_hierarchy(ofstream& outfile, Joint* joint);
void write_motion(ofstream& outfile, vector<vector<float>> frames, int numFrames, float frameTime);

void list_tree(Joint* joint);
void print_motion(vector<vector<float>> frames);



void read(string fileName, Joint *root, int *numFramesPointer, float *frameTimePointer, vector<vector<float>> &frames){
  ifstream infile;
  infile.open(fileName);
  if (!infile) {
        cout << "Unable to open file. Exiting.\n";
        exit(2); //terminate with error
  }

  //read hierarchy
  cout << "Reading...\n";
  read_hierarchy(infile, root);
  list_tree(root);
  //read motion
  string line;
  getline(infile, line); //MOTION
  getline(infile, line); //Frames: 20
  const char *framesLine = line.c_str();
  char w1[10];
  sscanf(framesLine, "%s %d", w1, numFramesPointer);
  getline(infile, line); // Frame Time: 0.012312
  const char *frameTimeLine = line.c_str();
  char w2[10];
  sscanf(frameTimeLine, "%s %s %f", w1, w2, frameTimePointer);
  read_motion(infile, frames);
  //print_motion(frames);
  infile.close();
  cout << "Done.\n-------------------------------\n\n";
}


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

    //return infile; //????

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
  sscanf(offsetLine, "%s %f %f %f",
    offsetWord, &x_offset, &y_offset, &z_offset);
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
  //vector<char*> cnames; //list of channel names
  if(numCh == 6){
    char cname1[20], cname2[20], cname3[20], cname4[20], cname5[20], cname6[20];
    sscanf(channelsLine, "%s %d %s %s %s %s %s %s",
      channelsWord, &numCh, cname1, cname2, cname3, cname4, cname5, cname6);
    string cn1=cname1, cn2=cname2, cn3=cname3, cn4=cname4, cn5=cname5, cn6= cname6;
    joint->set6Channels(cname1, cname2, cname3, cname4, cname5, cname6);


    //joint->setChannelNames(cnames);
  }
  else if(numCh == 3){
    char cname1[20], cname2[20], cname3[20];
    sscanf(channelsLine, "%s %d %s %s %s",
     channelsWord, &numCh, cname1, cname2, cname3);
    string cn1=cname1, cn2=cname2, cn3=cname3;
    joint->set3Channels(cname1, cname2, cname3);

    //joint->setChannelNames(cnames);
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



void read_motion(ifstream& infile, vector<vector<float>> &frames){
  string line;
  while (getline(infile, line)){
    frames.push_back(split(line));
  }

}

vector<float> split(string str){
  regex re("(-)?\\d+((\\.)?\\d+)?");
  sregex_iterator begin(str.begin(), str.end(), re);
  sregex_iterator end;
  vector<float> output;
  int n =0;
  for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        //convert string to float
        std::string::size_type sz;
        float d = stof(match_str, &sz);
        //add to output vector
        output.push_back(d);
        n++;
  }
  return output;
}


















void write(Joint* root, vector<vector<float>> frames, int numFrames, float frameTime){
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

}

//arguments: output file stream, root joint
void write_hierarchy(ofstream& outfile, Joint* joint){
  if (joint->isEndSite()){
    outfile << "End Site" << "\n{\n";
    outfile << "OFFSET "
      << joint->getXoffset() << " "
      << joint->getYoffset() << " "
      << joint->getZoffset() << "\n";
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
    }

    float xof, yof, zof;
    xof = joint->getXoffset();
    yof = joint->getYoffset();
    zof = joint->getZoffset();
    outfile << std::fixed;
    outfile << std::setprecision(6);
    outfile << "OFFSET "  << xof << " " << yof << " " << zof << "\n";
    int numCh = joint->getNumChannels();
    outfile << "CHANNELS " << numCh << " ";
    for(int i=0; i<numCh; i++){
      outfile << joint->getChannelName(i) << " ";
    } outfile << "\n";

  }

  vector<Joint*> children = joint->getChildren();
  for(int i=0; i<joint->getCount(); i++){
    Joint *child = children[i];
    write_hierarchy(outfile, child);
    outfile << "}\n";
  }
}

void write_motion(ofstream& outfile, vector<vector<float>> frames, int numFrames, float frameTime){
  outfile << "MOTION\n";
  outfile << "Frames: " << numFrames << "\n";
  outfile << fixed;
  outfile << setprecision(6);
  outfile << "Frame Time: " << frameTime << "\n";
  int f = frames.size();
  for (int i=0; i<f; i++){
    int n = frames[i].size();
    for (int j=0; j<n; j++){
      outfile << frames[i][j] << " ";
    }
    outfile << "\n";
  }
}















int ID = 0;
void list_tree(Joint* joint){
  //add joint pointers to a list and assign joint IDs
  if (joint->isEndSite() == false){
    joint->setID(ID);
    ID++;
  }
  // cout << joint->getID() << " "
  //   << joint->getName() << " "
  //   << joint->getXoffset() << " "
  //   << joint->getYoffset() << " "
  //   << joint->getZoffset() << endl;
  vector<Joint*> children = joint->getChildren();
  for (int i=0; i<joint->getCount(); i++){
    list_tree(children[i]);
  }

}


void print_motion(vector<vector<float>> frames){
  int f = frames.size();
  for (int i=0; i<f; i++){
    int n = frames[i].size();
    for (int j=0; j<n; j++){
      cout << frames[i][j] << " ";
    }
    cout << "\n\n";
  }
}



















//
