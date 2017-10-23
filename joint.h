#include <string>
#include <stdio.h>

using namespace std;

class Joint {
  private:
        bool endSite = false;
        char name[20];
        vector<Joint*> children;
        Joint *parent = 0;
        float Xoffset, Yoffset, Zoffset;
        int numChannels = 0;
        vector<string> channelNames;
        float Xposition, Yposition, Zpostition;
        float Zrotation, Yrotation, Xrotation;

  public:
    char* getName(){
        return name;
    }
    void setName(char n[10]){
        strcpy(name, n);
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
    Joint* getChild(char *name){
        Joint *child;
        for(int i=0; i<children.size(); i++){
          if(strcmp(children[i]->getName(), name) == 0){
            return children[i];
          }
        }
        return 0;
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

    //Offset getters and setters
    float getXoffset(){
      return Xoffset;
    }
    float getYoffset(){
      return Yoffset;
    }
    float getZoffset(){
      return Zoffset;
    }
    void setXoffset(float xof){
      Xoffset = xof;
    }
    void setYoffset(float yof){
      Yoffset = yof;
    }
    void setZoffset(float zof){
      Zoffset = zof;
    }

    //Channels
    int getNumChannels(){
      return numChannels;
    }
    void setNumChannels(int n){
      numChannels = n;
    }
    string getChannelName(int index){
      return channelNames[index];
    }
    void set6Channels(string cname1, string cname2, string cname3, string cname4, string cname5, string cname6){
      channelNames.assign({cname1, cname2, cname3, cname4, cname5, cname6});
    }
    void set3Channels(string cname1, string cname2, string cname3){
      channelNames.assign({cname1, cname2, cname3});
    }

    void setEndSite(){
      endSite = true;
    }
    bool isEndSite(){
      return endSite;
    }


};
