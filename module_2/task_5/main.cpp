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
bool defaultIsGreater(const T& item_1, const T& item_2) {
    return item_1 > item_2;
}

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

size_t parent_index(size_t index) {
    return (index - 1) / 2;
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
void heapifyBottom(T* begin, T* end, bool (* compare)(const T&, const T&)) {

    size_t item_index = end - begin - 1;
    while (true) {
        if (item_index == 0) {
            break;
        }

        if (compare(begin[parent_index(item_index)], begin[item_index])) {
            std::swap(begin[parent_index(item_index)], begin[item_index]);

            item_index = parent_index(item_index);
        } else {
            break;
        }
    }
}

template <class T>
void add(T* begin, T*& end, T value, bool (* compare)(const T&, const T&)) {
    *(end++) = value;
    heapifyBottom(begin, end, compare);
}

template <class T>
T extractTop(T *begin, T *&end, bool (*compare)(const T &, const T &)) {
    std::swap(*begin, *(--end));
    auto result = *end;

    heapifyTop(begin, end, begin, compare);

    return result;
}

template <class T>
void fullHeapify(T *begin, T *end, bool (*compare)(const T &, const T &)) {
    for (std::ptrdiff_t i = (end - begin) / 2; i >= 0; --i) {
        heapifyTop(begin, end, begin + i, compare);
    }
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
        }

        if (cnt_2 == range_2 && cnt_1 != range_1) {
            result_begin[i] = begin_1[cnt_1++];
        }

        if (compare(begin_1[cnt_1], begin_2[cnt_2])) {
            result_begin[i] = begin_1[cnt_1++];
        } else {
            result_begin[i] = begin_2[cnt_2++];
        }
    }
}


void solve2(size_t n, size_t k, std::istream &is, std::ostream &os,
            bool (* compare)(const int&, const int&)) {
    const size_t series_len = k;

    auto big_buffer = std::vector<int>(series_len * 2, 0);
    auto big_buffer_begin  = &(big_buffer[0]);
    auto big_buffer_end = &(big_buffer[big_buffer.size()]);

    auto small_buffer = std::vector<int>(series_len, 0);
    auto small_buffer_begin = &(small_buffer[0]);
    auto small_buffer_end = &(small_buffer[small_buffer.size()]);
    int temp = 0;
    size_t cnt = 0;

    for (; cnt != 2 * series_len; ++cnt) {
        is >> temp;
        big_buffer[cnt] = temp;
    }

    heapSort(big_buffer_begin, big_buffer_end, compare);

    for (size_t i = 0; i != series_len; ++i) {
        os << big_buffer[i] << ' ';
    }

    for (; cnt != n; ++cnt) {
        if (cnt % series_len == 0 && cnt != 2 * series_len) {
            heapSort(small_buffer_begin, small_buffer_end, compare);
            merge(big_buffer_begin + series_len, big_buffer_end,
                  small_buffer_begin, small_buffer_end,
                  big_buffer_begin, compare);

            for (size_t i = 0; i != series_len; ++i) {
                os << big_buffer[i] << ' ';
            }
        }

        is >> temp;
        small_buffer[cnt % series_len] = temp;
    }

    heapSort(small_buffer_begin, small_buffer_begin + n % k, compare);
    merge(big_buffer_begin + series_len, big_buffer_end,
          small_buffer_begin, small_buffer_begin + n % k,
          big_buffer_begin, defaultIsLess);

    for (size_t i = 0; i != series_len + n % k; ++i) {
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
    size_t n = 10;
    size_t k = 3;
    size_t set_num = 10;
    //std::ofstream os("/home/artem/ClionProjects/algorithm-problems/module_2/task_5/test_input.txt");
    //make_random_file(n, k, set_num, os);
    //os.close();

    std::ifstream is("/home/artem/ClionProjects/algorithm-problems/module_2/task_5/test_input.txt");

    clock_t start, end;

    start = clock();
    test(n, k, set_num, is, solve2, defaultIsLess);
    end = clock();

    std::cout << (double)(end - start) / (set_num * CLOCKS_PER_SEC);
}

int main() {
/*
    //std::istream& is = std::cin;
    std::ifstream is("/home/artem/ClionProjects/algorithm-problems/module_2/task_5/input.txt");

    size_t n = 0;
    size_t k = 0;
    is >> n;
    is >> k;
    solve2(n, k, is, std::cout, defaultIsLess);
*/

    makeTest();

    return 0;
}

/*
 *
void solve(size_t n, size_t k, std::istream& is=std::cin, std::ostream& os=std::cout) {
    auto buffer = std::vector<int >();

    int temp = 0;
    for (size_t i = 0; i != k; ++i) {
        is >> temp;
        buffer.push_back(temp);
    }

    fullHeapify(&(buffer[0]), &(buffer[buffer.size()]), defaultIsGreater);

    int * begin = &(buffer[0]);
    int * end = &(buffer[buffer.size()]);
    for (size_t i = 0; i < n - k; ++i) {
        int top = extractTop<int >(begin, end, defaultIsGreater);
        os << top << ' ';

        is >> temp;
        add<int >(begin, end, temp, defaultIsGreater);
    }

    for (size_t i = 0; i != k; ++i) {
        int out = extractTop<int >(begin, end, defaultIsGreater);
        os << out << ' ';
    }
}


void solve1(size_t n, size_t k, std::istream &is = std::cin, std::ostream &os = std::cout) {
    auto series_len = k + 1;

    auto buffer = std::vector<int >(series_len, 0);
    int temp = 0;

    for (size_t i = 0; i < n; ++i) {
        is >> temp;
        buffer[i % series_len] = temp;

        if ((i + 1) % series_len == 0) {
            heapSort(&(buffer[0]), &(buffer[buffer.size()]), defaultIsLess);

            for (auto item: buffer) {
                os << item << ' ';
            }
        }
    }

    heapSort(&(buffer[0]), &(buffer[n % series_len]), defaultIsLess);

    for (size_t i = 0; i != n % series_len; ++i) {
        os << buffer[i] << ' ';
    }
}
 */