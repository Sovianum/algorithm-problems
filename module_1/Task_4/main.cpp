#include <iostream>


class Deque {
public:
    Deque(size_t init_size=2, unsigned int size_factor=2): arr_size(init_size), size_factor(size_factor), is_full(
            false), is_empty(true) {
        this->arr = new int[arr_size];
        this->arr_begin = arr;
        this->arr_end = this->arr_begin + arr_size;

        this->begin = this->arr_begin;
        this->end = this->arr_begin;
    }

    ~Deque() {
        delete[] this->arr;
    }

    void push_front(int element) {
        if (this->is_full) {
            this->realloc_arr();
        }
        if (this->is_empty) {
            this->is_empty = false;
        }

        *(this->begin) = element;
        this->move_backward(this->begin);

        return;
    }

    void push_back(int element) {
        if (this->is_full) {
            this->realloc_arr();
        }
        if (this->is_empty) {
            this->is_empty = false;
        }

        this->move_forward(this->end);
        *(this->end) = element;

        return;
    }

    int pop_front() {
        if (this->is_empty) {
            return -1;
        } else {
            this->move_forward(this->begin);
            return *(this->begin);
        }
    }

    int pop_back() {
        if (this->is_empty) {
            return -1;
        } else {
            int result = *(this->end);
            this->move_backward(this->end);
            return result;
        }
    }

private:
    void move_forward(int* &ptr) {
        if (++ptr == this->arr_end) {
            ptr = this->arr_begin;
        }

        if (this->begin == this->end) {
            this->is_full = true;
        }
    }

    void move_backward(int* &ptr) {
        if (ptr == this->arr_begin) {
            ptr = this->arr_end - 1;
        } else {
            --ptr;
        }

        if (this->begin == this->end) {
            this->is_empty = true;
        }
    }

    void realloc_arr() {
        this->arr_size *= this->size_factor;
        int* new_arr = new int[this->arr_size];
        int new_arr_cnt = 0;

        while (this->begin != this->end) {
            new_arr[new_arr_cnt++] = *(this->begin);
            this->move_forward(this->begin);
        }

        this->arr_begin = this->arr;
        this->arr_end = this->arr + this->arr_size;

        this->begin = this->arr_begin;
        this->end = this->arr_begin + new_arr_cnt;

        delete[] this->arr;
        this->arr = new_arr;
        this->is_full = false;
    }

    int* arr;
    int* arr_begin;
    int* arr_end;

    int* begin;
    int* end;
    size_t arr_size;
    int size_factor;

    bool is_full;
    bool is_empty;
};

int main() {
    auto deq = Deque();
    deq.push_back(100);
    deq.push_back(250);
    deq.push_back(300);

    for (int i = 0; i != 3; ++i) {
        std::cout << deq.pop_front() << std::endl;
    }

    return 0;
}