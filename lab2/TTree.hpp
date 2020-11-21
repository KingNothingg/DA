#ifndef DALAB2_TAVLTREE_H
#define DALAB2_TAVLTREE_H

#include<cstdlib>
#include<cstring>
#include<iostream>
#include<fstream>
#include<new>

namespace NStd {
    const short MAX_KEY_LENGTH = 256;
    int StringCompare(const char *first, const char *second) {
        while (*first) {
            if (*first != *second) {
                break;
            }
            first++;
            second++;
        }
        return *(const unsigned char *) first - *(const unsigned char *) second;
    }
    struct TNode {
        short balance = 0;
        char* key;
        unsigned long long value;
        TNode *left = nullptr;
        TNode *right = nullptr;
        TNode () : key(nullptr), value(0) { }
        TNode (char* _key, unsigned long long _value) {
            value = _value;
            key = new char[MAX_KEY_LENGTH + 1];
            strncpy(key, _key, sizeof(char)*(MAX_KEY_LENGTH + 1));
        }
        TNode (char *_key, unsigned long long _value, short _balance) {
            value = _value;
            balance = _balance;
            key = new char[MAX_KEY_LENGTH + 1];
            strncpy(key, _key, sizeof(char)*(MAX_KEY_LENGTH + 1));
        }
        ~TNode() {
            delete [] key;
        }
    };

    class TTree {
    public:
        TTree() {}
        void Insert (char* key, unsigned long long value);
        void Delete(char *key);
        void Search(char *key);
        void Save(char *path);
        void Load(char *path);
        ~TTree() {
            DeleteTree(root);
        }
    private:
        TNode *RotateLeft(TNode *node);
        TNode *RotateRight(TNode *node);
        TNode *Balance(TNode *node);
        TNode *BigLeftRotation(TNode *node);
        TNode *BigRightRotation(TNode *node);
        TNode *DeleteHelper(TNode *node, TNode *rNode);
        void SaveHelper(TNode *node, std::ostream& os);
        TNode *LoadHelper(std::istream& is);
        TNode *Insert (char* key, unsigned long long value, TNode* node);
        TNode *Delete(char *key, TNode *node);
        TNode *Search(char *key, TNode *node);
        void DeleteTree(TNode *node);
        TNode *root = nullptr;
    };

    bool stopBalancing;
    TNode* TTree::RotateLeft(TNode *node) {
        TNode *b = node->right;
        if (node == root) {
            root = b;
        }
        node->right = b->left;
        b->left = node;

        return b;
    }
    TNode* TTree::RotateRight(TNode *node) {
        TNode *b = node->left;
        if (node == root) {
            root = b;
        }
        node->left = b->right;
        b->right = node;
        return b;
    }
    TNode *TTree::BigLeftRotation(TNode *node) {
        node->right = RotateRight(node->right);
        node = RotateLeft(node);
        if(node->balance == 1) {
            node->left->balance = 0;
            node->right->balance = -1;
            node->balance = 0;
        } else
        if(node->balance == -1) {
            node->left->balance = 1;
            node->right->balance = 0;
            node->balance = 0;
        } else
        if(node->balance == 0) {
            node->left->balance = 0;
            node->right->balance = 0;
        }
        return node;
    }
    TNode *TTree::BigRightRotation(TNode *node) {
        node->left = RotateLeft(node->left);
        node = RotateRight(node);
        if(node->balance == 1) {
            node->left->balance = 0;
            node->right->balance = -1;
            node->balance = 0;
        } else
        if(node->balance == -1) {
            node->left->balance = 1;
            node->right->balance = 0;
            node->balance = 0;
        } else
        if(node->balance == 0) {
            node->left->balance = 0;
            node->right->balance = 0;
        }
        return node;
    }
    TNode* TTree::Balance(TNode* node) {
        if(node == nullptr) { return nullptr; }
        if(node->balance == 2) {
            if( node->left->balance != -1) {
                node = RotateRight(node);
                if(node->balance == 1) {
                    node->right->balance = 0;
                    node->balance = 0;
                }
                else if(node->balance == 0) {
                    node->right->balance = 1;
                    node->balance = -1;
                }
            }
            else {
                node = BigRightRotation(node);
            }
        }
        else if(node->balance == -2){
            if(node->right->balance != 1) {
                node = RotateLeft(node);
                if (node->balance == -1) {
                    node->left->balance = 0;
                    node->balance = 0;
                }
                else if(node->balance == 0) {
                    node->left->balance = -1;
                    node->balance = 1;
                }
            }
            else {
                node = BigLeftRotation(node);
            }
        }
        return node;
    }

