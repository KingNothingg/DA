#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <cctype>

class TBigInt {
private:
    static const long int BASE = 1e5;
    static const int DIGIT_LENGTH = 5;
    std::vector<int> number;
public:
    size_t Size() const {
        return number.size();
    }

    TBigInt() = default;

    TBigInt(int num) {
        if (!num) {
            number.push_back(0);
        }
        else {
            while (num) {
                number.push_back(num % BASE);
                num /= BASE;
            }
        }
    }

    explicit TBigInt(const size_t &size) : number(size, 0) {};

    explicit TBigInt(const std::string &str) {
        number.clear();
        for (int i = (int)str.length(); i > 0; i -= DIGIT_LENGTH)
            if (i < DIGIT_LENGTH)
                number.push_back (atoi(str.substr (0, i).c_str()));
            else
                number.push_back (atoi(str.substr (i-DIGIT_LENGTH, DIGIT_LENGTH).c_str()));
        while (number.size() > 1 && number.back() == 0)
            number.pop_back();
    }
    TBigInt operator = (const TBigInt &other) {
        this->number = other.number;
        return *this;
    }

    friend TBigInt operator + (TBigInt &lhs, TBigInt &rhs) {
        if (lhs.Size() < rhs.Size()) {
            return rhs + lhs;
        }
        TBigInt result = lhs;
        int carry = 0;
        for (int i = 0; i < std::max(result.Size(), rhs.Size()) || carry; ++i) {
            if (i == result.Size()) {
                result.number.emplace_back(0);
            }
            result.number[i] += carry + (i < rhs.Size() ? rhs.number[i] : 0);
            carry = result.number[i] / BASE;
            result.number[i] %= BASE;
        }
        while (result.number.size() > 1 and result.number.back() == 0) {
            result.number.pop_back();
        }
        return result;
    }

    friend TBigInt operator - (TBigInt &lhs, TBigInt &rhs) {
        if (lhs < rhs) {
            throw std::runtime_error("error");
        }
        int carry = 0;
        TBigInt result(lhs);
        for (int i = 0; i < static_cast<int>(rhs.Size()) || carry; ++i) {
            result.number.at(i) -= carry + (i < static_cast<int>(rhs.Size()) ? rhs.number[i] : 0);
            carry = result.number[i] < 0;
            if (carry != 0) {
                result.number.at(i) += BASE;
            }
        }
        while (result.number.size() > 1 and result.number.back() == 0) {
            result.number.pop_back();
        }
        return result;
    }



    friend TBigInt operator * (TBigInt &lhs, TBigInt &rhs) {
        TBigInt result(lhs.Size() + rhs.Size());
        int carry = 0;
        for (int i = 0; i < lhs.Size(); ++i) {
            carry = 0;
            for (int j = 0; j < int(rhs.Size()) || carry; ++j) {
                long long curr = result.number[i + j] + carry + lhs.number[i] * 1ll * (j < (int)rhs.Size() ? rhs.number[j] : 0);
                result.number[i + j] = int(curr % BASE);
                carry = int(curr / BASE);
            }
        }
        while (result.number.size() > 1 and result.number.back() == 0) {
            result.number.pop_back();
        }
        return result;
    }

    friend TBigInt operator * (TBigInt &lhs, const int &rhs) {
        TBigInt result(lhs);
        int carry = 0;
        for (int i = 0; i < result.Size() || carry; ++i) {
            if (i == result.Size()) {
                result.number.push_back(0);
            }
            long long curr = carry + result.number[i] * 1ll * rhs;
            result.number[i] = int (curr % BASE);
            carry = (int) (curr / BASE);
        }
        while (result.number.size() > 1 and result.number.back() == 0) {
            result.number.pop_back();
        }
        return result;
    }

    friend TBigInt operator / (TBigInt &lhs, TBigInt &rhs) {
        if(lhs < rhs) {
            return TBigInt("0");
        }
        if((rhs.Size() == 1) && rhs.number[0] == 0) {
            throw std::runtime_error("error");
        }
        int norm = BASE / (rhs.number.back() + 1);
        TBigInt lCopy = lhs * norm;
        TBigInt rCopy = rhs * norm;
        TBigInt q(lCopy.Size());
        TBigInt r;
        for (int i = lCopy.Size() - 1; i >= 0; --i) {
            r = r * BASE;
            TBigInt increment(std::to_string(lCopy.number[i]));
            r = r + increment;
            int s1 = r.Size() <= rCopy.Size() ? 0 : r.number[rCopy.Size()];
            int s2 = r.Size() <= rCopy.Size() - 1 ? 0 : r.number[rCopy.Size() - 1];
            int d = static_cast<int>((static_cast<int>(s1) * BASE + s2) / rCopy.number.back());
            TBigInt  tmp = rCopy * d;
            while (tmp > r){
                tmp = tmp - rCopy;
                d--;
            }
            r = r - tmp;
            q.number[i] = d;
        }
        while (q.number.size() > 1 and q.number.back() == 0) {
            q.number.pop_back();
        }
        return q;
    }

