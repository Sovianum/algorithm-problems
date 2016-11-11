/*
 * Группа людей называется современниками если был такой момент, когда они могли собраться
 * вместе. Для этого в этот момент каждому из них должно было уже исполниться 18 лет, но еще не
 * исполниться 80 лет.
 * Дан список Жизни Великих Людей. Необходимо получить максимальное количество современников.
 * В день 18летия человек уже может принимать участие в собраниях, а в день 80летия и в день
 * смерти уже не может.
 *
 * Замечание. Человек мог не должить до 18летия, либо умереть в день 18летия. В этих случаях
 * принимать участие в собраниях он не мог.
 */

#include <cstddef>
#include <iostream>
#include <fstream>
#include <vector>

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
void heapifyTop(T* begin, T* end, T* start, bool (* isLess)(const T&, const T&)) {
    size_t root_ind = start - begin;
    size_t left_child_ind = left_child_index(root_ind);
    size_t right_child_ind = right_child_index(root_ind);
    size_t max_child_ind;

    size_t interval_length = end - begin;

    while (true) {
        if (right_child_ind < interval_length) {
            max_child_ind = isLess(begin[left_child_ind], begin[right_child_ind]) ? right_child_ind : left_child_ind;
        } else if (left_child_ind < interval_length) {
            max_child_ind = left_child_ind;
        } else {
            return;
        }

        if (isLess(begin[root_ind], begin[max_child_ind])) {
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
T extractMax(T* begin, T*& end, bool (* isLess)(const T&, const T&)) {
    std::swap(*begin, *end);
    auto result = *(end--);

    heapifyTop(begin, end, begin, isLess);

    return result;
}

template <class T>
void heapSort(T* begin, T* end, bool (* isLess)(const T&, const T&)=defaultIsLess) {
    for (std::ptrdiff_t i = (end - begin) / 2; i >= 0; --i) {
        heapifyTop(begin, end, begin + i, isLess);
    }

    for (size_t i = end - begin; i != 0; --i) {
        T top_elem = extractMax(begin, end, isLess);

        heapifyTop(begin, begin + i, begin, isLess);
        begin[i] = top_elem;
    }
}


struct LifePeriod {
    int birth_day;
    int birth_month;
    int birth_year;
    int death_day;
    int death_month;
    int death_year;

    LifePeriod(): birth_day(0), birth_month(0), birth_year(0),
                  death_day(0), death_month(0), death_year(0) {};

    int convertToInt(int years, int month, int days) const {
        return years * 10000 + month * 100 + days;
    }

    int getBirthValue() const {
        return convertToInt(birth_year, birth_month, birth_day);
    }

    int getStartValue() const {
        return convertToInt(birth_year + 18, birth_month, birth_day);
    }

    int getDeathValue() const {
        return convertToInt(death_year, death_month, death_day);
    }

    int getRetireValue() const {
        return convertToInt(birth_year + 80, birth_month, birth_day);
    }

    int getFinishValue() const {
        int death_value = getDeathValue();
        int retire_value = getRetireValue();

        return death_value < retire_value ? death_value : retire_value;
    }

    int getActiveLifeLength() const {
        int start_val = getStartValue();
        int finish_val = getFinishValue();

        return finish_val - start_val;
    }
};

struct TimePoint {
    int value;
    bool is_add_point;

    TimePoint(): value(0), is_add_point(false) {};

    TimePoint(int value, bool is_add_point): value(value), is_add_point(is_add_point) {};
};

bool operator<(const TimePoint& point_1, const TimePoint& point_2) {
    return point_1.value < point_2.value;
}

std::ostream& operator << (std::ostream& os, LifePeriod& request) {
    os << request.convertToInt(request.birth_year, request.birth_month, request.birth_day);
    return os;
}

std::istream& operator >> (std::istream& is, LifePeriod& request) {
    is >> request.birth_day >> request.birth_month >> request.birth_year;
    is >> request.death_day >> request.death_month >> request.death_year;
    return is;
}

bool operator<(const LifePeriod& item_1, const LifePeriod& item_2) {
    if (item_1.getStartValue() < item_2.getStartValue()) {
        return true;
    } else {
        return item_1.getFinishValue() < item_2.getFinishValue();
    }
}



void readIn(std::istream& is, size_t value_num, std::vector<LifePeriod>& vec) {
    LifePeriod life_period;

    for (size_t i = 0; i != value_num; ++i) {
        is >> life_period;
        if (life_period.getActiveLifeLength() > 0) {
            vec.push_back(life_period);
        }
    }
}

std::vector<TimePoint> getTimePoints(const std::vector<LifePeriod> &input_vec) {     // возвращает вектор точек времени
    std::vector<TimePoint> result;
    for (auto i = input_vec.cbegin(); i != input_vec.cend(); ++i) {
        result.push_back(TimePoint(i->getStartValue(), true));
        result.push_back(TimePoint(i->getFinishValue(), false));
    }

    return result;
}

int processTimeLine(std::vector<TimePoint>& time_points) {
    int val = 0;
    int max_val = 0;

    heapSort(&(time_points[0]), &(time_points[time_points.size() - 1]));

    for (auto i = time_points.cbegin(); i != time_points.cend(); ++i) {
        if (i->is_add_point) {
            ++val;
            //std::cout << '+';

            if (val > max_val) {
                max_val = val;
            }
        } else {
            --val;
            //std::cout << '-';
        }
    }

    return max_val;
}

int solve(std::vector<LifePeriod> input_vec) {
    auto time_points = getTimePoints(input_vec);
    return processTimeLine(time_points);
}

/*
void testHeapSort() {
    auto v = std::vector<int> {5, 2, 7, 2, 4,56, 78, 4, 3, 54,678, 6};
    heapSort(&(v[0]), &(v[v.size() - 1]));

    for (auto i = v.cbegin(); i != v.cend(); ++i) {
        std::cout << *i << ' ';
    }
}
*/


int main() {

    std::vector<LifePeriod> vec;
    size_t value_num = 0;
    //std::ifstream is("/home/artem/ClionProjects/algorithm-problems/module_2/task_3/input.txt");

    std::cin >> value_num;
    readIn(std::cin, value_num, vec);

    std::cout << solve(vec);
     /*

    testHeapSort();

     */
    return 0;
}