    void TTree::Insert (char *key, unsigned long long value){
        if (root == nullptr) {
            try {
                root = new TNode(key, value);
                std::cout << "OK\n";
            }
            catch (std::bad_alloc &e) {
                std::cout << "ERROR: No memory\n";
                exit(-1);
            }
            return;
        }
        stopBalancing = false;
        root = Insert(key, value, root);
    }
    TNode* TTree::Insert (char *key, unsigned long long value, TNode* node) {
        if (StringCompare(node->key, key) > 0 && node->left == nullptr) {
            try {
                node->left = new TNode(key, value);
                if(!stopBalancing) {
                    node->balance += 1;
                    if(node->balance == 0) {
                        stopBalancing = true;
                    }
                }
                std::cout << "OK\n";
            }
            catch (std::bad_alloc& e) {
                std::cout << "ERROR: No memory\n";
                exit(-1);
            }
        } else
        if (StringCompare(node->key, key) < 0 && node->right == nullptr) {
            try {
                node->right = new TNode(key, value);
                if(!stopBalancing) {
                    node->balance -= 1;
                    if(node->balance == 0) {
                        stopBalancing = true;
                    }
                }
                std::cout << "OK\n";
            }
            catch (std::bad_alloc& e) {
                std::cout << "ERROR: No memory\n";
                exit(-1);
            }
            return node;
        } else
        if (StringCompare(node->key, key) > 0 && node->left != nullptr) {
            node->left = Insert(key, value, node->left);
            if(!stopBalancing) {
                node->balance += 1;
                if(node->balance == 0) {
                    stopBalancing = true;
                }
            }
        } else
        if (StringCompare(node->key, key) < 0 && node->right != nullptr) {
            node->right = Insert(key, value, node->right);
            if (!stopBalancing) {
                node->balance -= 1;
                if (node->balance == 0) {
                    stopBalancing = true;
                }
            }
        } else
        if (StringCompare(node->key, key) == 0) {
            stopBalancing = true;
            std::cout << "Exist\n";
            return node;
        }
        node = Balance(node);
        if(node->balance == 0) {
            stopBalancing = true;
        }
        return node;
    }

    TNode* TTree::DeleteHelper(TNode *node, TNode *rNode) {
        if(rNode->left != nullptr) {
            rNode->left = DeleteHelper(node, rNode->left);
            if(!stopBalancing) {
                rNode->balance -= 1;
                if (rNode->balance == -1) { stopBalancing = true; }
            }
        } else {
            TNode* rightChild = rNode->right;
            memcpy(node->key, rNode->key, sizeof(char) * (strlen(rNode->key) + 1));
            node->value = rNode->value;
            delete rNode;
            rNode = rightChild;
        }
        rNode = Balance(rNode);
        if(rNode != nullptr && rNode->balance != 0) {
            stopBalancing = true;
        }
        return rNode;
    }

    void TTree::Delete(char *key) {
        stopBalancing = false;
        if (root == nullptr) {
            std::cout << "NoSuchWord\n";
            return;
        }
        root = Delete(key, root);

    }

    TNode* TTree::Delete(char *key, TNode *node) {
        if(node == nullptr) {
            stopBalancing = true;
            std::cout << "NoSuchWord\n";
            return nullptr;
        }
        if (StringCompare(node->key, key) < 0) {
            node->right = Delete(key, node->right);
            if(!stopBalancing) {
                node->balance += 1;
                if(node->balance == 1) { stopBalancing = true; }
            }
        } else
        if (StringCompare(node->key, key) > 0) {
            node->left = Delete(key, node->left);
            if(!stopBalancing) {
                node->balance -= 1;
                if(node->balance == -1) { stopBalancing = true; }
            }
        } else
        if (StringCompare(node->key, key) == 0) {
            TNode *leftChild = node->left;
            TNode *rightChild = node->right;
            if(leftChild == nullptr && rightChild == nullptr) {
                delete node;
                std::cout << "OK\n";
                return nullptr;
            }
            if(rightChild == nullptr) {
                delete node;
                std::cout << "OK\n";
                return leftChild;
            }
            if(leftChild == nullptr) {
                delete node;
                std::cout << "OK\n";
                return rightChild;
            }
            node->right = DeleteHelper(node, rightChild);
            if(!stopBalancing) {
                node->balance += 1;
                if(node->balance == 1) { stopBalancing = true; }
            }
            std::cout << "OK\n";
        }
        node = Balance(node);
        if(node->balance != 0) {
            stopBalancing = true;
        }
        return node;
    }

    void TTree::Search(char *key) {
        TNode* result = Search(key, root);
        if(result != nullptr) {
            std::cout << "OK: " << result->value << "\n";
        }
        else {
            std::cout << "NoSuchWord\n";
        }
    }

    TNode *TTree::Search(char *key, TNode *node) {
        if(node == nullptr) {
            return nullptr;
        }
        if(StringCompare(node->key, key) == 0) {
            return node;
        }
        else if(StringCompare(node->key, key) < 0) {
            return Search(key, node->right);
        }
        else if(StringCompare(node->key, key) > 0) {
            return Search(key, node->left);
        } else {
            return nullptr;
        }
    }

