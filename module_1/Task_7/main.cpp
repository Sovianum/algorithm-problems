/*
 * В город N приехал цирк с командой атлетов. Они хотя удивить горожан города N - выстроить из своих тел башню максимальной высоты.
 * Башня - это цепочка атлетов, первый стоит на земле, второй стоит у него на плечах, третий стоит на плечах у второго и т.д.
 * Каждый атлет характеризуется силой s_i (kg) и массой m_i (kg). Сила - это максимальная масса, которую атлет способен держать у себя на плечах.
 * К сожалению, ни один из атлетов не умеет программировать, так как всю жизнь они занимались физической подготовкой, и у них
 * не было времени на изучение языков программирования. Помогите им, напишите программу, которая определит максимальную высоту башни, которую они
 * могут составить.
 * Известно, что если атлет тяжелее, то он и сильнее: если m_i > m_j, то s_i > s_j.
 * Атлеты равной массы могут иметь различную силу.
 */

#include <iostream>
#include <assert.h>
#include <fstream>


struct Athlete {
    Athlete (): mass(0), strength(0){};
    Athlete (int mass, int strength): mass(mass), strength(strength) {};
    Athlete (const Athlete& athlete): mass(athlete.mass), strength(athlete.strength) {};
    Athlete&operator=(const Athlete& athlete) {
        mass = athlete.mass;
        strength = athlete.strength;
        return *this;
    };

    int mass;
    int strength;
};

void print_athlete(const Athlete& athlete) {
    std::cout << "m = " << athlete.mass << "; s = " << athlete.strength << std::endl;
}


class Array {
public:
    Array();

    Array(int start_buffer_size, int size_factor);

    ~Array();

    void push_back(const Athlete& element);

    void push_back(Athlete&& element);

    Athlete&operator[](int index);

    int get_length() { return this->element_count;};

    bool is_empty() { return this->element_count == 0;};

private:
    bool is_full() { return this->element_count == this->buff_size;};
    void realloc_buffer();

    Athlete* buffer;
    int buff_size;
    int element_count;
    int size_factor;
};

Array::Array() {
    this->buff_size = 10;
    this->size_factor = 2;
    this->element_count = 0;
    this->buffer = new Athlete[this->buff_size];
}

Array::Array(int start_buffer_size, int size_factor) {
    this->buff_size = start_buffer_size;
    this->size_factor = size_factor;
    this->element_count = 0;
    this->buffer = new Athlete[this->buff_size];
}

Array::~Array() {
    delete[] this->buffer;
}

void Array::push_back(const Athlete &element) {
    if (is_full()) {
        this->realloc_buffer();
    }
    this->buffer[element_count++] = element;
}

void Array::push_back(Athlete &&element) {
    if (is_full()) {
        this->realloc_buffer();
    }
    this->buffer[element_count++] = element;
}

Athlete& Array::operator[](int index) {
    assert(!this->is_empty());

    return this->buffer[index];
}

void Array::realloc_buffer() {
    Athlete* new_buffer = new Athlete[this->buff_size * this->size_factor]();
    for (int i = 0; i != this->buff_size; ++i) {
        new_buffer[i] = this->buffer[i];
    }

    delete[] this->buffer;
    this->buffer = new_buffer;
}


Athlete find_strongest(Array& arr, bool* athlete_use_arr) {
    int max_strength = -1;
    Athlete strongest;
    int strongest_ind = -1;

    for (int i = 0; i != arr.get_length(); ++i) {
        if (!athlete_use_arr[i] && arr[i].strength > max_strength) {
            strongest = arr[i];
            strongest_ind = i;
            max_strength = arr[i].strength;
        }
    }

    athlete_use_arr[strongest_ind] = true;
    return strongest;
}

Athlete find_next(Array& athlete_arr, bool* athlete_use_arr, int& strength_left, bool& status) {
    int max_next_strength_left = -1;
    Athlete next_athlete;
    int next_athlete_ind = -1;
    status = false;

    for (int i = 0; i != athlete_arr.get_length(); ++i) {
        if (!athlete_use_arr[i]) {
            int strength_1 = athlete_arr[i].strength;
            int strength_2 = strength_left - athlete_arr[i].mass;
            int min_val = strength_1 < strength_2 ? strength_1 : strength_2;

            bool cond = true;
            cond &= (min_val > max_next_strength_left) && (min_val >= 0);
            if (min_val == max_next_strength_left) {
                cond &= athlete_arr[i].mass > athlete_arr[next_athlete_ind].mass;
            }

            //if ((min_val > max_next_strength_left) && (min_val > 0)) {
            if (cond) {
                max_next_strength_left = min_val;
                next_athlete = athlete_arr[i];
                next_athlete_ind = i;
                strength_left = min_val;
                status = true;
            }
        }
    }

    if (status) {
        athlete_use_arr[next_athlete_ind] = true;
    }

    return next_athlete;
}


void read_in(std::istream& is, Array& array) {
    int mass = 0;
    int strength = 0;
    while (is >> mass >> strength) {
        array.push_back(Athlete(mass, strength));
    }
}

int solve(Array& athlete_arr, bool* athlete_use_arr) {
    int max_height = 1;
    Athlete curr_athlete = find_strongest(athlete_arr, athlete_use_arr);
    int strength_left = curr_athlete.strength;
    bool status = true;

    while (status) {
        curr_athlete = find_next(athlete_arr, athlete_use_arr, strength_left, status);
        if (status) {
            ++max_height;
        }
    }

    return max_height;
}




int main() {
    Array athlete_arr = Array();
    //std::ifstream ifs ("/home/artem/ClionProjects/algorithm-problems/module_1/Task_7/input.txt");

    read_in(std::cin, athlete_arr);
    bool* athlete_use_arr = new bool[athlete_arr.get_length()];

    std::cout << solve(athlete_arr, athlete_use_arr);

    delete[] athlete_use_arr;
    return 0;
}