#ifndef TString_HPP
#define TString_HPP

#include<string.h>
#include<iostream>

namespace NStd {
    class TString {
        int size;
        char * buffer;
    public:
        TString();
        TString(const TString &);
        TString(const char *);
        ~TString();
        int Size() const;
        char& operator[] (unsigned int);
        void operator =(const TString&);
        void operator =(const char *);
        void operator +(const TString&);
        void operator +(const char);
        
        friend std::ostream & operator<<(std::ostream &, const TString &);
    };
    TString::TString() {
        buffer = nullptr;
        size = 0;
    }
    TString::TString(const TString&s) {
        size = s.size;
        buffer = new char[size];
        for(int i = 0; i < size; ++i){
            buffer[i] = s.buffer[i];
        }
    } 
    TString::TString(const char *p) {
        int i = 0;
        const char * t = p;

        while(*p++) {
            ++i;
        }

        buffer = new char[i];
        int j = 0;

        for(j = 0;*t;t++,++j) {
            buffer[j] = *t;
        }
        size = j;
    }
    TString::~TString() {
        delete[] buffer;
    }
    int TString::Size() const {
        if(buffer == nullptr) {
            return 0;
        }
        else {
            return size;
        }
    }
    char & TString::operator[] (unsigned int x) {
        return buffer[x];
    }
    void TString::operator =(const TString&s) {
        delete [] buffer;
        size = s.size;
        buffer = new char[size];
        for(int i = 0; i < s.Size();++i) {
            buffer[i] = s.buffer[i];
        }
    }
    /*void TString::operator =(const char* s){
        delete [] buffer;
        size = strlen(s);
        buffer = new char [size];
        for(int i = 0; i < size; ++i){
            buffer[i] = s[i];
        }
    }*/
    void TString::operator =(const char* s){
        delete [] buffer;
        size = strlen(s);
        buffer = new char [size + 1];
        strcpy(buffer, s);
    }

    void TString::operator +(const char c){
        char * temp = new char[size];
        for(int i = 0; i < Size(); ++i){
            temp[i] = buffer[i];
        }
        ++size;
        delete [] buffer;
        buffer = new char[size];
        for(int i = 0; i < Size() - 1; ++i){
            buffer[i] = temp[i];
        }
        buffer[size - 1] = c;
        delete [] temp;
    }
    void TString::operator +(const TString& s){
        char * temp = new char[size];
        int t = Size();
        for(int i = 0; i < Size(); ++i) {
            temp[i] = buffer[i];
        }
        int tsize = size + s.size;
        delete [] buffer;
        buffer = new char[tsize];
        for(int i = 0; i < Size(); ++i) {
            buffer[i] = temp[i];
        }
        for(int i = t, j = 0; j < s.Size(); ++i,++j) {
            buffer[i] = s.buffer[j];
        }
        size+=s.size;
        delete [] temp;
    }

    std::ostream & operator<<(std::ostream &os, const TString &s) {
        for(int i = 0; i < s.Size(); ++i) {
            os << s.buffer[i];
        }
        return os;
    }
}
#endif 