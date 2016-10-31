/*
 * На вокзале есть некоторое количество тупиков, куда прибывают электрички. Этот вокзал является их
 * конечной станцией. Дано расписание движения электричек, в котором для каждой электрички
 * указано время ее прибытия. Когда электричка прибывает, ее ставят в свободный тупик с
 * минимальным номером. При этом если электричка из какого-то тупика отправилась в момент
 * времени Х, то электричку, которая прибывает в момент времени Х, в этот тупик ставить нельзя, а
 * электричку, прибывающую в момент Х+1 - можно.
 * В данный момент на вокзале достаточное количество тупиков для работы по расписанию.
 * Напишите момент, которая по данному расписанию определяет, какое минимальное количество
 * тупиков требуется для работы вокзала.
 *
 * Формат входных данных. Вначале вводится n - количество электричек в расписании. Затем вводится
 * n строк для каждой электрички, в строке - время прибытия и время отправления. Время -
 * натуральное число от 0 до 10^9. Строки в расписании упорядочены по времени прибытия.
 *
 * Формат выходных данных. Натуральное число - минимальное количество тупиков.
 *
 * Максимальное время: 50 мс, память: 5 Мб.
 */

#include <iostream>
#include <fstream>
#include <vector>

template <class T>
bool defaultIsLess(const T& item_1, const T& item_2) {
    return item_1 < item_2;
}

template <class T>
bool defaultIsGreater(const T& item_1, const T& item_2) {
    return item_1 > item_2;
}

template <class T>
bool defaultIsNotGreater(const T& item_1, const T& item_2) {
    return item_1 <= item_2;
}

template <class T>
class Heap {
public:
    Heap(): buffer(), isLess(defaultIsLess) {}

    Heap(bool (* comparator)(const T&, const T&)): buffer(), isLess(comparator) {};

    const T& getMax() { return buffer[0]; };

    T extractMax();

    void add(T);

    bool empty() {
        return buffer.empty();
    }
private:
    size_t parent_index(size_t index) { return (index - 1) / 2; };
    size_t left_child_index(size_t index) { return 2 * index + 1; };
    size_t right_child_index(size_t index) { return 2 * index + 2; };

    void heapifyBottom(size_t item_index);
    void heapifyTop();

    std::vector<T> buffer;
    bool (* isLess)(const T&, const T&);
};

template <class T>
T Heap<T>::extractMax() {
    std::swap(buffer[0], buffer[buffer.size() - 1]);
    auto result = buffer[buffer.size() - 1];
    buffer.pop_back();

    heapifyTop();

    return result;
}

template <class T>
void Heap<T>::add(T value) {
    buffer.push_back(value);
    heapifyBottom(buffer.size() - 1);
}

template <class T>
void Heap<T>::heapifyBottom(size_t item_index) {
    while (true) {
        if (item_index == 0) {
            break;
        }

        if (isLess(buffer[parent_index(item_index)], buffer[item_index])) {
            std::swap(buffer[parent_index(item_index)], buffer[item_index]);

            item_index = parent_index(item_index);
        } else {
            break;
        }
    }
}

template <class T>
void Heap<T>::heapifyTop() {
    size_t root_ind = 0;
    size_t left_child_ind = left_child_index(root_ind);
    size_t right_child_ind = right_child_index(root_ind);
    size_t max_child_ind;

    while (true) {
        if (right_child_ind < buffer.size()) {
            max_child_ind = isLess(buffer[left_child_ind], buffer[right_child_ind]) ? right_child_ind : left_child_ind;
        } else if (left_child_ind < buffer.size()) {
            max_child_ind = left_child_ind;
        } else {
            return;
        }

        if (isLess(buffer[root_ind], buffer[max_child_ind])) {
            std::swap(buffer[root_ind], buffer[max_child_ind]);

            root_ind = max_child_ind;
            left_child_ind = left_child_index(root_ind);
            right_child_ind = right_child_index(root_ind);
        } else {
            return;
        }
    }
}

struct Request {
    int time_start;
    int time_finish;

    Request(): time_start(0), time_finish(0) {};
};

bool operator < (const Request& request_1, const Request& request_2) {
    return request_1.time_finish < request_2.time_finish; // TODO проверить функционал оператора
}

bool operator > (const Request& request_1, const Request& request_2) {
    return request_1.time_finish > request_2.time_finish; // TODO проверить функционал оператора
}

bool operator <= (const Request& request_1, const Request& request_2) {
    return request_1.time_finish <= request_2.time_finish;  // TODO проверить функционал оператора
}

std::ostream& operator << (std::ostream& os, Request& request) {
    os << request.time_start << ' ' << request.time_finish << std::endl;
    return os;
}

std::istream& operator >> (std::istream& is, Request& request) {
    is >> request.time_start >> request.time_finish;
    return is;
}


void addRequest(const Request& request, Heap<Request>& heap, int& requests_pending, int& max_requests_pending) {
    if (heap.empty()) {
        heap.add(request);

        ++requests_pending;
        if (requests_pending > max_requests_pending) {
            max_requests_pending = requests_pending;
        }
        return;
    }

    auto top_request = heap.getMax();

    while (top_request.time_finish < request.time_start) {
        heap.extractMax();
        --requests_pending;
        top_request = heap.getMax();
    }

    /*
    if (request.time_start <= top_request.time_finish) {

        heap.add(request);

        ++requests_pending;
        if (requests_pending > max_requests_pending) {
            max_requests_pending = requests_pending;
        }
    }
     */

    heap.add(request);

    ++requests_pending;
    if (requests_pending > max_requests_pending) {
        max_requests_pending = requests_pending;
    }
}


void testHeap() {
    std::ifstream ifs("/home/artem/ClionProjects/algorithm-problems/module_2/task_2/test_heap.txt");
    Heap<int> heap;

    int val;

    while(ifs >> val) {
        heap.add(val);
    }
    while(!heap.empty()) {
        std::cout << heap.extractMax() << ' ';
    }
}

void testSolve() {
    std::ifstream ifs("/home/artem/ClionProjects/algorithm-problems/module_2/task_2/test_heap.txt");
    int requests_pending = 0;
    int max_requests_pending = 0;

    Heap<Request> heap(defaultIsGreater);
    Request request;

    while (ifs >> request) {
        addRequest(request, heap, requests_pending, max_requests_pending);
    }

    std::cout << max_requests_pending;
}

int main() {
    testHeap();
    testSolve();

    int request_num;
    int requests_pending = 0;
    int max_requests_pending = 0;

    std::cin >> request_num;

    Heap<Request> heap(defaultIsGreater);
    Request request;
    for (int i = 0; i != request_num; ++i) {
        std::cin >> request;
        addRequest(request, heap, requests_pending, max_requests_pending);
    }

    std::cout << max_requests_pending;
    return 0;
}