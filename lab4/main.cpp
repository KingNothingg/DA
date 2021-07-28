#include<string>
#include<vector>
#include<unordered_map>
#include<algorithm>
#include<iostream>


void BadCharacterRule(const std::vector<std::string>& pattern, std::unordered_map<std::string, int>& badCharacterShift) {
    int size = pattern.size();
    for(int i = 0; i < size; ++i) {
        badCharacterShift[pattern[i]] = i;
    }
}

void ZFunction(const std::vector<std::string>& pattern, std::vector<int>& zResult) {
    int size = pattern.size();
    std::vector<int> zFunc(size, 0);
    for(int i = 1, left = 0, right = 0; i < size; ++i) {
        if (i <= right) {
            zFunc[i] = std::min(right - i + 1, zFunc[i-left]);
        }
        while(i + zFunc[i] < size && pattern[zFunc[i]] == pattern[i + zFunc[i]]) {
            ++zFunc[i];
        }
        if(i + zFunc[i] - 1 > right) {
            left = i;
            right = i + zFunc[i] - 1;
        }
    }
    zResult = zFunc;
}

void NFunc(const std::vector<std::string>& pattern, std::vector<int>& nResult) {
    std::vector<std::string> revPattern(pattern.rbegin(), pattern.rend());
    std::vector<int> zFunc;
    ZFunction(revPattern, zFunc);
    std::vector<int> nFunc(zFunc.size());
    for(int i = 0; i < revPattern.size(); ++i) {
        nFunc[i] = zFunc[revPattern.size() - i - 1];
    }
    nResult = nFunc;
}

void GoodSuffixRule(const std::vector<std::string>& pattern, std::vector<int>& bigLFunc, std::vector<int>& smallLFunc) {
    std::vector<int> n;
    NFunc(pattern, n);
    std::vector<int> l(n.size());
    std::vector<int> L(pattern.size()+1);
    int j = 0;
    for(int i = 0; i < pattern.size()-1; ++i)
    {
        if(n[i] != 0)
        {
            j = pattern.size()-n[i];
            l[j] = i;
        }
        if(n[i] == i+1) { L[pattern.size()-i-1] = i+1; }
        else { L[pattern.size()-i-1] = L[pattern.size()-i]; }
    }
    bigLFunc = L;
    smallLFunc = l;
}

int main(){
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    std::vector<std::string> text;
    std::vector<std::string> pattern;
    std::vector<std::pair<int,int>> wordsID;
    std::string tempWord;
    int stringInd = 1, wordInd = 1;
    std::pair<int,int> tempPair;
    std::unordered_map<std::string, int> badCharacterShift;
    char c = getchar();
    while (c > 0) {

        if (c == '\n') {
            if(!tempWord.empty()) {
                pattern.push_back(tempWord);
            }
            break;
        }
        if (c == '\t' || c == ' '){
            if (!tempWord.empty()) {
                pattern.push_back(tempWord);
                tempWord.clear();
            }
        }
        else {
            if ('A' <= c and c <= 'Z') {
                c = c + 'a' - 'A';
            }
            tempWord.push_back(c);
        }
        c = getchar();
    }
    tempWord.clear();
    c = getchar();
    while(c > 0) {
        if (c == '\n') {
            if(!tempWord.empty()) {
                text.push_back(tempWord);
                tempWord.clear();
                tempPair.first = stringInd;
                tempPair.second = wordInd;
                wordsID.push_back(tempPair);
            }
            stringInd++;
            wordInd = 1;
        } else if (c == '\t' || c == ' ') {
            if(!tempWord.empty()) {
                text.push_back(tempWord);
                tempWord.clear();
                tempPair.first = stringInd;
                tempPair.second = wordInd;
                wordsID.push_back(tempPair);
                wordInd++;
            }
        } else {
            if ('A' <= c and c <= 'Z') {
                c = c + 'a' - 'A';
            }
            tempWord.push_back(c);
        }
        c = getchar();
    }
    if(!tempWord.empty()) {
        text.push_back(tempWord);
        tempPair.first = stringInd;
        tempPair.second = wordInd;
        wordsID.push_back(tempPair);
    }
    if(pattern.empty() || text.empty()) {
        return 0;
    }
    BadCharacterRule(pattern, badCharacterShift);
    std::vector<int> bigLFunc;
    std::vector<int> smallLFunc;
    GoodSuffixRule(pattern, bigLFunc, smallLFunc);
    std::vector<int> entryVec;
    int k = pattern.size() - 1;
    while (k < text.size()) {
        int i = pattern.size() - 1;
        int j = k;
        while ((i >= 0) && (pattern[i] == text[j])) {
            --i;
            --j;
        }
        if (i == -1) {
            entryVec.push_back(k-pattern.size()+1);
            if (pattern.size() > 2) {
                k += pattern.size() - bigLFunc[1];
            } else {
                ++k;
            }
        } else {
            int goodSuffixShift = 1;
            int bCShift = 0;
            if(badCharacterShift.find(text[j]) != badCharacterShift.end()) {
                bCShift = badCharacterShift[text[j]];
            }
            if (i != pattern.size() - 1) {
                if (smallLFunc[i + 1] > 0) {
                    goodSuffixShift = pattern.size() - smallLFunc[i + 1] - 1;
                } else {
                    goodSuffixShift = pattern.size() - smallLFunc[i + 1];
                }
            }
            k += std::max({goodSuffixShift, i - bCShift, static_cast<int>(1)});
        }
    }
    for(long long i : entryVec) {
        std::cout << wordsID[i].first << ", " << wordsID[i].second << '\n';
    }
}

