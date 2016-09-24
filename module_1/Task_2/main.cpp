/*
 * Даны два массива целых числе одинаковой длины A[0..n-1] и B[0..n-1]. Необходимо найти первую пару индексов i0 и j0,
 * i0 <= j0, такую, что A[i0] + B[j0] = max{A[i] + B[j], где 0 <= i < n, 0 <= j < n, i <= j}.
 * Время работы - O(n)
 */


#include <iostream>

void find_max(int* arr_1, int* arr_2, int arr_len, int &ind_1, int &ind_2) {
    int arr_1_ind = 0;
    int global_arr_1_max_ind = 0;   // индекс максимального элемента в первом массиве

    int local_arr_1_max_ind = 0;    // индекс элемента в первом массиве, обеспечивающего максимальную сумму
    int local_arr_2_max_ind = 0;    // индекс элемента во втором массиве, обеспечивающего максимальную сумму

    int curr_max = INT32_MIN;       // текущее значение максимальной суммы

    for (int arr_2_ind = 0; arr_2_ind != arr_len; ++arr_2_ind) {
        while (arr_1_ind <= arr_2_ind) {
            if (arr_1[arr_1_ind] > arr_1[global_arr_1_max_ind]) {
                global_arr_1_max_ind = arr_1_ind;
            }

            if(arr_1[global_arr_1_max_ind] + arr_2[arr_2_ind] > curr_max) {
                local_arr_1_max_ind = global_arr_1_max_ind;
                local_arr_2_max_ind = arr_2_ind;

                curr_max = arr_1[global_arr_1_max_ind] + arr_2[arr_2_ind];
            }

            ++arr_1_ind;
        }
    }

    ind_1 = local_arr_1_max_ind;
    ind_2 = local_arr_2_max_ind;
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

    int A_max_ind = 0;
    int B_max_ind = 0;

    find_max(A_arr, B_arr, arr_len, A_max_ind, B_max_ind);

    std::cout << A_max_ind << ' ' << B_max_ind << std::endl;

    delete[] A_arr;
    delete[] B_arr;
}