    friend TBigInt operator / (TBigInt &lhs, const int &rhs) {
        if (rhs == 0) {
            throw std::runtime_error("error");
        }
        int carry = 0;
        TBigInt result(lhs);
        for (int i = result.Size() - 1; i >= 0; --i) {
            long long curr = result.number[i] + carry *1ll * BASE;
            result.number[i] = int (curr/rhs);
            carry = int (curr % rhs);
        }
        while (result.number.size() > 1 and result.number.back() == 0) {
            result.number.pop_back();
        }
        return result;
    }

    friend TBigInt operator ^ (TBigInt &lhs, TBigInt &rhs) {
        if ((lhs.Size() == 1 && lhs.number[0] == 0) && (rhs.Size() == 1 && rhs.number[0] == 0)) {
            throw std::runtime_error("error");
        }
        TBigInt decrement("1");
        TBigInt num(lhs);
        TBigInt result("1");
        while (!(rhs.Size() == 1 && rhs.number[0] == 0)) {
            if ((rhs.number[0] % 10) % 2 == 0) {
                num = num * num;
                rhs = rhs / 2;
            } else {
                result = result * num;
                rhs = rhs - decrement;
            }
        }
        while (result.number.size() > 1 and result.number.back() == 0) {
            result.number.pop_back();
        }
        return result;
    }

    friend std::ostream& operator << (std::ostream &os, const TBigInt &num) {
        int num_len = num.Size();
        os << num.number.back();
        for (int i = num_len - 2; i >= 0; --i) {
            os << std::setfill('0') << std::setw(DIGIT_LENGTH) << num.number[i];
        }
        return os;
    }

    friend std::istream& operator >> (std::istream &is, TBigInt &num) {
        std::string str;
        is >> str;
        num = TBigInt(str);
        return is;
    }

    friend bool operator < (const TBigInt &lhs, const TBigInt &rhs) {
        if (lhs.Size() != rhs.Size()) {
            return lhs.Size() < rhs.Size();
        }
        for (int i = lhs.Size() - 1; i >= 0 ; --i) {
            if (lhs.number[i] != rhs.number[i]) {
                return lhs.number[i] < rhs.number[i];
            }
        }
        return false;
    }

    friend bool operator > (const TBigInt &lhs, const TBigInt &rhs) {
        if (lhs.Size() != rhs.Size()) {
            return lhs.Size() > rhs.Size();
        }
        for (int i = lhs.Size() - 1; i >=0 ; --i) {
            if (lhs.number[i] != rhs.number[i]) {
                return lhs.number[i] > rhs.number[i];
            }
        }
        return false;
    }

    friend bool operator == (const TBigInt &lhs, const TBigInt &rhs) {
        if (lhs.Size() != rhs.Size()) {
            return false;
        }
        for (int i = lhs.Size() - 1; i >=0 ; --i) {
            if (lhs.number[i] != rhs.number[i]) {
                return false;
            }
        }
        return true;
    }

};

int main() {
    TBigInt a, b;
    char action;
    using duration_t = std::chrono::microseconds;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    while(!std::cin.eof()) {
        std::cin >> a >> b >> action;
        if (std::cin.eof()) {
            break;
        }
        if (action == '+') {
            //std::cout << a + b << "\n";
            a = a + b;
        } else if (action == '-') {
            try {
                a = a -b;
                //std::cout << a - b << '\n';
            } catch (...) {
                //std::cout << "Error\n";
            }
        } else if (action == '*') {
            //std::cout << a * b << '\n';
            a = a * b;
        } else if (action == '/') {
            try {
                //std::cout << a / b << '\n';
                a = a / b;
            } catch (...) {
                //std::cout << "Error\n";
            }
        } else if (action == '^') {
            try {
                //std::cout << (a ^ b) << '\n';
            } catch (...) {
                //std::cout << "Error\n";
            }
        } else if (action == '<') {
            if (a < b) {
                std::cout << "true\n";
            } else {
                std::cout << "false\n";
            }
        } else if (action == '>') {
            if (a > b) {
                std::cout << "true\n";
            } else {
                std::cout << "false\n";
            }
        } else if (action == '=') {
            if (a == b) {
                std::cout << "true\n";
            } else {
                std::cout << "false\n";
            }
        }
    }
    end = std::chrono::system_clock::now();
    auto time = std::chrono::duration_cast<duration_t>(end - start).count();
    std::cout << time << "ms";
}