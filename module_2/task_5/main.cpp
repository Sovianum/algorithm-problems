/*
 * Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если
 * j >= i + k, то a[i] <= a[j]. Требуется отсортировать последовательность. Последовательность может быть
 * очень длинной. Время работы O(n * log(k)). Доп. память O(k). Использовать слияние.
 */

#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <time.h>
#include <assert.h>

template <class T>
bool defaultIsLess(const T& item_1, const T& item_2) {
    return item_1 < item_2;
}

size_t left_child_index(size_t index) {
    return 2 * index + 1;
};

size_t right_child_index(size_t index) {
    return 2 * index + 2;
};

template <class T>
void heapifyTop(T* begin, T* end, T* start, bool (* compare)(const T&, const T&)) {
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
T extractTop(T *begin, T *&end, bool (*compare)(const T &, const T &)) {
    std::swap(*begin, *(--end));
    auto result = *end;

    heapifyTop(begin, end, begin, compare);

    return result;
}


template <class T>
void heapSort(T* begin, T* end, bool (* compare)(const T&, const T&)) {
    for (std::ptrdiff_t i = (end - begin) / 2; i >= 0; --i) {
        heapifyTop(begin, end, begin + i, compare);
    }

    auto range = end - begin;
    for (size_t i = 0; i != range; ++i) {
        extractTop(begin, end, compare);
    }
}


template <typename T>
void merge(const T* begin_1, const T* end_1, const T* begin_2, const T* end_2, T* result_begin,
           bool (* compare)(const T&, const T&)) {
    size_t range_1 = end_1 - begin_1;
    size_t range_2 = end_2 - begin_2;

    size_t cnt_1 = 0;
    size_t cnt_2 = 0;

    for (size_t i = 0; i != range_1 + range_2; ++i) {
        if (cnt_1 == range_1 && cnt_2 != range_2) {
            result_begin[i] = begin_2[cnt_2++];
        }else if (cnt_2 == range_2 && cnt_1 != range_1) {
            result_begin[i] = begin_1[cnt_1++];
        }else if (compare(begin_1[cnt_1], begin_2[cnt_2])) {
            result_begin[i] = begin_1[cnt_1++];
        } else {
            result_begin[i] = begin_2[cnt_2++];
        }
    }
}


void solve(size_t n, size_t k, std::istream &is, std::ostream &os,
           bool (*compare)(const int &, const int &)) {
    auto big_buffer = std::vector<int>(k * 2, 0);
    auto big_buffer_begin  = &(big_buffer[0]);
    auto big_buffer_end = &(big_buffer[big_buffer.size()]);

    auto small_buffer = std::vector<int>(k, 0);
    auto small_buffer_begin = &(small_buffer[0]);
    auto small_buffer_end = &(small_buffer[small_buffer.size()]);

    int temp = 0;
    size_t cnt = 0;

    for (; cnt != 2 * k; ++cnt) {
        is >> temp;
        big_buffer[cnt] = temp;
    }

    heapSort(big_buffer_begin, big_buffer_end, compare);

    for (size_t i = 0; i != k; ++i) {
        os << big_buffer[i] << ' ';
    }

    for (; cnt != n; ++cnt) {
        if (cnt % k == 0 && cnt != 2 * k) {
            heapSort(small_buffer_begin, small_buffer_end, compare);
            merge(big_buffer_begin + k, big_buffer_end,
                  small_buffer_begin, small_buffer_end,
                  big_buffer_begin, compare);

            for (size_t i = 0; i != k; ++i) {
                os << big_buffer[i] << ' ';
            }
        }

        is >> temp;
        small_buffer[cnt % k] = temp;
    }

    size_t rest_buffer_offset = n % k == 0 ? k : n % k;

    heapSort(small_buffer_begin, small_buffer_begin + rest_buffer_offset, compare);
    merge(big_buffer_begin + k, big_buffer_end,
          small_buffer_begin, small_buffer_begin + rest_buffer_offset,
          big_buffer_begin, defaultIsLess);

    for (size_t i = 0; i != k + rest_buffer_offset; ++i) {
        os << big_buffer[i] << ' ';
    }
}



void make_random_file(size_t n, size_t k, size_t set_num, std::ostream &os) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);

    std::srand((unsigned int)std::time(0));
    for (size_t i = 0; i != set_num; ++i) {
        auto start_vec = std::vector<int >();
        int buffer = 0;
        int term = 100;

        for (size_t j = 0; j != n; ++j) {
            if (j < k) {
                buffer = dis(gen);
                start_vec.push_back(buffer);
                os << buffer << ' ';
            } else {
                auto new_val = start_vec[j % k] + term;
                os << new_val << ' ';

                start_vec[j % k] = new_val;
            }
        }

        os << std::endl;
    }
}

void test(size_t n, size_t k, size_t set_num, std::istream& is,
          void (*test_func) (size_t, size_t, std::istream&, std::ostream&, bool (*) (const int&, const int&)),
          bool (* compare)(const int&, const int&)) {
    for (size_t i = 0; i != set_num; ++i) {
        test_func(n, k, is, std::cout, compare);
        std::cout << std::endl;
    }
}

void makeTest() {
    size_t n = 9;
    size_t k = 3;
    size_t set_num = 10;
    std::ofstream os("/home/artem/ClionProjects/algorithm-problems/module_2/task_5/test_input.txt");
    make_random_file(n, k, set_num, os);
    os.close();

    std::ifstream is("/home/artem/ClionProjects/algorithm-problems/module_2/task_5/test_input.txt");

    clock_t start, end;

    start = clock();
    test(n, k, set_num, is, solve, defaultIsLess);
    end = clock();

    std::cout << (double)(end - start) / (set_num * CLOCKS_PER_SEC);
}

int main() {

    std::istream& is = std::cin;
    //std::ifstream is("/home/artem/ClionProjects/algorithm-problems/module_2/task_5/input.txt");

    size_t n = 0;
    size_t k = 0;
    is >> n;
    is >> k;
    solve(n, k, is, std::cout, defaultIsLess);


    //makeTest();

    return 0;
}