// Example program
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Joint {
    public:
        string name;
        vector<Joint*> children;
        Joint *parent = 0;

    string getName(){
        return name;
    }
    void setName(string n){
        name = n;
    }
    vector<Joint*> getChildren(){
        return children;
    }
    int getCount(){
        return children.size();
    }
    void addChild(Joint *child){
        children.push_back(child);
    }
    Joint* getParent(){
        return parent;
    }
    void setParent(Joint* p){
        parent = p;
    }
    bool isRoot(){
        if (parent == NULL){
            return true;
        } else {
            return false;
        }
    }
};
int main()
{
  // Joint *hips = new Joint();
  // hips->setName("Hips");
  //
  // Joint *leftleg = new Joint();
  // leftleg->setName("leftleg");
  // leftleg->setParent(hips);
  //
  // Joint *rightleg = new Joint();
  // rightleg->setName("rightleg");
  // rightleg->setParent(hips);
  //
  // //std::cout << hips->getName() << endl;
  // //if (hips->parent) std::cout << hips->getParent() << endl;
  // hips->addChild(leftleg);
  // hips->addChild(rightleg);
  // std::cout << hips->getChildren()[0]->getParent()->getName() << endl; //print first child's parent name
  // std::cout << hips->getChildren()[1]->getParent()->getName() << endl; //print second child's parent name

  const char *str = "root hips";
    char result[20];
    char name[20];
    sscanf(str, "%s%s",result,name );
    cout << result << name << endl;
    return 0;



}





ifstream infile;
infile.open(filename);
bool hierarchy = false;
bool motion = false;
bool open_bracket = false;
bool closed_bracket = false;

string line;
while (getline(infile,line)){
  //convert string to char*
  const char *str = line.c_str();
  char beginning[10];
  sscanf(str, "%s", beginning);
  if (strcmp(beginning, "HIERARCHY") == 0 ){
    hierarchy = true;
    motion = false;
    continue;
  }
  if (hierarchy){
    //read joint data
    //scan at first word of line
    if (strcmp(beginning, "ROOT") == 0){
      char name[20];
      sscanf(str, "%s%s", beginning, name);
      root->setName(name);
    }
    else if(strcmp(beginning, "JOINT") == 0){

    }
    else if(strcmp(beginning, "OFFSET") == 0){

    }
    else if(strcmp(beginning, "CHANNELS") == 0){

    }
    else if(strcmp(beginning, "JOINT") == 0){

    }
  }
  if (motion){
    //read motion data
  }



}
