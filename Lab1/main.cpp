
#include "TTriplet.hpp"
#include "TString.hpp"
#include <iostream>
#include <stdio.h>
#include "TVector.hpp"


void CountingSort(NStd::TVector<NStd::TTriplet<NStd::TString, unsigned long long int>> &arr, const unsigned long long size, const int digit) {
    NStd::TVector<unsigned long long int> count(10,0);
    NStd::TVector<NStd::TTriplet<NStd::TString, unsigned long long int>> result(arr.Size());
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

unsigned long long Get_Max(NStd::TVector<NStd::TTriplet<NStd::TString, unsigned long long int>> const &arr) {
    unsigned long long max = 0;
    for(unsigned long long int i = 0; i < arr.Size(); ++i) {
        if(arr[max].first.Size() < arr[i].first.Size()) {
            max = i;
        } else
        if(arr[max].first.Size() > arr[i].first.Size()) {
            continue;
        } else {
            for(unsigned long long int j = 0; j < arr[max].first.Size(); ++j) {
                if(arr[i].first[j] < '0' || arr[i].first[j] > '9') {
                    continue;
                }
                if((arr[i].first[j] - '0') > (arr[max].first[j] - '0')) {
                    max = i;
                    break;
                } else
                if((arr[i].first[j] - '0') < (arr[max].first[j] - '0')) {
                    break;
                }
            }
        }
    }
    return max;
}

void AddZero(NStd::TVector<NStd::TTriplet<NStd::TString, unsigned long long int>> &arr, const unsigned long long max) {
    unsigned long long max_size = arr[max].first.Size();
    NStd::TString zero, plus;
    zero = "0";
    plus = "+";
    for(unsigned long long int i = 0; i < arr.Size(); ++i) {
        unsigned long long curr_size = arr[i].first.Size();
        if( curr_size < max_size) {
            arr[i].first[0] = '0';
            arr[i].changed = max_size - curr_size;
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

void RemoveZero(NStd::TVector<NStd::TTriplet<NStd::TString, unsigned long long int>> &arr) {
    NStd::TString output;
    for(unsigned long long i = 0; i < arr.Size(); ++i) {
        if (arr[i].changed != 0) {
            output = "+";
            for(int j = 1 + arr[i].changed; j < arr[i].first.Size(); ++j) {
                output + arr[i].first[j];
            }
            arr[i].first = output;
        }
    }
}

void RadixSort(NStd::TVector<NStd::TTriplet<NStd::TString, unsigned long long int>> &arr) {
    unsigned long long max = Get_Max(arr);
    AddZero(arr, max);
    for(unsigned long long int i = 1; i <= arr[max].first.Size(); ++i) {
        CountingSort(arr, arr[max].first.Size(), i);
    }
    RemoveZero(arr);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    NStd::TVector<NStd::TTriplet<NStd::TString, unsigned long long int>> arr;
    NStd::TTriplet<NStd::TString, unsigned long long int> p;
    char * temp = new char[17];
    while(std::cin >> temp >> p.second){
        p.first = temp;
        arr.PushBack(p);
    }
    if(arr.Size() > 1) {
        RadixSort(arr);
    }
    for(unsigned long long int i = 0; i < arr.Size(); ++i){
        std::cout << arr[i].first << "\t" << arr[i].second << std::endl;
    }
    delete [] temp;
}
