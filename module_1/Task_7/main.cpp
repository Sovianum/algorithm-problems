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
#include <algorithm>
#include <assert.h>
#include <ctime>


struct Athlete {
    Athlete (): mass(0), strength(0){};
    Athlete (int mass, int strength): mass(mass), strength(strength) {};
    Athlete (const Athlete& athlete): mass(athlete.mass), strength(athlete.strength) {};
    Athlete&operator=(const Athlete& athlete) {
        mass = athlete.mass;
        strength = athlete.strength;
        return *this;
    };

    bool operator<(const Athlete& athlete) const {
        assert(mass && strength);

        if (mass < athlete.mass) {
            return true;
        } else {
            return strength < athlete.strength;
        }
    }

    int mass;
    int strength;
};


class Array {
public:
    Array();

    Array(int start_buffer_size, int size_factor);

    ~Array();

    void push_back(const Athlete& element);

    void push_back(Athlete&& element);

    Athlete&operator[](int index);

    int get_length() { return element_count;};

    bool is_empty() { return element_count == 0;};

    void sort() {
        std::sort(buffer, buffer + element_count);
    }

private:
    bool is_full() { return element_count == buff_size;};
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
    delete[] buffer;
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
    buff_size *= size_factor;
    Athlete* new_buffer = new Athlete[buff_size]();
    for (int i = 0; i != element_count; ++i) {
        new_buffer[i] = buffer[i];
    }

    delete[] buffer;
    buffer = new_buffer;
}



void read_in(std::istream& is, Array& array) {
    int mass = 0;
    int strength = 0;
    while (is >> mass >> strength) {
        array.push_back(Athlete(mass, strength));
    }
}

int solve(Array& athlete_arr) {
    int result = 0;
    int top_mass = 0;
    athlete_arr.sort();

    for (size_t i = 0; i != athlete_arr.get_length(); ++i) {
        if (athlete_arr[i].strength >= top_mass) {
            top_mass += athlete_arr[i].mass;
            ++result;
        }
    }

    return result;
}


void make_random_athletes_file(size_t athlete_num, std::ofstream& ofs) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);


    std::srand((unsigned int)std::time(0));
    for (size_t i = 0; i != athlete_num; ++i) {
        ofs << dis(gen) << ' ' << dis(gen) << std::endl;
    }
}

void test() {
    std::ofstream ofs("/home/artem/ClionProjects/algorithm-problems/module_1/Task_7/test_input.txt");
    make_random_athletes_file(1000, ofs);
    ofs.close();


    Array athlete_arr;

    std::ifstream ifs ("/home/artem/ClionProjects/algorithm-problems/module_1/Task_7/test_input.txt");
    read_in(ifs, athlete_arr);
    ifs.close();

    if (athlete_arr.get_length() == 0) {
        std::cout << 0;
    }

    bool* athlete_use_arr = new bool[athlete_arr.get_length()];

    std::cout << solve(athlete_arr) << std::endl;

    delete[] athlete_use_arr;

}


int main() {

    //test();

    //std::ifstream ifs ("/home/artem/ClionProjects/algorithm-problems/module_1/Task_7/input.txt");

    Array athlete_arr;

    read_in(std::cin, athlete_arr);

    athlete_arr.sort();

    std::cout << solve(athlete_arr);

    return 0;
}