#include <string.h>
#include <stdio.h>

using namespace std;

class Joint {
  private:
        bool endSite = false;
        char *name;
        vector<Joint*> children;
        Joint *parent = 0;
        float Xoffset, Yoffset, Zoffset;
        int numChannels = 0;
        vector<char*> channelNames;
        float Xposition, Yposition, Zpostition;
        float Zrotation, Yrotation, Xrotation;

  public:
    char* getName(){
        return name;
    }
    void setName(char *n){
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
    vector<char*> getChannelNames(){
      return channelNames;
    }
    void setChannelNames(vector<char*> cnames){
      channelNames = cnames;
    }
    void setEndSite(){
      endSite = true;
    }
    bool isEndSite(){
      return endSite;
    }


};
