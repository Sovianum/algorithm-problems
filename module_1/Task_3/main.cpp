#include <iostream>
#include <assert.h>

int find_peak(int *arr, int arr_len) {
    assert(arr_len >= 0);

    int beg = 0;
    int end = arr_len;
    int mid = (beg + end) / 2;

    while (beg != mid) {
        if (arr[mid] < arr[mid + 1]) {
            beg = mid + 1;
        } else {
            end = mid;
        }
        mid = (beg + end) / 2;
    }

    return mid;
}

int main() {
    int* arr = new int[10];
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 4;
    arr[4] = 5;
    arr[5] = 6;
    arr[6] = 7;
    arr[7] = 6;
    arr[8] = 5;
    arr[9] = 4;

    int peak = find_peak(arr, 10);

    std::cout << peak << ' ' << arr[peak];
    return 0;
}