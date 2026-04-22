#ifndef __TVECTOR_H__
#define __TVECTOR_H__

#include <cstddef>
#include <cassert>
#include <string>

const int MAX_VECTOR_CAP = 100000000;

template<typename T>
class TVector {
private:
    static size_t check_vector_cap(size_t cap);
protected:
    size_t sz;
    size_t cap;
    T* data;
public:
    // Конструкторы
    TVector(size_t input_cap = 1);
    TVector(const TVector& v);
    TVector(TVector&& v) noexcept;
    ~TVector();
    TVector& operator=(const TVector& v);
    TVector& operator=(TVector&& v) noexcept;

    // Методы получения значений полей
    size_t size() const noexcept;
    size_t capacity() const noexcept;

    // Написать front и back
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    // Методы добавления\удаления элементов
    void pushBack(const T& element);
    void popBack();
    void clear();

    // Методы изменения вместимости
    void reserve(size_t new_cap);
    void resize(size_t new_cap);
    void shrinkToFit();

    // Индексация
    T& operator[](size_t ind);
    const T& operator[](size_t ind) const;
    T& at(size_t ind);
    const T& at(size_t ind) const;

    // Операторы сравнения
    bool operator==(const TVector& v) const noexcept;
    bool operator!=(const TVector& v) const noexcept;

    friend void swap(TVector& lhs, TVector& rhs) noexcept {
        std::swap(lhs.cap, rhs.cap);
        std::swap(lhs.data, rhs.data);
    }
};


template<typename T>
size_t TVector<T>::check_vector_cap(size_t cap)
{
    if (cap > MAX_VECTOR_CAP || cap == 0) throw std::out_of_range("Vector cap must be greater than zero and not over" + std::to_string(MAX_VECTOR_CAP));

    return cap;
}


template<typename T>
TVector<T>::TVector(size_t input_cap) :
sz(0),
cap(check_vector_cap(input_cap)),
data(static_cast<T*>(::operator new(cap * sizeof(T))))
{
}


template<typename T>
TVector<T>::TVector(const TVector& v) : 
sz(v.sz),
cap(v.cap), 
data(static_cast<T*>(::operator new(v.cap * sizeof(T))))
{
    for (size_t i = 0; i < sz; ++i)
    {
        pushBack(v.data[i]);
    }
}


template<typename T>
TVector<T>::TVector(TVector&& v) noexcept :
sz(v.sz),
cap(v.cap),
data(v.data) { 
    v.sz = 0;
    v.cap = 0;
    v.data = nullptr;
}


template<typename T>
TVector<T>::~TVector() {
    for (size_t i = 0; i < sz; ++i) {
        data[i].~T();
    }
    ::operator delete(data);
    data = nullptr;
}


template <typename T>
size_t TVector<T>::size() const {
    return sz;
}


template<typename T>
size_t TVector<T>::capacity() const {
 return cap;
} 


template <typename T>
T& TVector<T>::front() {
    if (sz == 0) {
        throw std::logic_error("TVector<T>::front(): Vector is empty");
    }

    return data[0];
}


template <typename T>
const T& TVector<T>::front() const {
    if (sz == 0) {
        throw std::logic_error("TVector<T>::front(): Vector is empty");
    }

    return data[0];
}


template <typename T>
T& TVector<T>::back() {
    if (sz == 0) {
        throw std::logic_error("TVector<T>::back(): Vector is empty");
    }

    return data[sz - 1];
}


template <typename T>
const T& TVector<T>::back() const {
    if (sz == 0) {
        throw std::logic_error("TVector<T>::back(): Vector is empty");
    }

    return data[sz - 1];
}


template <typename T>
void TVector<T>::reserve(size_t new_cap) {
    if (new_cap <= cap) return;
        
    T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));
        
    for (size_t i = 0; i < sz; ++i) {
        new (new_data + i) T(std::move(data[i]));
        data[i].~T();
    }
        
    ::operator delete(data);
    data = new_data;
    cap = new_cap;
}


template <typename T>
void TVector<T>::resize(size_t new_cap) {
    if (new_cap == cap) return;

    TVector<T> temp;
    temp.reserve(new_cap);

    size_t copy_sz = (new_cap < sz) ? new_cap : sz;
    for (size_t i = 0; i < copy_sz; ++i) {
        temp.pushBack(data[i]);
    }

    swap(*this, temp);
}


template <typename T>
void TVector<T>::shrinkToFit() {
    resize(sz);
}


template <typename T>
void TVector<T>::pushBack(const T& element) {
    if (sz == cap) {
        reserve(cap == 0 ? 1 : cap * 2);
    }

    new (data + sz) T(element);
    ++sz;
}


template <typename T>
void TVector<T>::popBack() {
    sz--;
    data[sz].~T();
}


template <typename T>
void TVector<T>::clear() {
    while (sz != 0) {
        popBack();
    }
}


template<typename T>
TVector<T>& TVector<T>::operator=(const TVector& v) {
    if (this == &v) return *this;

    TVector<T> temp(v);
    swap(*this, temp);
    return *this;
}


template<typename T>
TVector<T>& TVector<T>::operator=(TVector&& v) {
    swap(*this, v);
    return *this;
}


template<typename T>
T& TVector<T>::operator[](size_t ind) {
    return data[ind];
}


template<typename T>
const T& TVector<T>::operator[](size_t ind) const {
    return data[ind];
}


template<typename T>
T& TVector<T>::at(size_t ind) {
    if (ind >= cap) throw std::out_of_range("Index out of range");

    return data[ind];
}


template<typename T>
const T& TVector<T>::at(size_t ind) const {
    if (ind >= cap) throw std::out_of_range("Index out of range");

    return data[ind];
}


template<typename T>
bool TVector<T>::operator==(const TVector& v) const {
    if (sz != v.sz) return false;

    for (size_t i = 0; i < sz; ++i)
    {
        if (data[i] != v.data[i]) return false;
    }

    return true;
}


template<typename T>
bool TVector<T>::operator!=(const TVector& v) const {
    return !(*this == v);
}

#endif