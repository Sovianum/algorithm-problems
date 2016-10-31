#include <iostream>
#include <fstream>
#include <vector>


template <class T>
void insertionSort(T* begin, T* end, bool (* isLess)(const T&, const T&)) {
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

struct Point {
    int x;
    int y;

    Point(): x(0), y(0){};
    Point(int x, int y): x(x), y(y){};
};

bool operator < (const Point& point_1, const Point& point_2) {
    if (point_1.x != point_2.x) {
        return point_1.x < point_2.x;
    } else {
        return point_1.y < point_2.y;
    }
}

std::istream& operator >> (std::istream& is, Point& point) {
    is >> point.x >> point.y;

    return is;
}

std::ostream& operator << (std::ostream& os, Point& point) {
    os << point.x << ' ' << point.y << std::endl;

    return os;
}

void read_points(std::istream &is, std::vector<Point> &point_vec, int &point_num) {
    is >> point_num;

    for (int i = 0; i != point_num; ++i) {
        Point point = Point();
        is >> point;

        point_vec.push_back(point);
    }
}

int main() {
    //std::ifstream ifs("/home/artem/ClionProjects/algorithm-problems/module_2/task_1/input.txt");

    std::vector<Point> point_vec;
    int point_num;

    read_points(std::cin, point_vec, point_num);
    insertionSort(&point_vec[0], &point_vec[0] + point_num, defaultIsLess);

    for (int i = 0; i != point_num; ++i) {
        std::cout << point_vec[i];
    }

}