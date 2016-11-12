/*
 * Дан массив строк. Количество строк не больше 10^5. Отсортировать массив методом поразрядной
 * сортировки MSD п символам. Размер алфавита - 256 символов. Поледний символ = '\0'
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <ctime>

size_t left_child_index(size_t index) {
    return 2 * index + 1;
};

size_t right_child_index(size_t index) {
    return 2 * index + 2;
};

template <class T>
void heapifyTop(T* begin, T* end, T* start, std::function<bool(const T&, const T&)> compare) {
    size_t root_ind = start - begin;
    size_t left_child_ind = left_child_index(root_ind);
    size_t right_child_ind = right_child_index(root_ind);
    size_t max_child_ind;

    size_t interval_length = end - begin;

    while (true) {
        if (right_child_ind < interval_length) {
            max_child_ind = compare(begin[left_child_ind], begin[right_child_ind]) ? right_child_ind : left_child_ind;
        } else if (left_child_ind < interval_length) {
            max_child_ind = left_child_ind;
        } else {
            return;
        }

        if (compare(begin[root_ind], begin[max_child_ind])) {
            std::swap(begin[root_ind], begin[max_child_ind]);

            root_ind = max_child_ind;
            left_child_ind = left_child_index(root_ind);
            right_child_ind = right_child_index(root_ind);
        } else {
            return;
        }
    }
}

template <class T>
T extractTop(T *begin, T *&end, std::function<bool(const T&, const T&)> compare) {
    std::swap(*begin, *(--end));
    auto result = *end;

    heapifyTop(begin, end, begin, compare);

    return result;
}

template <class T>
void heapSort(T* begin, T* end, std::function<bool(const T&, const T&)> compare) {
    for (std::ptrdiff_t i = (end - begin) / 2; i >= 0; --i) {
        heapifyTop(begin, end, begin + i, compare);
    }

    auto range = end - begin;
    for (size_t i = 0; i != range; ++i) {
        extractTop(begin, end, compare);
    }
}


std::vector<std::string> myReadIn(std::istream& is) {
    std::vector<std::string> result;
    std::string temp;

    while (is >> temp) {
        result.push_back(temp);
    }

    return result;
}

size_t getMaxStrLen (const std::string* begin, const std::string* end) {
    size_t result = 0;

    for (auto i = begin; i != end; ++i) {
        if (i->size() > result) {
            result = i->size();
        }
    }

    return result;
}

char getSymbol(const std::string& s, const size_t index) {
    if (index >= s.size()) {
        return '\0';
    } else {
        return s[index];
    }
}

void sortStringsBySymbol(std::string* begin, std::string* end, const size_t index) {
    auto compare = [index](const std::string& s_1, const std::string& s_2) -> bool {
        return getSymbol(s_1, index) < getSymbol(s_2, index);
    };

    heapSort<std::string>(begin, end, compare);
}

std::vector<size_t> getBinLengths(const std::string* begin, const std::string* end, const size_t index) {
    auto result = std::vector<size_t>();
    char symbol = -1;

    for (auto str_ptr = begin; str_ptr != end; ++str_ptr) {
        //if ((*str_ptr)[index] != symbol) {
        char curr_symbol = getSymbol(*str_ptr, index);
        if (curr_symbol != symbol) {
            result.push_back(1);
            symbol = curr_symbol;
        } else {
            ++(result.back());
        }
    }

    return result;
}

void MSDSort(std::string *begin, std::string *end, size_t index=0) {
    if ((end - begin <= 1) || (index >= getMaxStrLen(begin, end))) {
        return;
    }

    sortStringsBySymbol(begin, end, index);
    auto bin_lengths = getBinLengths(begin, end, index);

    std::string* range_begin;
    std::string* range_end = begin;

    for (auto bin_length: bin_lengths) {
        range_begin = range_end;
        range_end = range_begin + bin_length;

        MSDSort(range_begin, range_end, index + 1);
    }
}

template <typename T>
void printVec(std::vector<T> vec) {
    for (auto item: vec) {
        std::cout << item << std::endl;
    }
}


void make_random_file(size_t max_str_len, size_t str_num, std::ofstream &ofs) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> char_distribution((int)'a', (int)'z');
    std::uniform_int_distribution<> str_len_distribution(1, (int)max_str_len);

    std::srand((unsigned int)std::time(0));
    for (size_t i = 0; i != str_num; ++i) {
        auto curr_str_len = str_len_distribution(gen);
        for (size_t j = 0; j != curr_str_len; ++j) {
            ofs << (char)char_distribution(gen);
        }

        ofs << std::endl;
    }
}


void test() {
    std::ofstream ofs("/home/artem/ClionProjects/algorithm-problems/module_2/task_6/test_input.txt");
    make_random_file(100, 100, ofs);
    ofs.close();

    std::ifstream is("/home/artem/ClionProjects/algorithm-problems/module_2/task_6/test_input.txt");

    auto input = myReadIn(is);

    MSDSort(&(input[0]), &(input[input.size()]), 0);
    printVec(input);
    std::cout << std::endl;

    is.close();
}

int main() {
    std::istream& is = std::cin;

    auto input = myReadIn(is);
    MSDSort(&(input[0]), &(input[input.size()]));
    printVec(input);

    return 0;
}