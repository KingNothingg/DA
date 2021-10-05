#ifndef DA6_BIGINT_H
#define DA6_BIGINT_H

/*#include <vector>
#include <cstdint>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>


class TBigInt {
private:
    static const int BASE = 10e5;
    static const int DIGIT_LENGTH = 5;
    std::vector<int> number;
public:
    size_t Size() const {
        return number.size();
    }

    TBigInt() = default;

    TBigInt(const size_t &size) : number(size, 0) {};

    TBigInt(const std::string &str) {
        for (int i=(int)str.length(); i>0; i-=DIGIT_LENGTH)
            if (i < DIGIT_LENGTH)
                number.push_back (atoi (str.substr (0, i).c_str()));
            else
                number.push_back (atoi (str.substr (i-DIGIT_LENGTH, DIGIT_LENGTH).c_str()));
        while (number.size() > 1 && number.back() == 0)
            number.pop_back();
    }
    TBigInt& operator = (const TBigInt &other) {
        number = other.number;
        return *this;
    }

    void Print() {
        int size = number.size();
        for (int i = size - 1; i >= 0; --i) {
            if(i == size - 1) {
                std::cout << number[i];
            } else {
                std::cout << std::setfill('0') << std::setw(DIGIT_LENGTH) << number[i];
            }
            if(i != 0) {
                std::cout << "_";
            }
        }
    }

    friend TBigInt operator + (TBigInt &lhs, TBigInt &rhs) {
        if (lhs.Size() < rhs.Size()) {
            return rhs + lhs;
        }
        int loopSize = std::max(lhs.Size(), rhs.Size());
        int k = 0;
        TBigInt res(loopSize + 2);
        for (int i = 0; i < loopSize; ++i) {
            int tempSum = lhs.number[i] + (i < rhs.Size()? rhs.number[i] : 0) + k;
            k = tempSum / BASE;
            res.number[i] = tempSum % BASE;
        }
        if (k > 0) {
            res.number[loopSize] = k;
        }
        while (res.number.size() > 1 and res.number.back() == 0) {
            res.number.pop_back();
        }
        return res;
    }

    friend TBigInt operator - (TBigInt &lhs, TBigInt &rhs) {
        if (lhs < rhs) {
            throw std::runtime_error("error");
        }
        int k = 0;
        TBigInt res(lhs);
        for (int i = 0; i < rhs.Size() || k; ++i) {
            res.number[i] -= k + (i < rhs.Size() ? rhs.number[i] : 0);
            k = res.number[i] < 0;
            if (k != 0) {
                res.number[i] += BASE;
            }
        }
        while (res.number.size() > 1 and res.number.back() == 0) {
            res.number.pop_back();
        }
        return res;
    }

    friend TBigInt operator * (TBigInt &lhs, TBigInt &rhs) {
        TBigInt res(lhs.Size() + rhs.Size());
        int k = 0;
        for (int i = 0; i < lhs.Size(); ++i) {
            for (int j = 0; j < rhs.Size() || k; ++j) {
                long long curr = res.number[i + j] + lhs.number[i] * 1ll * rhs.number[j] + k;
                res.number[i + j] = int (curr % BASE);
                k =  int ( curr / BASE);
            }
        }
        while (res.number.size() > 1 and res.number.back() == 0) {
            res.number.pop_back();
        }
        return res;
    }

    friend TBigInt operator * (TBigInt &lhs, const int &rhs) {
        TBigInt res = lhs;
        int k = 0;
        for (int i = 0; i < res.Size() || k; ++i) {
            if (i == res.Size()) {
                res.number.push_back(0);
            }
            long long curr = k + res.number[i] * 1ll * rhs;
            res.number[i] = int (curr % BASE);
            k = (int) (curr / BASE);
        }
        while (res.number.size() > 1 and res.number.back() == 0) {
            res.number.pop_back();
        }
        return res;
    }

    friend TBigInt operator / (TBigInt &lhs, TBigInt &rhs) {
        if(rhs.number[0] == 0 && rhs.Size() == 1) {
            throw std::runtime_error("error");
        }
        TBigInt d(std::to_string(BASE/(rhs.number.back() + 1)));
        TBigInt div(1);
        TBigInt mod(rhs.Size());
        lhs = lhs * d;
        rhs = rhs * d;
        for (int j = lhs.Size() - rhs.Size() - 1; j >= 0; j--) {
            long long q = (lhs.number[j + rhs.Size()]*BASE + lhs.number[j + rhs.Size() - 1]) / rhs.number.back();
            long long r = (lhs.number[j + rhs.Size()]*BASE + lhs.number[j + rhs.Size() - 1]) % rhs.number.back();
            while (r < BASE && (q == BASE || q * rhs.number[rhs.Size() - 2] > BASE * r + lhs.number[rhs.Size() + j - 2])) {
                q--;
                r += rhs.number.back();
            }
            if (div.number.back() == 0) {
                div.number[0] = q;
            } else {
                div.number.push_back(q);
            }
            long long temp = std::pow(BASE, j);
            q *= temp;
            TBigInt tmp(rhs*q);
            lhs = lhs - tmp;
        }
        std::reverse(div.number.begin(), div.number.end());
        if (div.Size() == 1 && div.number[0] == 0) {
            div.number[0] = 1;
        }
        return div;
    }

    friend TBigInt operator / (TBigInt &lhs, const int &rhs) {
        if (rhs == 0) {
            throw std::runtime_error("error");
        }
        int k = 0;
        TBigInt res(lhs);
        for (int i = res.Size() - 1; i >= 0; --i) {
            long long curr = res.number[i] + k *1ll * BASE;
            res.number[i] = int (curr/rhs);
            k = int (curr % rhs);
        }
        while (res.number.size() > 1 and res.number.back() == 0) {
            res.number.pop_back();
        }
        return res;
    }

    friend TBigInt operator ^ (TBigInt &lhs, TBigInt &rhs) {
        if ((lhs.Size() == 1 && lhs.number[0] == 0) && (rhs.Size() == 1 && rhs.number[0] == 0)) {
            throw std::runtime_error("error");
        }
        TBigInt decrement("1");
        TBigInt num(lhs);
        TBigInt res("1");
        while (!(rhs.Size() == 1 && rhs.number[0] == 0)) {
            if ((rhs.number[0] % 10) % 2 == 0) {
                num = num * num;
                rhs = rhs / 2;
            } else {
                res = res * num;
                rhs = rhs - decrement;
            }
        }
        while (res.number.size() > 1 and res.number.back() == 0) {
            res.number.pop_back();
        }
        return res;
    }

    friend std::ostream& operator << (std::ostream &os, const TBigInt &num) {
        printf ("%d", num.Size() == 0 ? 0 : num.number.back());
        for (int i = (int)num.Size() - 2; i >= 0; --i)
            printf ("%01d", num.number[i]);
        return os;
    }

    friend std::istream& operator >> (std::istream &is, TBigInt &num) {
        std::string str;
        is >> str;
        int zeros = 0;
        while (zeros < str.size() && str[zeros] == 0) {
            ++zeros;
        }
        if (zeros == str.size()) {
            num = TBigInt("0");
        } else {
            num = TBigInt(str.substr(zeros, str.size() - zeros + 1));
        }
        return is;
    }

    friend bool operator < (const TBigInt &lhs, const TBigInt &rhs) {
        if (lhs.Size() != rhs.Size()) {
            return lhs.Size() < rhs.Size();
        }
        for (int i = lhs.Size() - 1; i >=0 ; --i) {
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
            return lhs.Size() > rhs.Size();
        }
        for (int i = lhs.Size() - 1; i >=0 ; --i) {
            if (lhs.number[i] != rhs.number[i]) {
                return false;
            }
        }
        return true;
    }

};
*/

#endif //DA6_BIGINT_H
