// при вставке в полный стек должна перевыделяться память

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
        size_t Size() const; // Получение количества элементов в стеке
        bool IsEmpty() const; // Проверка стека на пустоту
        bool IsFull() const; // Проверка стека на полноту
        T Top() const; // Просмотр верхнего элемента стека

        // Методы работы со стеком
        T Pop(); // Извлечение элемента
        void Push(const T& val); // Вставка элемента
        void Clean(); // Очистка стека



};

template <typename T>
TStack<T>::TStack(size_t input_data_size) : top(-1), data(input_data_size) {
}


template <typename T>
size_t TStack<T>::size() const {
    return top + 1;
}

