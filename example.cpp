// Example program
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

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



void printLines(ifstream& infile){
  string line;
  getline(infile, line);
  printf("%s\n", line.c_str());
  getline(infile, line);
  printf("%s\n", line.c_str());
  getline(infile, line);
  printf("%s\n", line.c_str());
  getline(infile, line);
  printf("%s\n", line.c_str());
}



int main(int argc, char *argv[])
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

  // const char *str = "root hips";
  //   char result[20];
  //   char name[20];
  //   sscanf(str, "%s%s",result,name );
  //   cout << result << name << endl;

    ifstream infile;
    infile.open(argv[1]);
    printLines(infile);



    return 0;



}
