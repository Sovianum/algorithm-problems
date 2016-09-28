/*
 * Дано N кубиков. Требуется определить, каким количеством способов можно выстроить из этих
 * кубиков пинамиду. Каждый вышележащий слой пирамиды должен быть строго меньше нижележащего
 */

#include <iostream>
#include <assert.h>

void inner_get_var_num(int cubes_left, int last_row, long& var_num, long* result_table, int table_length) {
    assert(cubes_left >= 0);
    int cell_coord = cubes_left * table_length + last_row;
    if (result_table[cell_coord] >= 0) {
        var_num += result_table[cell_coord];
        return;
    }

    long local_var_num = var_num;    // переменная нужна для того, чтобы внести запись в result_table

    if (cubes_left == 0) {
        ++var_num;
        return;
    }

    for (int new_row = 1; new_row < last_row && new_row <= cubes_left; ++new_row) {
        inner_get_var_num(cubes_left - new_row, new_row, var_num, result_table, table_length);
    }

    result_table[cell_coord] = var_num - local_var_num;
}

long int get_var_num(int cubes_num) {
    long var_num = 0;
    long* result_table = new long[(cubes_num + 1) * (cubes_num + 1)];
    for (int i = 0; i <= cubes_num * cubes_num; ++i) {
        result_table[i] = -1;
    }

    for (int first_row = 1; first_row <= cubes_num; ++first_row) {
        inner_get_var_num(cubes_num - first_row, first_row, var_num, result_table, cubes_num);
    }

    delete[] result_table;
    return var_num;
}

int main() {
    int n = 0;
    std::cin >> n;
    std::cout << get_var_num(n);
    return 0;
}