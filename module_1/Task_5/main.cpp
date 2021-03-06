/*
 * Дана последовательность N прямоугольников различной ширины и высоты (w_i, h_i).
 * Прямоугольники расположены, начиная с точки (0, 0) на оси ОХ вплотную друг за другом (враво).
 * Требуется найти М-площадь ммаксимального прямоугольника (параллельного осям координат), который можно вырезать из этой фигуры.
 */


#include <iostream>
#include <assert.h>

struct Rectangle {
    Rectangle(): height(0), width(0) {};

    Rectangle(int height, int width): height(height), width(width) {};

    Rectangle(const Rectangle& rect) {
        height = rect.height;
        width = rect.width;
    }

    Rectangle(const Rectangle&& rect) {
        height = rect.height;
        width = rect.width;
    }

    Rectangle&operator=(const Rectangle& rect) {
        height = rect.height;
        width = rect.width;

        return *this;
    }

    int area() {
        return this->height * this-> width;
    }

    int height;
    int width;
};


class Stack {
public:

    Stack(size_t init_size=5, unsigned int size_factor=2);

    ~Stack();

    void push(Rectangle rect);

    Rectangle pop();

    Rectangle& top();

    bool is_empty();

private:

    bool is_full();

    void realloc_buffer();

    Rectangle* buffer_begin;
    Rectangle* buffer_end;

    Rectangle* end;
    int size_factor;
};

Stack::Stack(size_t init_size, unsigned int size_factor): size_factor(size_factor) {
    this->buffer_begin = new Rectangle[init_size];
    this->buffer_end = this->buffer_begin + init_size;

    this->end = this->buffer_begin;
}

Stack::~Stack() {
    delete[] this->buffer_begin;
}

void Stack::push(Rectangle rect) {

    if (this->is_full()) {
        this->realloc_buffer();
    }

    *(this->end) = Rectangle(rect.height, rect.width);
    ++(this->end);

    return;
}

Rectangle Stack::pop() {
    --(this->end);
    return *(this->end);
}

Rectangle& Stack::top() {
    return *(this->end - 1);
}

bool Stack::is_empty() {
    return this->end == this->buffer_begin;
}

bool Stack::is_full() {
    return this->end == this->buffer_end;
}

void Stack::realloc_buffer() {
    size_t new_buffer_size = this->size_factor * (this->buffer_end - this->buffer_begin);
    Rectangle* new_buffer = new Rectangle[new_buffer_size];
    int new_buffer_cnt = 0;

    Rectangle* temp_ptr = buffer_begin;
    while (temp_ptr != this->buffer_end) {
        new_buffer[new_buffer_cnt++] = *(temp_ptr++);
    }
    delete[] this->buffer_begin;
    this->buffer_begin = new_buffer;

    this->buffer_end = this->buffer_begin + new_buffer_size;

    this->end = this->buffer_begin + new_buffer_cnt;
}


void inner_process_rectangle(Stack& stack, Rectangle rect, int& curr_max) { // функция выполняет фактическую работу по добавлению прямоугольника в стек
    Rectangle eqv_rect = stack.pop();                // прямоугольник, который будет помещен обратно в стек
    if (eqv_rect.area() > curr_max) {
        curr_max = eqv_rect.area();
    }

    Rectangle temp_rect;                             // прямоугольник для временных вычислений

    while (!stack.is_empty() && stack.top().height >= rect.height) {
        temp_rect = stack.pop();
        if (temp_rect.height <= eqv_rect.height) {  // происходит срезание голов прямоугольников
            eqv_rect.height = temp_rect.height;
            eqv_rect.width += temp_rect.width;

            if (eqv_rect.area() > curr_max) {
                curr_max = eqv_rect.area();
            }
        } else {
            assert (false);     // невозможный случай, так как в этом случае прямоугольник должен был быть просто присоединен
        }
    }

    eqv_rect.height = rect.height;
    eqv_rect.width += rect.width;

    if (eqv_rect.area() > curr_max) {
        curr_max = eqv_rect.area();
    }
    stack.push(eqv_rect);
}


void process_rectangle(Stack& stack, Rectangle rect, int& curr_max) {   // обертка, добавляющая прямоугольник в стек
    if (stack.is_empty()) {
        stack.push(rect);
        curr_max = stack.top().area();
        return;
    }

    if (rect.height >= stack.top().height) {
        stack.push(rect);
    } else {
        inner_process_rectangle(stack, rect, curr_max);
    }
}


int get_max_area(Stack& stack, int& curr_max) { // функция производит обработку стека с прямоугольниками, упорядоченными по высоте
    if (stack.is_empty()) {
        return curr_max;
    }

    Rectangle top_rect = stack.pop();   // прямоугольник, снятый с вершины стека
    Rectangle temp_rect;

    while (!stack.is_empty()) {
        if (top_rect.area() > curr_max) {
            curr_max = top_rect.area();
        }

        temp_rect = stack.pop();
        top_rect.height = temp_rect.height;
        top_rect.width += temp_rect.width;
    }

    if (top_rect.area() > curr_max) {
        curr_max = top_rect.area();
    }

    return curr_max;
}


int main() {
    int rect_num = 0;
    std::cin >> rect_num;

    int rect_width = 0;
    int rect_height = 0;
    Stack stack = Stack();

    int curr_max = 0;
    for (int i = 0; i != rect_num; ++i) {
        std::cin >> rect_width >> rect_height;
        Rectangle rect = Rectangle(rect_height, rect_width);
        process_rectangle(stack, rect, curr_max);
    }


    std::cout << get_max_area(stack, curr_max);
}