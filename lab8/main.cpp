#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

bool ReverseCompare(int &lhs, int &rhs) {
    return lhs > rhs;
}

bool ValidTriangle (int &a, int &b, int &c) {
    if (a < (b + c) && b < (a + c) && c < (a + b)) {
        return true;
    }
    return false;
}

double Area (int &a, int &b, int &c) {
    double p = 0.5 * (a + b + c);
    return sqrt(p) * sqrt(p - a) * sqrt(p - b) * sqrt(p - c);
}

int main() {
    int n, s = 0, a = 0, b = 0, c = 0;
    std::vector<int> data;
    using duration_t = std::chrono::microseconds;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    double maxArea = 0.0, currArea = 0.0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> s;
        data.push_back(s);
    }
    std::sort(data.begin(), data.end(), ReverseCompare);
    for (int i = 1; i < data.size() - 1; ++i) {
        if (data.size() < 3) {
            break;
        }
        if (ValidTriangle(data[i - 1], data[i], data[i + 1])) {
            currArea = Area(data[i - 1], data[i], data[i + 1]);
            if (currArea > maxArea) {
                maxArea= currArea;
                a = data[i + 1];
                b = data[i];
                c = data[i - 1];
            }
        }
    }
    if (maxArea == 0.0) {
        //std::cout << "0\n";
    }
    else {
        printf("%.3f\n", maxArea);
        //std::cout << a << ' ' << b << ' ' << c  << '\n';
    }
    end = std::chrono::system_clock::now();
    auto time = std::chrono::duration_cast<duration_t>(end - start).count();
    std::cout << time << "ms";
}