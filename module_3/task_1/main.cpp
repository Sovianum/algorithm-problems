#include <string>

/*
 * Реализуйте структур данных типа "множество строк" на основе динамической
 * хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных
 * латинских букв.
 *
 * Хеш-функция строки должна быть реализована с помощью вычисления значения
 * многочлена методом Горнера.
 *
 * Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте
 * при добавлении элемента в случае, когда коэффициент заполнения таблицы достигает
 * 3/4.
 *
 * Структура данных должна подерживать операции добавления строки в множестов,
 * удаления строки из множества и проверки принадлежности данной строки множеству.
 *
 * Для разрешения коллизий используйте двойное хеширование
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <set>

class StringSet {
public:
    StringSet(size_t init_size=8, int hash_1_factor=17, int hash_2_factor=13, float max_fill_factor=0.75,
              int rehash_factor=2):
            curr_size{init_size}, item_count{0}, hash_1_factor{hash_1_factor}, hash_2_factor{hash_2_factor},
            max_fill_factor{max_fill_factor}, rehash_factor{rehash_factor} {
        key_vec = std::vector<enum KeyState>(init_size, EMPTY);
        value_vec = std::vector<std::string>(init_size);
    }

    bool add(std::string &);

    bool erase(std::string &);

    bool contains(std::string &);

private:
    enum KeyState {
        EMPTY,
        FULL,
        DELETED
    };

    size_t findFree(std::string&, size_t, size_t);

    size_t findIndex(std::string&, size_t, size_t, bool&);

    float get_fill_factor();

    size_t hash(std::string &, int);

    size_t hash_1(std::string &);
    size_t hash_2(std::string &);

    void rehash();

    size_t curr_size;
    size_t item_count;

    int hash_1_factor;
    int hash_2_factor;

    float max_fill_factor;

    int rehash_factor;

    std::vector<enum KeyState> key_vec;
    std::vector<std::string> value_vec;
};


bool StringSet::add(std::string &str) {
    if (get_fill_factor() >= max_fill_factor) {
        rehash();
    }

    size_t h_1 = hash_1(str);
    size_t h_2 = hash_2(str);
    bool is_free;
    size_t index = findIndex(str, h_1, h_2, is_free);

    if (is_free) {
        key_vec[index] = FULL;
        value_vec[index] = str;
        ++item_count;

        return true;
    }

    return false;
}

bool StringSet::erase(std::string &str) {
    size_t h_1 = hash_1(str);
    size_t h_2 = hash_2(str);
    bool is_free;

    size_t index = findIndex(str, h_1, h_2, is_free);
    if (!is_free && index != curr_size) {
        key_vec[index] = DELETED;
        --item_count;
        return true;
    }
    return false;
}

bool StringSet::contains(std::string &str) {
    size_t h_1 = hash_1(str);
    size_t h_2 = hash_2(str);
    bool is_free;
    size_t index = findIndex(str, h_1, h_2, is_free);
    return !is_free && index != curr_size;
    //return findIndex(str, h_1, h_2) != curr_size;
}


size_t StringSet::findFree(std::string& str, size_t h_1, size_t h_2) {
    for (size_t i = 0; i != curr_size; ++i) {
        size_t index = (h_1 + i * (2 * h_2 + 1)) % curr_size;

        if (key_vec[index] != FULL) {
            return index;
        }
    }

    return curr_size;
}

size_t StringSet::findIndex(std::string& str, size_t h_1, size_t h_2, bool& is_free) {
    is_free = false;
    size_t free_index = 0;
    bool used_deleted = false;

    for (size_t i = 0; i != curr_size; ++i) {
        size_t index = (h_1 + i * (2 * h_2 + 1)) % curr_size;

        if (!used_deleted && key_vec[index] == DELETED) {
            used_deleted = true;
            free_index = index;
        }
        if (key_vec[index] == EMPTY) {
            is_free = true;
            free_index = index;
            break;
        } else if (key_vec[index] == FULL && value_vec[index] == str) {
            return index;
        }
    }

    if (used_deleted) {
        is_free = true;
    }

    if (is_free) {
        return free_index;
    }

    return curr_size;
}

float StringSet::get_fill_factor() {
    return static_cast<float >(item_count) / curr_size;
}

size_t StringSet::hash(std::string &str, int a) {
    size_t hash = 0;
    for (auto c: str) {
        hash = (hash * a + c) % curr_size;
    }

    return hash;
}

size_t StringSet::hash_1(std::string &str) {
    return hash(str, hash_1_factor);
}

size_t StringSet::hash_2(std::string &str) {
    return hash(str, hash_2_factor);
}

void StringSet::rehash() {
    std::vector<std::string> value_buffer = value_vec;
    std::vector<enum KeyState> key_buffer = key_vec;

    curr_size *= rehash_factor;
    key_vec = std::vector<enum KeyState>(curr_size, EMPTY);
    value_vec = std::vector<std::string>(curr_size);

    for (size_t i = 0; i != value_buffer.size(); ++i) {
        if (key_buffer[i] == FULL) {
            std::string str = value_buffer[i];
            size_t h_1 = hash_1(str);
            size_t h_2 = hash_2(str);

            size_t insert_index = findFree(str, h_1, h_2);
            key_vec[insert_index] = FULL;
            value_vec[insert_index] = str;
        }
    }
}


bool handleStr(StringSet& string_set, std::string command_key, std::string str) {
    if (command_key == "+") {
        return string_set.add(str);
    } else if (command_key == "-") {
        return string_set.erase(str);
    } else if (command_key == "?") {
        return string_set.contains(str);
    } else {
        return false;
    }
}


void make_random_file(std::vector<std::string> str_vec, size_t str_num, std::ofstream &ofs) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> key_distribution(0, 2);
    std::uniform_int_distribution<> str_ind_distribution(0, str_vec.size() - 1);

    std::set<std::string> check_set;
    std::string status;

    std::srand((unsigned int)std::time(0));
    for (size_t i = 0; i != str_num; ++i) {
        auto key_code = key_distribution(gen);
        auto str_ind = str_ind_distribution(gen);
        auto sample = str_vec[str_ind];

        char key_val;
        if (key_code == 0) {
            key_val = '+';
            if (check_set.count(sample)) {
                status = "FAIL";
            } else {
                status = "OK";
                check_set.insert(sample);
            }

        } else if (key_code == 1) {
            key_val = '-';
            if (check_set.count(sample)) {
                status = "OK";
                check_set.erase(sample);
            } else {
                status = "FAIL";
            }

        } else if (key_code == 2) {
            key_val = '?';
            if (check_set.count(sample)) {
                status = "OK";
            } else {
                status = "FAIL";
            }
        }

        ofs << key_val << ' ';

        for (auto c: sample) {
            ofs << c;
        }

        ofs << " ";

        for (auto c: status) {
            ofs << c;
        }

        ofs << std::endl;
    }
}


void test() {
    /*
    std::ofstream ofs("/home/artem/ClionProjects/algorithm-problems/module_3/task_1/test_input.txt");
    auto str_vec = std::vector<std::string>{"hi", "buy", "go", "asdf", "qwrf", "vqwer", "bvqwvw", "wefw", "afwevawef",
                                            "vqwe", "vawv", "vawejn", "asvjhb", "vakvaejbnv", "as;kjd", "vaewrf", "vasdv"};
    make_random_file(str_vec, 1000, ofs);
    ofs.close();
     */

    std::ifstream is("/home/artem/ClionProjects/algorithm-problems/module_3/task_1/test_input.txt");
    std::string command_key;
    std::string str;
    std::string status;
    StringSet string_set;

    std::string my_status;
    int cnt = 1;
    while (is >> command_key >> str >> status) {
        if (handleStr(string_set, command_key, str)) {
            my_status = "OK";
        } else {
            my_status = "FAIL";
        }

        if (status == my_status) {
            std::cout << "Right " << cnt << std::endl;
        } else {
            std::cout << "Wrong " << cnt << std::endl;
            break;
        }

        ++cnt;
    }
}


int main() {
    //test();

    auto string_set = StringSet();
    //std::ifstream is("/home/artem/ClionProjects/algorithm-problems/module_3/task_1/input.txt");
    std::istream& is = std::cin;

    std::string command_key;
    std::string str;
    while (is >> command_key >> str) {
        if (handleStr(string_set, command_key, str)) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "FAIL" << std::endl;
        }
    }
}