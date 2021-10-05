#include "SuffTree.cpp"

int main() {
    std::string a, b;
    std::cin >> a >> b;
    if (a.empty() || b.empty()) {
        exit(0);
    }
    TSuffTree tree(a, b);
    tree.LongestCommonSubstring();
}
