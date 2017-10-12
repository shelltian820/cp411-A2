using namespace std;

//class Joint {
//    private:
//        string name;
//        vector<Joint> children;
//        Joint parent = NULL;
//        
//    string getName(){
//        return name;
//    }
//    vector<Joint> getChildren(){
//        return children;
//    }
//    int getCount(){
//        return children.size();
//    }
//    Joint getParent(){
//        return parent;
//    }
//    void addChild(Joint child){
//        children.push_back(child);
//    }
//    void setParent(Joint p){
//        parent = p;
//    }
//    bool isRoot(){
//        if (parent = NULL){
//            return true;
//        } else {
//            return false;
//        }
//    }
//};


void read(string filename, Joint root);






void read(string filename, Joint root){
    ifstream infile;
    infile.open(filename);
    if (!infile) {
          cout << "Unable to open file. Exiting.\n";
          exit(2); //terminate with error
    }else{
        cout << "blah\n";    
    }
    infile.close();
}



void write(){
    
}