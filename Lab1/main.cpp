
#include "TPair.hpp"
#include "TString.hpp"
#include <iostream>
#include <stdio.h>
#include "TVector.hpp"


void CountingSort(NStd::TVector<NStd::TPair<NStd::TString, unsigned long long int>> &arr, const size_t size, const int digit) {
    NStd::TVector<unsigned int> count(10,0);
    NStd::TVector<NStd::TPair<NStd::TString, unsigned long long int>> result(arr.Size());
    if(arr[0].first[size - digit] == '+' || arr[0].first[size - digit] == '-') {
        return;
    }
    for (int i = 0; i < arr.Size(); ++i) {
        count[arr[i].first[size - digit] - '0']++;
    }
    for(int i = 1; i < 10; ++i) {
        count[i] += count[i - 1];
    }
    for(int i = arr.Size() - 1; i >= 0; i--) {
        result[count[arr[i].first[size - digit] - '0'] - 1] = arr[i];
        count[arr[i].first[size - digit] - '0']--;
    }
    for(int i = 0; i < arr.Size(); ++i) {
        arr[i] = result[i];
    }
}

size_t Get_Max(NStd::TVector<NStd::TPair<NStd::TString, unsigned long long int>> const &arr) {
    size_t max = 0;
    for(int i = 0; i < arr.Size(); ++i) {
        if(arr[max].first.Size() < arr[i].first.Size()) {
            max = i;
        } else
        if(arr[max].first.Size() > arr[i].first.Size()) {
            continue;
        } else {
            for(int j = 0; j < arr[max].first.Size(); ++j) {
                if(arr[i].first[j] == '+' || arr[i].first[j] == '-') {
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

void AddZero(NStd::TVector<NStd::TPair<NStd::TString, unsigned long long int>> &arr, const size_t max) {
    size_t max_size = arr[max].first.Size();
    NStd::TString zero, plus;
    zero = "0";
    plus = "+";
    for( int i = 0; i < arr.Size(); ++i) {
        size_t curr_size = arr[i].first.Size();
        if( curr_size < max_size) {
            arr[i].first[0] = '0';
            for(int j = 0; j < max_size - curr_size - 1; ++j) {
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

void RemoveZero(NStd::TVector<NStd::TPair<NStd::TString, unsigned long long int>> &arr) {
    NStd::TString output;
    for(int i = 0; i < arr.Size(); ++i) {
        if(arr[i].first[1] == '0') {
            output = "+";
            int j;
            for(j = 2; j < arr[i].first.Size(); ++j) {
                if(arr[i].first[j] != '0') {
                    break;
                }
            }
            for(int k = j; k < arr[i].first.Size(); ++k) {
                output + arr[i].first[k];
            }
            arr[i].first = output;
        }
    }
}

void RadixSort(NStd::TVector<NStd::TPair<NStd::TString, unsigned long long int>> &arr) {
    size_t max = Get_Max(arr);
    AddZero(arr, max);
    for(int i = 1; i <= arr[max].first.Size(); ++i) {
        CountingSort(arr, arr[max].first.Size(), i);
    }
    RemoveZero(arr);
}

int main() {
    //std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    
    NStd::TVector<NStd::TPair<NStd::TString, unsigned long long int>> arr;
    NStd::TPair<NStd::TString, unsigned long long int> p;
    char * temp = new char[17];
    while(std::cin >> temp >> p.second){
        p.first = temp;
        arr.PushBack(p);
    }
    if(arr.Size() > 1) {
        RadixSort(arr);
    }
    for(int i = 0; i < arr.Size(); ++i){
        std::cout << arr[i].first << "\t" << arr[i].second << std::endl;
    }
    delete [] temp;
}
// "+<1-3sym>-<3 sym>-<7sym>"