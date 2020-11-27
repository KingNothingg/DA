#include "TTree.hpp"
#include<iostream>
#include<cctype>
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    
    NStd::TTree root;
    char *cmd = new char[NStd::MAX_KEY_LENGTH + 1];
    char *key = new char[NStd::MAX_KEY_LENGTH + 1];
    short keyLen = 0;
    unsigned long  long  value;
    char *fileName = new char[NStd::MAX_KEY_LENGTH + 1];
    while(std::cin >> cmd) {
        if(NStd::StringCompare(cmd, "+") == 0) {
            std::cin >> key >> value;
            keyLen = strlen(key);
            for(int i = 0; i < keyLen; ++i) {
                key[i] = (char)tolower(key[i]);
            }
            root.Insert(key, value);
        } else
        if(NStd::StringCompare(cmd, "-") == 0) {
            std::cin >> key;
            keyLen = strlen(key);
            for(int i = 0; i < keyLen; ++i) {
                key[i] = (char)tolower(key[i]);
            }
            root.Delete(key);
        } else
        if(NStd::StringCompare(cmd, "!") == 0) {
            std::cin >> cmd >> fileName;
            if(NStd::StringCompare(cmd, "Save") == 0) {
                root.Save(fileName);
            }
            else {
                root.Load(fileName);
            }
        }
        else {
            keyLen = strlen(cmd);
            for(int i = 0; i < keyLen; ++i) {
                cmd[i] = (char)tolower(cmd[i]);
            }
            root.Search(cmd);
        }
    }
    delete [] fileName;
    delete [] cmd;
    delete [] key;
}
