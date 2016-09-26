#include <iostream>


class Rectangle {
public:
    Rectangle(): height(0), width(0) {};

    Rectangle(int height, int width): height(height), width(width) {};

    int area() {
        return this->height * this-> width;
    }

    void print(std::ostream& os=std::cout) {
        os << this->width << ' ' << this->height << std::endl;
    }

    int height;
    int width;
};


class Stack {
public:

    Stack(size_t init_size=5, unsigned int size_factor=2): arr_size(init_size), size_factor(size_factor) {
        this->arr = new Rectangle*[arr_size];
        this->arr_begin = arr;
        this->arr_end = this->arr_begin + arr_size;

        this->end = this->arr_begin;
    }

    ~Stack() {
        delete[] this->arr;
    }

    void push(Rectangle rect) {

        if (this->is_full()) {
            this->realloc_arr();
        }

        *(this->end) = new Rectangle(rect.height, rect.width);
        ++(this->end);

        return;
    }

    Rectangle& pop() {
        --(this->end);
        return **(this->end);
    }

    Rectangle& top() {
        return **(this->end - 1);
    }

    bool is_empty() {
        return this->end == this->arr_begin;
    }

private:

    bool is_full() {
        return this->end == this->arr_end;
    }

    void realloc_arr() {
        this->arr_size *= this->size_factor;
        Rectangle** new_arr = new Rectangle*[this->arr_size];
        int new_arr_cnt = 0;

        while (!(this->is_empty())) {
            new_arr[new_arr_cnt++] = *(this->arr_begin++);
        }
        delete[] this->arr;
        this->arr = new_arr;

        this->arr_begin = this->arr;
        this->arr_end = this->arr + this->arr_size;

        this->end = this->arr_begin + new_arr_cnt;
    }

    Rectangle** arr;
    Rectangle** arr_begin;
    Rectangle** arr_end;

    Rectangle** end;
    size_t arr_size;
    int size_factor;
};


void push_to_stack(Stack& stack, Rectangle rect) {
    if (stack.is_empty()) {
        stack.push(rect);
        return;
    }

    if (rect.height > stack.top().height) {
        stack.push(rect);
    } else {
        stack.top().width += rect.width;
        stack.top().height = rect.height;
    }
}


int get_max_area(Stack& stack) {
    int curr_max_area = 0;

    while (!stack.is_empty()) {
        Rectangle curr_top = stack.pop();

        if (curr_top.area() > curr_max_area) {
            curr_max_area = curr_top.area();
        }

        if (!stack.is_empty()) {
            stack.top().width += curr_top.width;
        } else {
            break;
        }
    }

    return curr_max_area;
}


int main() {
    int rect_num = 0;
    std::cin >> rect_num;

    int rect_width = 0;
    int rect_height = 0;
    Stack stack = Stack();
    for (int i = 0; i != rect_num; ++i) {
        std::cin >> rect_width >> rect_height;
        Rectangle rect = Rectangle(rect_height, rect_width);
        push_to_stack(stack, rect);
    }


    for (int i = 0; i != rect_num; ++i) {
        Rectangle rect = stack.pop();
        rect.print();
    }


    //std::cout << get_max_area(stack);
}