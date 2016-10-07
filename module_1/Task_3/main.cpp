/*
 * Дан массив целых чисел A[0..n-1]. Известно, что на интервале [0, m] значения массива строго возрастают,
 * а на интервале [m, n-1] строго убывают. Найти m за O(log m).
 * 2 <= n <= 10000
 */

#include <iostream>
#include <assert.h>

void localize_peak(int *arr, int arr_len, int &left_offset, int &right_offset) {
    assert(arr_len > 0);
    left_offset = 0;
    right_offset = arr_len;

    for (int length = 1; length < arr_len; length *= 2) {
        if (arr[length - 1] < arr[length - 1]) {
            right_offset = length;
            left_offset = length / 2;
            return;
        }
    }

    return;
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

int main() {

    int arr_len = 0;
    std::cin >> arr_len;
    int* arr = new int[arr_len];

    for (int i = 0; i != arr_len; ++i) {
        std::cin >> arr[i];
    }

    int peak = 0;
    int left_offset = 0;
    int right_offset = 0;
    localize_peak(arr, arr_len, left_offset, right_offset);

    peak = find_peak(arr + left_offset, right_offset - left_offset);

    std::cout << peak;

    delete[] arr;
    return 0;


}