    void TTree::DeleteTree(TNode *node) {
        if (node != nullptr) {
            DeleteTree(node->left);
            DeleteTree(node->right);
            delete node;
        }
        return;
    }
    bool stopSaving;
    void TTree::Save(char *path) {
        stopSaving = false;
        std::ofstream os;
        os.open(path, std::ios::binary | std::ios::out);
        if(os.fail()) {
            std::cerr << "ERROR: Unable to open file in write mode\n";
            stopSaving = true;
            return;
        }
        SaveHelper(root, os);
        os.close();
        if (!stopSaving) {
            std::cout << "OK\n";
        }
    }
    void TTree::SaveHelper(TNode *node, std::ostream &os) {
        if(stopSaving) { return; }
        short keyLength;
        if (node == nullptr) { return; }
        keyLength = strlen(node->key);
        os.write((char*)&keyLength, sizeof(short));
        if (os.fail()) {
            std::cerr << "ERROR: Unable to save in file\n";
            stopSaving = true;
            return;
        }
        os.write(node->key, sizeof(char) * keyLength);
        if (os.fail()) {
            std::cerr << "ERROR: Unable to save in file\n";
            stopSaving = true;
            return;
        }
        os.write((char*)&(node->value), sizeof(unsigned long long));
        if (os.fail()) {
            std::cerr << "ERROR: Unable to save in file\n";
            stopSaving = true;
            return;
        }
        char nodeBalance;
        if (node->balance == -1) {
            nodeBalance = '0';
        }
        if (node->balance == 0) {
            nodeBalance = '1';
        }
        if (node->balance == 1) {
            nodeBalance = '2';
        }
        os.write((char*)&(nodeBalance), sizeof(char));
        if (os.fail()) {
            std::cerr << "ERROR: Unable to save in file\n";
            stopSaving = true;
            return;
        }
        bool hasLeftChild = node->left != nullptr;
        bool hasRightChild = node->right != nullptr;
        os.write((char*)&hasLeftChild, sizeof(bool));
        os.write((char*)&hasRightChild, sizeof(bool));
        if(hasLeftChild){
            SaveHelper(node->left, os);
        }
        if(hasRightChild){
            SaveHelper(node->right, os);
        }
    }
    bool stopLoading;
    void TTree::Load(char *path) {
        stopLoading = false;
        std::ifstream is;
        is.open(path, std::ios::binary | std::ios::in);
        if(is.fail()) {
            std::cerr << "ERROR: Unable to open file read mode\n";
            stopLoading = true;
            return;
        }
        DeleteTree(root);
        root = nullptr;
        root = LoadHelper(is);
        is.close();
        if(!stopLoading) {
            std::cout << "OK\n";
        } else {
            std::cout << "ERROR: Can't load\n";
        }
    }

    TNode *TTree::LoadHelper(std::istream &is) {
        if(stopLoading) { return nullptr; }
        TNode *nNode;
        short keyLength = 0;
        is.read((char*)&keyLength, sizeof(short));
        if (is.gcount() == 0) {
            return nullptr;
            stopLoading = true;
        }
        if (is.fail()) {
            std::cerr << "ERROR: Unable to load from file\n";
            stopLoading = true;
            return nullptr;
        }
        char nodeBalance;
        char *nodeKey = new char[MAX_KEY_LENGTH + 1];
        unsigned long long nodeValue;
        is.read(nodeKey, keyLength);
        nodeKey[keyLength] = '\0';
        if (is.fail()) {
            std::cerr << "ERROR: Unable to load from file\n";
            stopLoading = true;
            return nullptr;
        }
        nodeKey[keyLength] = '\0';
        is.read((char*)&nodeValue, sizeof(unsigned long long));
        if (is.fail()) {
            std::cerr << "ERROR: Unable to load from file\n";
            stopLoading = true;
            return nullptr;
        }

        is.read((char*)&nodeBalance, sizeof(char));
        if (is.fail()) {
            std::cerr << "ERROR: Unable to load from file\n";
            stopLoading = true;
            return nullptr;
        }
        if (nodeBalance > '2' || nodeBalance < '0') {
            std::cerr << "ERROR: Wrong file format\n";
            stopLoading = true;
            return nullptr;
        }
        nNode = new TNode(nodeKey, nodeValue,nodeBalance - '0' - 1);
        bool hasLeftChild;
        bool hasRightChild;
        is.read((char*)&hasLeftChild, sizeof(bool));
        if (is.fail()) {
            std::cerr << "ERROR: Unable to load from file\n";
            stopLoading = true;
            return nullptr;
        }
        is.read((char*)&hasRightChild, sizeof(bool));
        if (is.fail()) {
            std::cerr << "ERROR: Unable to load from file\n";
            stopLoading = true;
            return nullptr;
        }
        if(hasLeftChild) {
            nNode->left = LoadHelper(is);
        } else { nNode->left = nullptr; }
        if(hasRightChild) {
            nNode->right = LoadHelper(is);
        } else { nNode->right = nullptr; }
        delete [] nodeKey;
        return nNode;
    }
}

#endif //DALAB2_TAVLTREE_H
