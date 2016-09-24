/*
 * Реализовать дек с динамически зацикленным буфером
 */

#include <iostream>
#include <assert.h>


class Deque {
public:

    Deque(size_t init_size=5, unsigned int size_factor=2): arr_size(init_size), size_factor(size_factor) {
        this->arr = new int[arr_size];
        this->arr_begin = arr;
        this->arr_end = this->arr_begin + arr_size;

        this->begin = this->arr_begin;
        this->end = this->arr_begin + 1;
    }

    ~Deque() {
        delete[] this->arr;
    }

    void push_front(int element) {
        if (this->is_full()) {
            this->realloc_arr();
        }

        *(this->begin) = element;
        this->begin = this->move_backward(this->begin);

        return;
    }

    void push_back(int element) {

        if (this->is_full()) {
            this->realloc_arr();
        }

        *(this->end) = element;
        this->end = this->move_forward(this->end);

        return;
    }

    int pop_front() {
        if (this->is_empty()) {
            return -1;
        } else {
            this->begin = this->move_forward(this->begin);
            return *(this->begin);
        }
    }

    int pop_back() {
        if (this->is_empty()) {
            return -1;
        } else {
            this->end = this->move_backward(this->end);
            int result = *(this->end);
            return result;
        }
    }

private:

    int* move_forward(int* &ptr) {
        if (ptr + 1 == this->arr_end) {
            return this->arr_begin;
        } else {
            return ptr + 1;
        }
    }

    int* move_backward(int* &ptr) {
        if (ptr == this->arr_begin) {
            return this->arr_end - 1;
        } else {
            return ptr - 1;
        }
    }

    bool is_empty() {
        return this->move_forward(this->begin) == this->end;
    }

    bool is_full() {
        return this->move_forward(this->end) == this->begin;
    }

    void realloc_arr() {
        this->arr_size *= this->size_factor;
        int* new_arr = new int[this->arr_size];
        int new_arr_cnt = 0;

        while (!(this->is_empty())) {
            int temp = this->pop_front();   // TODO delete
            new_arr[new_arr_cnt++] = temp;
        }
        delete[] this->arr;
        this->arr = new_arr;

        this->arr_begin = this->arr;
        this->arr_end = this->arr + this->arr_size;

        this->begin = this->move_backward(this->arr_begin);     // смещение назад сделано потому, что сдвиг указателя происходит перед извлечение элемента
        this->end = this->arr_begin + new_arr_cnt;
    }

    int* arr;
    int* arr_begin;
    int* arr_end;

    int* begin;
    int* end;
    size_t arr_size;
    int size_factor;
};


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