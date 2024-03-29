#include <iostream>
#include <chrono>
int main() {
    long long n, m;
    std::cin >> n >> m;

    if (n <= m){
        std::cout << 0 << '\n';
        return 0;
    }

    std::string str = std::to_string(n);
    long long dp = 0;
    using duration_t = std::chrono::microseconds;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    for (int i = 1; i <= str[0] - '0'; ++i){
        if (i % m == 0){
            dp += 1;
        }
    }

    long long curMin = 10;
    std::string curStr;
    curStr += str[0];

    for (int i = 1; i < str.length(); ++i){
        curStr += str[i];
        long long min = curMin;
        curMin *= 10;
        long long max = std::stoll(curStr);
        if (max % m != 0){
            max = max - (max % m);
        }
        if (min % m != 0){
            min = min + m - (min % m);
        }
        if (max >= min){
            dp += (max / m) - (min / m) + 1;
        }
    }

    if (n % m == 0){
    //    std::cout << dp - 1 << '\n';
    } else {
       // std::cout << dp << '\n';
    }
    end = std::chrono::system_clock::now();
    auto time = std::chrono::duration_cast<duration_t>(end - start).count();
    std::cout << time << "ms";
    return 0;
}