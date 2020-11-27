#ifndef DALAB2_TAVLTREE_H
#define DALAB2_TAVLTREE_H

#include<cstdlib>
#include<cstring>
#include<cstdio>
#include<iostream>
#include<fstream>
#include<new>

namespace NStd {
    const short MAX_KEY_LENGTH = 256;
    int StringCompare(const char *first, const char *second) {
        if(first != nullptr && second != nullptr) {
            while (*first) {
                if (*first != *second) {
                    break;
                }
                first++;
                second++;
            }
            return *(const unsigned char *) first - *(const unsigned char *) second;
        } else return 0;
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
            memcpy(key, _key, sizeof(char)*(MAX_KEY_LENGTH + 1));
        }
        TNode (char *_key, unsigned long long _value, short _balance) {
            value = _value;
            balance = _balance;
            key = new char[MAX_KEY_LENGTH + 1];
            memcpy(key, _key, sizeof(char)*(MAX_KEY_LENGTH + 1));
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
        void SaveHelper(TNode *node, FILE *os);
        TNode *LoadHelper(FILE *is);
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
        if (node == nullptr) {
            node = new TNode(key, value);
            std::cout << "OK\n";
            return node;
        }
        short comparedStrings = StringCompare(node->key, key);
        if (comparedStrings > 0) {
            node->left = Insert(key, value, node->left);
            if(!stopBalancing) {
                node->balance += 1;
                if(node->balance == 0) {
                    stopBalancing = true;
                }
            }
        } else
        if (comparedStrings < 0) {
            node->right = Insert(key, value, node->right);
            if (!stopBalancing) {
                node->balance -= 1;
                if (node->balance == 0) {
                    stopBalancing = true;
                }
            }
        } else {
            stopBalancing = true;
            std::cout << "Exist\n";
            return node;
        }
        if(node->balance == 2 || node->balance == -2) {
            node = Balance(node);
            if(node->balance == 0) {
                stopBalancing = true;
            }
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
        if(rNode == nullptr) {
            return rNode;
        }
        if(rNode->balance == 2 || rNode->balance == -2) {
            rNode = Balance(rNode);
            if(rNode->balance != 0) {
                stopBalancing = true;
            }
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
        short comparedStrings = StringCompare(node->key, key);
        if (comparedStrings < 0) {
            node->right = Delete(key, node->right);
            if(!stopBalancing) {
                node->balance += 1;
                if(node->balance == 1) { stopBalancing = true; }
            }
        } else
        if (comparedStrings > 0) {
            node->left = Delete(key, node->left);
            if(!stopBalancing) {
                node->balance -= 1;
                if(node->balance == -1) { stopBalancing = true; }
            }
        } else {
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
        if(node->balance == 2 || node->balance == -2) {
            node = Balance(node);
            if(node->balance != 0) {
                stopBalancing = true;
            }
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
        short comparedStrings = StringCompare(node->key, key);
        if(comparedStrings == 0) {
            return node;
        }
        else if(comparedStrings < 0) {
            return Search(key, node->right);
        }
        else if(comparedStrings > 0) {
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
    }
    bool stopSaving;
    void TTree::Save(char *path) {
        stopSaving = false;
        FILE *os;
        if((os = fopen(path, "wb")) == nullptr) {
            std::cerr << "ERROR: Unable to open file in write mode\n";
            stopSaving = true;
            return;
        }
        SaveHelper(root, os);
        fclose(os);
        if (!stopSaving) {
            std::cout << "OK\n";
        }
    }
    void TTree::SaveHelper(TNode *node, FILE *os) {
        if(stopSaving) { return; }
        short keyLength;
        if (node == nullptr) {
            keyLength = -1;
            fwrite(&keyLength, sizeof(short), 1, os);
            return;
        }
        keyLength = strlen(node->key);
        fwrite(&keyLength, sizeof(short), 1, os);
        fwrite(node->key, sizeof(char), keyLength, os);
        fwrite(&node->value, sizeof(unsigned long long), 1, os);
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
        fwrite(&nodeBalance, sizeof(char), 1, os);
        SaveHelper(node->left, os);
        SaveHelper(node->right, os);
    }
    bool stopLoading;
    void TTree::Load(char *path) {
        stopLoading = false;
        FILE *is;
        if((is = fopen(path, "rb")) == nullptr) {
            std::cerr << "ERROR: Unable to open file read mode\n";
            stopLoading = true;
            return;
        }
        DeleteTree(root);
        root = LoadHelper(is);
        fclose(is);
        if(!stopLoading) {
            std::cout << "OK\n";
        }
    }

    TNode *TTree::LoadHelper(FILE *is) {
        if(stopLoading) { return nullptr; }
        TNode *nNode;
        short keyLength = 0;
        if (fread(&keyLength, sizeof(short), 1, is) == 0) {
            return nullptr;
        }
        if (keyLength == -1) {
            return nullptr;
        }
        char nodeBalance;
        char *nodeKey = new char[MAX_KEY_LENGTH + 1];
        unsigned long long nodeValue;
        fread(nodeKey, sizeof(char), keyLength, is);
        nodeKey[keyLength] = '\0';
        fread(&nodeValue, sizeof(unsigned long long), 1, is);
        fread(&nodeBalance, sizeof(char), 1, is);
        if (nodeBalance > '2' || nodeBalance < '0') {
            std::cerr << "ERROR: Wrong file format\n";
            stopLoading = true;
            return nullptr;
        }
        nNode = new TNode(nodeKey, nodeValue,nodeBalance - '0' - 1);
        nNode->left = LoadHelper(is);
        nNode->right = LoadHelper(is);
        delete [] nodeKey;
        return nNode;
    }
}

#endif
