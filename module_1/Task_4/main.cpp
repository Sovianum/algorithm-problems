/*
 * Реализовать дек с динамически зацикленным буфером
 */

#include <iostream>
#include <assert.h>


class Deque {
public:

    Deque(size_t init_size=5, unsigned int size_factor=2);

    ~Deque();

    void push_front(int element);

    void push_back(int element);

    int pop_front();

    int pop_back();

    bool is_empty();

private:
    bool is_full();

    int* move_forward(int* &ptr);

    int* move_backward(int* &ptr);

    void realloc_buffer();

    int* buffer;
    int* buffer_begin;
    int* buffer_end;

    int* begin;
    int* end;
    size_t arr_size;
    int size_factor;
};

Deque::Deque(size_t init_size, unsigned int size_factor): arr_size(init_size), size_factor(size_factor) {
    this->buffer = new int[arr_size];
    this->buffer_begin = buffer;
    this->buffer_end = this->buffer_begin + arr_size;

    this->begin = this->buffer_begin;
    this->end = this->buffer_begin + 1;
}

Deque::~Deque() {
    delete[] this->buffer;
}

void Deque::push_front(int element) {
    if (this->is_full()) {
        this->realloc_buffer();
    }

    *(this->begin) = element;
    this->begin = this->move_backward(this->begin);

    return;
}

void Deque::push_back(int element) {
    if (this->is_full()) {
        this->realloc_buffer();
    }

    *(this->end) = element;
    this->end = this->move_forward(this->end);

    return;
}

int Deque::pop_front() {
    if (this->is_empty()) {
        return -1;
    } else {
        this->begin = this->move_forward(this->begin);
        return *(this->begin);
    }
}

int Deque::pop_back() {
    if (this->is_empty()) {
        return -1;
    } else {
        this->end = this->move_backward(this->end);
        int result = *(this->end);
        return result;
    }
}

bool Deque::is_empty() {
    return this->move_forward(this->begin) == this->end;
}

bool Deque::is_full() {
    return this->move_forward(this->end) == this->begin;
}

int* Deque::move_forward(int *&ptr) {
    if (ptr + 1 == this->buffer_end) {
        return this->buffer_begin;
    } else {
        return ptr + 1;
    }
}

int* Deque::move_backward(int *&ptr) {
    if (ptr == this->buffer_begin) {
        return this->buffer_end - 1;
    } else {
        return ptr - 1;
    }
}

void Deque::realloc_buffer() {
    this->arr_size *= this->size_factor;
    int* new_buffer = new int[this->arr_size];
    int new_buffer_cnt = 0;

    while (!(this->is_empty())) {
        new_buffer[new_buffer_cnt++] = this->pop_front();
    }
    delete[] this->buffer;
    this->buffer = new_buffer;

    this->buffer_begin = this->buffer;
    this->buffer_end = this->buffer + this->arr_size;

    this->begin = this->move_backward(this->buffer_begin);     // смещение назад сделано потому, что сдвиг указателя происходит перед извлечение элемента
    this->end = this->buffer_begin + new_buffer_cnt;
}


bool handle_deque(Deque &deque, int code, int arg) {
    switch(code) {
        case 1: {
            deque.push_front(arg);
            return true;
        }
        case 2: {
            return deque.pop_front() == arg;
        }
        case 3: {
            deque.push_back(arg);
            return true;
        }
        case 4: {
            return deque.pop_back() == arg;
        }
        default: {
            assert(false);
        }
    }
}


void test_deque() {
    Deque deque = Deque();
    std::cout << handle_deque(deque, 3, 44) << std::endl;
    std::cout << handle_deque(deque, 3, 50) << std::endl;
    std::cout << handle_deque(deque, 2, 44) << std::endl << std::endl;

    deque = Deque();
    std::cout << handle_deque(deque, 2, -1) << std::endl;
    std::cout << handle_deque(deque, 3, 10) << std::endl << std::endl;

    deque = Deque();
    std::cout << handle_deque(deque, 3, 44) << std::endl;
    std::cout << handle_deque(deque, 2, 60) << std::endl << std::endl;
}


int main() {
    int operation_num = 0;
    std::cin >> operation_num;

    Deque deque = Deque();
    int operation_code = 0;
    int operation_arg = 0;
    for (int i = 0; i != operation_num; ++i) {
        std::cin >> operation_code >> operation_arg;
        if (!handle_deque(deque, operation_code, operation_arg)) {
            std::cout << "NO" << std::endl;
            return 0;
        }
    }
    std::cout << "YES" << std::endl;

    return 0;
}