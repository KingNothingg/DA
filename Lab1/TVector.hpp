#ifndef TVector_HPP
#define TVector_HPP

namespace NStd{
    template<class T>
    class TVector {
    private:
        unsigned int capacity;
        unsigned int maxsize;
        T *data;
    public:
        void Assign(unsigned int n, T elem);
        T &operator[](const unsigned int &index) const;
        void PushBack(T elem);
        unsigned int Size() const;
        TVector();
        TVector(char * str){

        }
        TVector(TVector<T> &vec);
        TVector(unsigned int n);
        TVector(unsigned int n, T elem);
        ~TVector();
    };
    template<class T>
    TVector<T>::TVector(TVector<T> &vec) {
        data = vec.data;
        capacity = vec.capacity;
        maxsize = vec.maxsize;
    }
    template<class T>
    T& TVector<T>::operator[](const unsigned int &index) const {
        return data[index];
    }
    template<class T>
    void TVector<T>::Assign(const unsigned int n, T elem) {
        delete[] data;
        maxsize = n*2;
        data = new T[maxsize];
        capacity = n;
        for (int i = 0; i < n; i++) {
            data[i] = elem;
        }
    }
    template<class T>
    void TVector<T>::PushBack(T elem) {
        if (capacity == 0) {
            maxsize = 1;
            data = new T[maxsize];
        }
        if (capacity == maxsize) {
            maxsize = maxsize * 2;
            T* newdata = new T[maxsize];
            for (int i = 0; i < capacity; ++i) {
                newdata[i] = data[i];
            }
            delete[] data;
            data = newdata;
        }
        data[capacity] = elem;
        capacity++;
    }
    template<class T>
    unsigned int TVector<T>::Size() const{
        return capacity;
    }
    template<class T>
    TVector<T>::TVector() {
        capacity = 0;
        maxsize = 0;
        data = 0;
    }
    template<class T>
    TVector<T>::TVector(const unsigned int n) {
        capacity = n;
        maxsize = n;
        data = new T[capacity];
    }
    template<class T>
    TVector<T>::TVector(const unsigned int n, T elem) {
        capacity = n;
        maxsize = n;
        data = new T[capacity];
        Assign(n, elem);
    }
    template<class T>
    TVector<T>::~TVector() {
        delete[] data;
    }
}

#endif