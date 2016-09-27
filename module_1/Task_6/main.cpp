/*
 * Дано N кубиков. Требуется определить, каким количеством способов можно выстроить из этих
 * кубиков пинамиду. Каждый вышележащий слой пирамиды должен быть строго меньше нижележащего
 */

#include <iostream>
#include <assert.h>

void inner_get_var_num(int cubes_left, int last_row, int& var_num) {
    assert(cubes_left >= 0);
    if (cubes_left == 0) {
        ++var_num;
        return;
    }

    for (int new_row = 1; new_row < last_row && new_row <= cubes_left; ++new_row) {    // решается задача с условием <=
        inner_get_var_num(cubes_left - new_row, new_row, var_num);
    }
}

int get_var_num(int cubes_num) {
    int var_num = 0;

    for (int first_row = 1; first_row <= cubes_num; ++first_row) {
        inner_get_var_num(cubes_num - first_row, first_row, var_num);
    }

    return var_num;
}

int main() {
    int n = 0;
    std::cin >> n;
    std::cout << get_var_num(n);
    return 0;
}