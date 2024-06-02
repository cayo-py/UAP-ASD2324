#include <iostream>
#include <queue>  
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

void Node::addChild(Node *child) {
    if (childCount == capacity) {
        capacity *= 2;
        Node **newChildren = new Node*[capacity];
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
    Node *root;
    Node *currentDir;
    Node **path;
    int pathLength;
    FileSystem();
    void createDefaultFileSystem();
    void printTree(Node*, int);
    void listDirectory(Node*);
    Node *search(Node*, string);
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
    Node *Pribadi = new Node("Pribadi", false);
    Node *Kuliah = new Node("Kuliah", false);
    root->addChild(new Node("pinguin.zip", true));
    root->addChild(Pribadi);
    root->addChild(Kuliah);
    Kuliah->addChild(new Node("KTMS.jpg", true));
    Kuliah->addChild(new Node("Cayo-4x6-Red.jpg", true));
    Pribadi->addChild(new Node("KTP.jpg", true));
}

void FileSystem::printTree(Node *root, int depth) {
    for (int i = 0; i < depth; i++) {
        cout << "    ";
    }
    cout << (root->isFile ? "|- " : "|-- ") << root->name << endl;
    for (int i = 0; i < root->childCount; ++i) {
        printTree(root->children[i], depth + 1);
    }
}

void FileSystem::listDirectory(Node *dir) {
    if (dir->isFile) {
        cout << "Ini adalah file, bukan direktori." << endl;
        return;
    }
    for (int i = 0; i < dir->childCount; ++i) {
        cout << dir->children[i]->name << "    ";
        if (i==dir->childCount-1) cout << endl << endl;
    }
}


Node* FileSystem::search(Node *root, string name) {
    queue<Node*> toExplore;
    toExplore.push(root);

    while (!toExplore.empty()) {
        Node* current = toExplore.front();
        toExplore.pop();

        if (current->name == name) {
            return current;
        }

        for (int i = 0; i < current->childCount; ++i) {
            toExplore.push(current->children[i]);
        }
    }
    return nullptr;
}



string FileSystem::getPath(Node *root, Node *target) {
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

    else if (command.substr(0, 5) == "mkdir") { // untuk membuat direktori
        string dirName = command.substr(6);
        currentDir->addChild(new Node(dirName, false));
    }

    else if (command.substr(0, 5) == "touch") { // untuk membuat file
        string fileName = command.substr(6);
        currentDir->addChild(new Node(fileName, true));
    } 

    else if (command.substr(0, 5) == "rmdir") { // untuk menghapus direktori
        string dirName = command.substr(6);
        if (!currentDir->removeChild(dirName)) {
            cout << "Direktori tidak ditemukan." << endl;
        }
    }

    else if (command.substr(0, 2) == "rm") { // untuk menghapus file
        string fileName = command.substr(3);
        if (!currentDir->removeChild(fileName)) {
            cout << "File tidak ditemukan." << endl;
        }
    } 

    else if (command.substr(0, 2) == "ls") { // untuk menampilkan isi direktori
        listDirectory(currentDir);
    } 

    else if (command.substr(0, 4) == "tree") { // untuk menampilkan tree direktori
        printTree(root, 0);
    } 

    else if (command.substr(0, 5) == "find ") { // untuk menemukan file/direktori
        string name = command.substr(5);
        Node* result = search(root, name);
        if (result != nullptr) {
            cout << "Path: " << getPath(root, result) << endl;
        } 
        else {
            cout << "Tidak ditemukan." << endl;
        }
    } 

    else if (command.substr(0, 2) == "cd") { // untuk menuju ke direktori tertentu
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
        cout << "Perintah invalid" << endl;
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
