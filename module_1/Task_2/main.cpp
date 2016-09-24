#include <iostream>

int find_max(int* arr, int arr_len) {
    int max_ind = 0;

    for (int i = 0; i != arr_len; ++i) {
        if (arr[i] > arr[max_ind]) {
            max_ind = i;
        }
    }

    return max_ind;
}

int main() {
    int arr_len = 0;
    std::cin >> arr_len;
    std::cout << std::endl;

    int* A_arr = new int[arr_len];
    int* B_arr = new int[arr_len];

    for (int i = 0; i != arr_len; ++i) {
        std::cin >> A_arr[i];
    }

    for (int i = 0; i != arr_len; ++i) {
        std::cin >> B_arr[i];
    }

    int A_max_ind = find_max(A_arr, arr_len);
    int B_max_ind = find_max(B_arr, arr_len);

    std::cout << A_max_ind << ' ' << B_max_ind << std::endl;

    delete[] A_arr;
    delete[] B_arr;
}