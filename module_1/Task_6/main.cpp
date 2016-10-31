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

long int alt_inner_get_var_num(int cubes_left, int max_last_row, long *result_table, int table_length) {
    if (cubes_left < 0 or max_last_row == 0) {
        return 0;
    }

    int cell_coord = cubes_left * table_length + max_last_row;

    if (result_table[cell_coord] == -1) {
        if (cubes_left == 0) {
            return 1;
        } else if (cubes_left == 1) {
            result_table[cell_coord] = 0;
        } else {
            result_table[cell_coord] =
                    alt_inner_get_var_num(cubes_left - max_last_row, max_last_row, result_table, table_length) +
                    alt_inner_get_var_num(cubes_left, max_last_row - 1, result_table, table_length);
        }
    }

    return result_table[cell_coord];
}

long int alt_get_var_num(int cubes_num) {
    long var_num = 0;
    long* result_table = new long[(cubes_num + 2) * (cubes_num + 2)];
    for (int i = 0; i <= (cubes_num + 1) * (cubes_num + 1); ++i) {
        result_table[i] = -1;
    }

    var_num = alt_inner_get_var_num(cubes_num, cubes_num, result_table, cubes_num);
/*
    for (int i = 0; i <= cubes_num; ++i) {
        for (int j = 0; j <= cubes_num; ++j) {
            if (result_table[cubes_num * i + j] == -1) {
                std::cout << "? ";
            } else {
                std::cout << result_table[cubes_num * i + j] << ' ';
            }
        }
        std::cout << std::endl;
    }
*/

    delete[] result_table;
    return var_num;
}

void test() {
    for (int i = 1; i != 20; ++i) {
        if (get_var_num(i) == alt_get_var_num(i)) {
            std::cout << i << " success" << std::endl;
        } else {
            std::cout << i << " fail: " << "true: " << get_var_num(i) << " obtained: " << alt_get_var_num(i) << std::endl;
            //break;
        }
    }
}

int main() {
    int n = 0;
    std::cin >> n;
    std::cout << get_var_num(n);
    return 0;
}