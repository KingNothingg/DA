#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <set>

class TSuffTree {
private:
    class TNode {
    private:
        std::unordered_map<char, TNode*> children;
        TNode *link;
        int start, *end;
        int ind;
    public:
        friend TSuffTree;
        TNode(TNode *_link, int _start, int *_end, int _ind) : link(_link), start(_start),
                                                               end(_end), ind(_ind) {}
        TNode(TNode *_link, int _start, int *_end) : TNode(_link, _start, _end, -1) {}
    };
private:
    void DeleteHelper(TNode* node) {
        if (node == nullptr) {
            return;
        }
        for (auto it : node->children) {
            DeleteHelper(it.second);
        }
        if (node->ind < -1) {
            delete node->end;
        }
        delete node;
    }
    int EdgeLength(TNode *node) {
        return *(node->end) - (node->start) + 1;
    }
    void ExtendTree(int ind) {
        internalNode = nullptr;
        leafEnd++;
        remainSuff++;
        while (remainSuff > 0) {
            if (activeLength == 0) {
                activeEdge = ind;
            }
            auto findSymb = activeNode->children.find(text[activeEdge]);
            if (findSymb == activeNode->children.end()) {
                activeNode->children.insert(std::make_pair(text[activeEdge], new TNode(root, ind, &leafEnd, ind - remainSuff + 1)));
                if (internalNode != nullptr) {
                    internalNode->link = activeNode;
                    internalNode = nullptr;
                }
            } else {
                TNode *next = findSymb->second;
                int edgeLen = EdgeLength(next);
                if (activeLength >= edgeLen) {
                    activeEdge += edgeLen;
                    activeLength -= edgeLen;
                    activeNode = next;
                    continue;
                }
                if (text[next->start + activeLength] == text[ind]) {
                    if (internalNode != nullptr && activeNode != root) {
                        internalNode->link = activeNode;
                    }
                    activeLength++;
                    break;
                }
                TNode *split = new TNode(root, next->start, new int(next->start + activeLength - 1));
                activeNode->children[text[activeEdge]] = split;
                next->start += activeLength;
                split->children.insert(std::make_pair(text[ind], new TNode(root, ind, &leafEnd, ind - remainSuff + 1)));
                split->children.insert(std::make_pair(text[next->start], next));
                if (internalNode != nullptr) {
                    internalNode->link = split;
                }
                internalNode = split;
            }
            remainSuff--;
            if (activeNode == root && activeLength > 0) {
                activeLength--;
                activeEdge++;
            } else if (activeNode != root) {
                activeNode = activeNode->link;
            }
        }
    }
    void PrintHelper(TNode* p) {
        static int level = 0;
        level++;
        if (p->start == -1) {
            printf("\\_<root>");
        }
        else {
            printf("_<%d, %d>[%d]", p->start, *p->end, p->ind);
            //std::cout << text.substr(p->start, *p->end) << "[" << p->ind << "]";
        }
        for (int i = 0; i < (int)p->children.size(); i++) {
            if (p->children[i]) {
                printf("\n");
                for (int j = 0; j < level; j++)
                    printf("   ");
                printf("\\");
                PrintHelper(p->children[i]);
            }
        }
        level--;
    }

    int doTraversal(TNode *n, int labelHeight, int* maxHeight, std::vector<int> &substringStartIndex) {
        if(n == nullptr) {
            return 0;
        }
        int ret = -1;
        if(n->ind < 0) {
            for (auto it : n->children) {
                TNode *temp = it.second;
                if(temp != nullptr) {
                    ret = doTraversal(temp, labelHeight + EdgeLength(temp), maxHeight, substringStartIndex);
                    if(n->ind == -1) {
                        n->ind = ret;
                    }
                    else if((n->ind == -2 && ret == -3) || (n->ind == -3 && ret == -2) || n->ind == -4) {
                        n->ind = -4;
                        if(*maxHeight < labelHeight) {
                            *maxHeight = labelHeight;
                            substringStartIndex.clear();
                            substringStartIndex.push_back(*(n->end) - labelHeight + 1);
                        }
                        else if(*maxHeight == labelHeight && !substringStartIndex.empty() && substringStartIndex.back() != *(n->end) - labelHeight + 1) {
                            substringStartIndex.push_back(*(n->end) - labelHeight + 1);
                        }
                    }
                }
            }
        }
        else if(n->ind > -1 && n->ind < size1)
            return -2;
        else if(n->ind >= size1)
            return -3;
        return n->ind;
    }



    TNode *root = new TNode(nullptr, -1, new int(-1));
    TNode *internalNode = nullptr;

    std::string text;

    TNode *activeNode = nullptr;
    int activeEdge = -1;
    int activeLength = 0;
    int remainSuff = 0;
    int leafEnd = -1;
    int size1;
public:
    void Print() {
        PrintHelper(root);
    }
    TSuffTree(std::string &str1, std::string &str2) {
        size1 = str1.size();
        text = str1 + '#' + str2 + "$";
        BuildSuffTree();
    };
    void LongestCommonSubstring() {
        int maxHeight = 0;
        std::vector<int> substringStartIndex;
        doTraversal(root, 0, &maxHeight, substringStartIndex);
        std::cout << maxHeight << "\n";
        std::vector<std::string> ans;
        std::string temp;
        for (int i = 0; i < substringStartIndex.size();++i) {
            int k = 0;
            temp.clear();
            for (k = 0; k < maxHeight; k++) {
                temp += text[k + substringStartIndex[i]];
            }
            ans.push_back(temp);
        }
        std::stable_sort(ans.begin(), ans.end());
        for(auto i:ans) {
            std::cout << i << "\n";
        }
    }
    void BuildSuffTree() {
        activeNode = root;
        for (int i = 0; i < text.length(); ++i) {
            ExtendTree(i);
        }
    }
    ~TSuffTree() {
        DeleteHelper(root);
    }
};

