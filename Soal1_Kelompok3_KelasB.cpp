#include <iostream>
#include <string>
using namespace std;

struct Node {
    string name;
    bool isFile;
    Node **children;
    int childCount;
    int capacity;
    Node(string, bool);
    void addChild(Node*);
    Node* findChild(string);
    bool removeChild(string);
    ~Node();
};

Node::Node(string name, bool isFile) : name(name), isFile(isFile), childCount(0), capacity(2) {
    children = new Node*[capacity];
}

void Node::addChild(Node* child) {
    if (childCount == capacity) {
        capacity *= 2;
        Node** newChildren = new Node*[capacity];
        for (int i = 0; i < childCount; ++i) {
            newChildren[i] = children[i];
        }
        delete[] children;
        children = newChildren;
    }
    children[childCount++] = child;
}

Node* Node::findChild(string childName) {
    for (int i = 0; i < childCount; ++i) {
        if (children[i]->name == childName) {
            return children[i];
        }
    }
    return nullptr;
}

bool Node::removeChild(string childName) {
    for (int i = 0; i < childCount; ++i) {
        if (children[i]->name == childName) {
            delete children[i];
            for (int j = i; j < childCount - 1; ++j) {
                children[j] = children[j + 1];
            }
            childCount--;
            return true;
        }
    }
    return false;
}

Node::~Node() {
    for (int i = 0; i < childCount; ++i) {
        delete children[i];
    }
    delete[] children;
}

struct FileSystem {
    Node* root;
    Node* currentDir;
    Node** path;
    int pathLength;
    FileSystem();
    void createDefaultFileSystem();
    void printTree(Node*, int);
    void listDirectory(Node*);
    Node* search(Node*, string);
    string getPath(Node*, Node*);
    void processCommand(string);
    ~FileSystem();
};

FileSystem::FileSystem() {
    root = new Node("root", false);
    currentDir = root;
    path = new Node*[100];
    pathLength = 0;
    createDefaultFileSystem();
}

void FileSystem::createDefaultFileSystem() {
    Node* lib = new Node("lib", false);
    lib->addChild(new Node("LiquidCrystal_I2C.zip", true));
    lib->addChild(new Node("ServoESP32-master.zip", true));
    root->addChild(lib);

    Node* src = new Node("src", false);
    Node* smartFan = new Node("Smart_Fan", false);
    smartFan->addChild(new Node("config.h", true));
    smartFan->addChild(new Node("my_wifi.h", true));
    smartFan->addChild(new Node("Smart_Fan.ino", true));
    smartFan->addChild(new Node("wifi_communicator.h", true));
    src->addChild(smartFan);
    root->addChild(src);
}

void FileSystem::printTree(Node* root, int depth) {
    for (int i = 0; i < depth; i++) {
        cout << "  ";
    }
    cout << (root->isFile ? "" : "|-- ") << root->name << endl;
    for (int i = 0; i < root->childCount; ++i) {
        printTree(root->children[i], depth + 1);
    }
}

void FileSystem::listDirectory(Node* dir) {
    if (dir->isFile) {
        cout << "Ini adalah file, bukan direktori." << endl;
        return;
    }
    for (int i = 0; i < dir->childCount; ++i) {
        cout << (dir->children[i]->isFile ? "File: " : "Dir: ") << dir->children[i]->name << endl;
    }
}

Node* FileSystem::search(Node* root, string name) {
    if (root->name == name) {
        return root;
    }
    for (int i = 0; i < root->childCount; ++i) {
        Node* result = search(root->children[i], name);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

string FileSystem::getPath(Node* root, Node* target) {
    if (root == target) {
        return root->name;
    }
    for (int i = 0; i < root->childCount; ++i) {
        string path = getPath(root->children[i], target);
        if (!path.empty()) {
            return root->name + "/" + path;
        }
    }
    return "";
}

void FileSystem::processCommand(string command) {
    if (command == "exit") {
        exit(0);
    } 
    else if (command.substr(0, 5) == "mkdir") {
        string dirName = command.substr(6);
        currentDir->addChild(new Node(dirName, false));
    } 
    else if (command.substr(0, 5) == "touch") {
        string fileName = command.substr(6);
        currentDir->addChild(new Node(fileName, true));
    } 
    else if (command.substr(0, 6) == "rmdir") {
        string dirName = command.substr(7);
        if (!currentDir->removeChild(dirName)) {
            cout << "Direktori tidak ditemukan." << endl;
        }
    }
    else if (command.substr(0, 2) == "rm") {
        string fileName = command.substr(3);
        if (!currentDir->removeChild(fileName)) {
            cout << "File tidak ditemukan." << endl;
        }
    } 
    else if (command.substr(0, 2) == "ls") {
        listDirectory(currentDir);
    } 
    else if (command.substr(0, 4) == "tree") {
        printTree(root, 0);
    } 
    else if (command.substr(0, 5) == "find ") {
        string name = command.substr(5);
        Node* result = search(root, name);
        if (result != nullptr) {
            cout << "Path: " << getPath(root, result) << endl;
        } 
        else {
            cout << "Tidak ditemukan." << endl;
        }
    } 
    else if (command.substr(0, 2) == "cd") {
        string dirName = command.substr(3);
        if (dirName == "..") {
            if (pathLength > 0) {
                pathLength--;
                currentDir = (pathLength > 0) ? path[pathLength - 1] : root;
            }
        } 
        else {
            Node* nextDir = currentDir->findChild(dirName);
            if (nextDir != nullptr && !nextDir->isFile) {
                path[pathLength++] = nextDir;
                currentDir = nextDir;
            } 
            else {
                cout << "Direktori tidak ditemukan." << endl;
            }
        }
    } 
    else {
        cout << "Perintah tidak dikenali." << endl;
    }
}

FileSystem::~FileSystem() {
    delete[] path;
    delete root;
}

int main() {
    FileSystem fs;
    string command;

    while (true) {
        cout << "/";
        for (int i = 0; i < fs.pathLength; ++i) {
            cout << fs.path[i]->name << "/";
        }
        cout << "> ";

        getline(cin, command);
        fs.processCommand(command);
    }

    return 0;
}
