/*
 * Реализуйте стратегию выбора опорного элемента "медиана трех". Функцию Partition реализуйте
 * методом прохода двумя итераторами от начала массива к концу
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <ctime>
#include <algorithm>
#include <assert.h>

template <class T>
void insertionSort(T* begin, T* end, std::function<bool(const T&, const T&)> isLess) {
    for (size_t outer_index = 1; outer_index != end - begin; ++outer_index) {
        T tmp = begin[outer_index];

        size_t inner_index;
        for (inner_index = outer_index; inner_index > 0 && isLess(tmp, begin[inner_index - 1]); --inner_index) {
            begin[inner_index] = begin[inner_index - 1];
        }

        begin[inner_index] = tmp;
    }
}

template <class T>
bool defaultIsLess(const T& item_1, const T& item_2) {
    return item_1 < item_2;
}

template <class T>
bool defaultIsGreater(const T& item_1, const T& item_2) {
    return item_1 > item_2;
}

template<typename T>
size_t median3Strategy(T* begin, T* end,
                       std::function<bool(const T&, const T&)> isLess) {
    auto vec = std::vector<size_t > {0, (end - begin - 1) / 2, end - begin - 1};

    auto comparator = [begin, isLess](const size_t & ind_1, const size_t & ind_2) -> bool { return isLess(begin[ind_1], begin[ind_2]);};
    insertionSort<size_t >(&(vec[0]), &(vec[vec.size()]), comparator);

    return vec[1];
}

template<typename T>
size_t unidirectionalPartition(T* begin, T* end, size_t pivotInd, bool (* isGreater) (const T&, const T&)=defaultIsGreater) {
    T pivot = begin[pivotInd];
    std::swap(begin[pivotInd], end[-1]);

    size_t i = 0;
    size_t j = 0;
    while (j != end - begin - 1) {
        if (isGreater(begin[j], pivot)) {
            ++j;
        } else {
            std::swap(begin[i], begin[j]);
            ++i;
            ++j;
        }
    }

    std::swap(begin[i], end[-1]);

    return i;

}

template<typename T>
T nonRecursiveFindKStatistics(T* begin, T* end, size_t k,
                                size_t (* pivotPosStrategy) (T*, T*, std::function<bool(const T&, const T&)>)=median3Strategy,
                                size_t (* partition) (T*, T*, size_t, bool (*) (const T&, const T&))=unidirectionalPartition,
                                bool (* isLess) (const T&, const T&)=defaultIsLess,
                                bool (* isGreater) (const T&, const T&)=defaultIsGreater) {

    size_t l = 0;
    size_t r = end - begin;
    size_t init_pivot_pos = pivotPosStrategy(begin, end, (std::function<bool(const T&, const T&)>)isLess);
    size_t pivot_pos = partition(begin, end, init_pivot_pos, isGreater);

    while (pivot_pos + l != k) {
        if (pivot_pos + l > k) {
            r = pivot_pos + l;
        } else {
            l = pivot_pos + l + 1;
        }

        assert(r > l);

        pivot_pos = partition(begin + l, begin + r, k - l, isGreater);
    }


    return begin[pivot_pos + l];
}


void readIn(std::istream& is, size_t n, std::vector<long>& vec) {
    int item = 0;
    for (size_t i = 0; i != n; ++i) {
        is >> item;
        vec.push_back(item);
    }
}


void make_random_file(size_t item_num, size_t set_num, std::ofstream &ofs) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    std::srand((unsigned int)std::time(0));
    for (size_t i = 0; i != set_num; ++i) {
        for (size_t j = 0; j != item_num; ++j) {
            ofs << dis(gen) << ' ';
        }

        ofs << std::endl;
    }
}

void make_one_test(size_t n, size_t k, std::istream &is) {
    std::vector<long> vec = std::vector<long>();
    readIn(is, n, vec);

    long my = nonRecursiveFindKStatistics(&(vec[0]), &(vec[vec.size()]), k);

    std::sort(vec.begin(), vec.end());
    long real = vec[k];

    if (real != my) {
        std::cout << "Fail" << std::endl;
        return;
    }

    return;
}

void make_all_tests() {
    size_t line_number = 1000;
    size_t num_number = 100;
    size_t cnt = 0;

    for (size_t n = 3; n != num_number; ++n) {
        std::ofstream os("/home/artem/ClionProjects/algorithm-problems/module_2/task_4/test_input");
        make_random_file(n, line_number, os);
        os.close();

        for (size_t k = 0; k != n; ++k) {
            std::ifstream is("/home/artem/ClionProjects/algorithm-problems/module_2/task_4/test_input");
            for (size_t i = 0; i != line_number; ++i) {
                std::vector<long> vec = std::vector<long>();
                readIn(is, n, vec);
                make_one_test(n, k, is);
                std::cout << ++cnt << std::endl;
            }
            is.close();
        }
    }
}

void test_killer_seq() {
    std::ifstream is("/home/artem/ClionProjects/algorithm-problems/module_2/task_4/input.txt");
    size_t n = 0;
    size_t k = 0;
    is >> n;
    is >> k;
    std::vector<long> vec = std::vector<long>();
    readIn(is, n, vec);
    std::cout << nonRecursiveFindKStatistics(&(vec[0]), &(vec[vec.size()]), k);
}



int main() {
    //make_all_tests();
    //test_killer_seq();

    size_t n = 0;
    size_t k = 0;
    std::cin >> n;
    std::cin >> k;
    std::vector<long> vec = std::vector<long>();

    readIn(std::cin, n, vec);

    std::cout << nonRecursiveFindKStatistics(&(vec[0]), &(vec[vec.size()]), k);
    return 0;
}