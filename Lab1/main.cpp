
#include "TTriplet.hpp"
#include "TString.hpp"
#include <iostream>
#include <stdio.h>
#include "TVector.hpp"
#include <chrono>

void CountingSort(NStd::TVector<NStd::TTriplet<NStd::TString, unsigned long long int, NStd::TString *>> &arr, const unsigned long long size, const int digit) {
    NStd::TVector<unsigned long long int> count(10,0);
    NStd::TVector<NStd::TTriplet<NStd::TString, unsigned long long int, NStd::TString *>> result(arr.Size());
    if(arr[0].first[size - digit] < '0' || arr[0].first[size - digit] > '9') {
        return;
    }
    for (unsigned long long int i = 0; i < arr.Size(); ++i) {
        ++count[arr[i].first[size - digit] - '0'];
    }
    for(unsigned long long int i = 1; i < 10; ++i) {
        count[i] += count[i - 1];
    }
    for(long long int i = arr.Size() - 1; i >= 0; --i) {
        result[count[arr[i].first[size - digit] - '0'] - 1] = arr[i];
        --count[arr[i].first[size - digit] - '0'];
    }
    for(unsigned long long int i = 0; i < arr.Size(); ++i) {
        arr[i] = result[i];
    }
}

void AddZero(NStd::TVector<NStd::TTriplet<NStd::TString, unsigned long long int, NStd::TString *>> &arr) {
    int max_size = 16;
    NStd::TString zero, plus;
    zero = "0";
    plus = "+";
    for(unsigned long long int i = 0; i < arr.Size(); ++i) {
        unsigned long long curr_size = arr[i].first.Size();
        if( curr_size < max_size) {
            arr[i].first[0] = '0';
            for(unsigned long long int j = 0; j < max_size - curr_size - 1; ++j) {
                zero + arr[i].first;
                arr[i].first = zero;
                zero = "0";
            }
            plus + arr[i].first;
            arr[i].first = plus;
            plus = "+";
        }
    }
}

void RadixSort(NStd::TVector<NStd::TTriplet<NStd::TString, unsigned long long int, NStd::TString *>> &arr) {
    AddZero(arr);
    for(unsigned long long int i = 1; i <= 16; ++i) {
        CountingSort(arr, 16, i);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    NStd::TVector<NStd::TTriplet<NStd::TString, unsigned long long int, NStd::TString *>> arr;
    NStd::TTriplet<NStd::TString, unsigned long long int, NStd::TString *> p;
    NStd::TVector<NStd::TString> value;
    char * temp = new char[17];
    while(std::cin >> temp >> p.second){
        p.first = temp;
        value.PushBack(temp);
        arr.PushBack(p);
    }
    for(int i = 0; i < value.Size(); ++i) {
        arr[i].third = &value[i];
    }
    auto start = std::chrono::steady_clock::now();
    if(arr.Size() > 1) {
        RadixSort(arr);
    } 
    auto finish = std::chrono::steady_clock::now();
    for(unsigned long long int i = 0; i < arr.Size(); ++i){
        std::cout << *arr[i].third << '\t' << arr[i].second << '\n';
    }
    auto dur = finish - start;
    std::cerr << "stable sort from std " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << " ms" << std::endl;
    delete [] temp;
}
