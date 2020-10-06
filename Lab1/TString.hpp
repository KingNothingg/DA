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
        buffer = new char[size + 1];
        strcpy(buffer, s.buffer);
    } 
    TString::TString(const char *p) {
        size = strlen(p);
        buffer = new char[size + 1];
        strcpy(buffer, p);
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
        buffer = new char[size + 1];
        strcpy(buffer, s.buffer);
    }
    void TString::operator =(const char* s){
        delete [] buffer;
        size = strlen(s);
        buffer = new char [size + 1];
        strcpy(buffer, s);
    }
    
    void TString::operator +(const char c){
        char * temp = new char[size + 1];
        strncpy(temp, buffer, size + 1);
        ++size;
        delete [] buffer;
        buffer = new char[size + 1];
        strncpy(buffer, temp, size + 1);
        buffer[size - 1] = c;
        delete [] temp;
    }
    void TString::operator +(const TString& s){
        char * temp = new char[size + 1];
        strcpy(temp, buffer);
        size+=s.size;
        delete [] buffer;
        buffer = new char[size + 1];
        strcpy(buffer, temp);
        strcat(buffer, s.buffer);
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