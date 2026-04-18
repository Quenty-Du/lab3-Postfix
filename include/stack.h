#ifndef __TSTACK_H__
#define __TSTACK_H__

#include <tmatrix.h>

template <typename T>
class TStack {
    private:
        int top;
        TVector<T> data;
    public:
        // Конструктор и деструктор
        TStack(size_t input_data_size = 1); // Базовый конструктор
        ~TStack() = default; // Деструктор

        // Проверка состояния стека
        size_t Size() const noexcept; // Получение количества элементов в стеке
        bool IsEmpty() const noexcept; // Проверка стека на пустоту
        bool IsFull() const noexcept; // Проверка стека на полноту
        const T& Top() const; // Просмотр верхнего элемента стека

        // Методы работы со стеком
        T Pop(); // Извлечение элемента
        void Push(const T& val); // Вставка элемента с перевыделением
        void Clean(); // Очистка стека
};

template <typename T>
TStack<T>::TStack(size_t input_data_size) : top(-1), data(input_data_size) {
}


template <typename T>
size_t TStack<T>::Size() const {
    return top + 1;
}


template <typename T>
bool TStack<T>::IsEmpty() const {
    return top == -1;
}


template <typename T>
bool TStack<T>::IsFull() const {
    return static_cast<int>(data.size()) == top + 1;
}


template <typename T>
const T& TStack<T>::Top() const {
    if (IsEmpty())
        throw std::logic_error("TStack<T>::Top(): stack is empty");
    return data[top];
}


template <typename T>
T TStack<T>::Pop() {
    if (IsEmpty())
        throw std::logic_error("TStack<T>::Pop(): stack is empty");
    return data[top--];
}


template <typename T>
void TStack<T>::Push(const T& val) {
    if (IsFull()) {
        TVector<T> new_data(data.size() * 2);

        for (size_t i = 0; i <= static_cast<size_t>(top); ++i) {
            new_data[i] = data[i];
        }

        swap(data, new_data);
    }
    data[++top] = val;
}


template <typename T>
void TStack<T>::Clean() {
    top = -1;
}

#endif