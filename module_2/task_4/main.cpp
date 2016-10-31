#include <iostream>

/*
 * Реализуется метод встречных итераторов
 */
int partition(int* arr, int l, int r) {
    int pivot = arr[l];
    int i = l + 1;
    int j = r + 1;  // r не включаются

    while (true) {
        while (i < r && arr[i] < pivot) {
            ++i;
        }

        while (j > l && arr[j] > pivot) {
            --j;
        }

        if (i >= j) {
            std::swap(arr[l], arr[j]);
            return j;
        }

        std::swap(arr[i], arr[j]);
        ++i;
        --j;


    }
}

int findKStatistics(int* arr, int l, int r, int k) {
    int pivot_pos = partition(arr, l, r);

    if (pivot_pos == k) {
        return arr[pivot_pos];
    }

    if (pivot_pos > k) {
        return findKStatistics(arr, l, pivot_pos, k);
    }

    return findKStatistics(arr, pivot_pos + 1, r, k)
}

int main() {
    int n = 0;
    int k = 0;
    std::cin >> n;
    std::cin >> k;
    int* arr = new int[n];

    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    std::cout >> findKStatistics(arr, 0, n, k);
    return 0;
}