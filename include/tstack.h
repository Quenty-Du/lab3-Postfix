#ifndef __TSTACK_H__
#define __TSTACK_H__

#include "tvector.h"

template <typename T>
class TStack {
    private:
        TVector<T> data;
    public:
        // Конструктор и деструктор
        TStack(size_t input_data_size = 1);

        // Проверка состояния стека
        size_t size() const noexcept;
        bool isEmpty() const noexcept;
        const T& top() const;
        T& top();

        // Методы работы со стеком
        void pop();
        void push(const T& val);
        void clear();
};

template <typename T>
TStack<T>::TStack(size_t input_data_size) : data(input_data_size) {
}


template <typename T>
size_t TStack<T>::size() const {
    return data.size();
}


template <typename T>
bool TStack<T>::isEmpty() const {
    return data.size() == 0;
}


template <typename T>
const T& TStack<T>::top() const {
    if (isEmpty()) {
        throw std::logic_error("TStack<T>::Top(): stack is empty");
    }

    return data.back();
}


template <typename T>
T& TStack<T>::top() {
    if (isEmpty()) {
        throw std::logic_error("TStack<T>::Top(): stack is empty");
    }

   return data.back();
}


template <typename T>
void TStack<T>::pop() {
    if (isEmpty())
        throw std::logic_error("TStack<T>::Pop(): stack is empty");
    data.popBack();
}


template <typename T>
void TStack<T>::push(const T& val) {
    data.pushBack(val);
}


template <typename T>
void TStack<T>::clear() {
    data.clear();
}

#endif