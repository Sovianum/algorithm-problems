/*
 * Дан массив целых чисел A[0..n-1]. Известно, что на интервале [0, m] значения массива строго возрастают,
 * а на интервале [m, n-1] строго убывают. Найти m за O(log m).
 * 2 <= n <= 10000
 */

#include <iostream>
#include <assert.h>

int localize_peak(int* arr, int arr_len) {
    assert(arr_len > 0);

    for (int length = 1; length < arr_len; length *= 2) {
        if (arr[length - 1] < arr[length - 1]) {
            return length;
        }
    }

    return -1;
}

int find_peak(int *arr, int arr_len) {
    assert(arr_len > 0);

    int beg = 0;
    int end = arr_len;
    int mid = (beg + end) / 2;

    while (beg != mid) {
        if (arr[mid] > arr[mid - 1]) {
            beg = mid;
        } else {
            end = mid;
        }
        mid = (beg + end) / 2;
    }

    return mid;
}

void test_find_peak() {     // функция генерирует тесты
    int arr_max_size = 100;

    for (int arr_size = 2; arr_size != arr_max_size; ++arr_size) {
        for (int peak_coord = 0; peak_coord != arr_size; ++peak_coord) {
            int* arr = new int[arr_size];
            arr[0] = 0;

            for (int i = 1; i != arr_size; ++i) {
                if (i <= peak_coord) {
                    arr[i] = arr[i - 1] + 1;
                } else {
                    arr[i] = arr[i - 1] - 1;
                }
            }

            for (int j = 0; j != arr_size; ++j) {
                std::cout << arr[j] << ' ';
            }

            int found = 0;
            if ((found = find_peak(arr, arr_size)) == peak_coord) {
                std::cout << "success " << peak_coord << ' ' << found;
            } else {
                std::cout << "fail " << peak_coord << ' ' << found;
                return;
            }

            std::cout << std::endl;

            delete[] arr;
        }
    }
}

int main() {

    int arr_len = 0;
    std::cin >> arr_len;
    int* arr = new int[arr_len];

    for (int i = 0; i != arr_len; ++i) {
        std::cin >> arr[i];
    }

//    int peak = find_peak(arr, arr_len);

    int peak = 0;
    int length = localize_peak(arr, arr_len);
    if (length != -1) {
        peak = find_peak(arr + length / 2, length / 2);
    } else {
        peak = find_peak(arr, arr_len);
    }

    std::cout << peak;

    delete[] arr;
    return 0;


}