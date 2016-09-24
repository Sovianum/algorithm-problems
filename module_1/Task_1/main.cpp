/*
 * Вывести разложение натурального числа n на простые множители.
 * Простые множители должны быть упорядочены по возрастанию и разделены пробелами.
 * 2 <= n <= 1e6
 */

#include <iostream>
#include <assert.h>

int get_prime_factors(int n, int* prime_factors) {
    assert(n > 1);

    int factor_cnt = 0;
    int factor = 2;
    int local_n = n;
    while (factor * factor <= local_n) {
        if (local_n % factor == 0) {
            prime_factors[factor_cnt++] = factor;
            local_n /= factor;  // делим исходное число на найденный делитель
            continue;
        }

        ++factor;
    }

    if (local_n != 1) {
        prime_factors[factor_cnt++] = local_n;
    }

    return factor_cnt;
}

int main() {
    int n = 0;
    std::cin >> n;
    std::cout << std::endl;

    int* factors = new int[n];
    for (int i = 0; i != n; ++i) {
        factors[i] = 0;
    }

    int factor_num = get_prime_factors(n, factors);

    for (int i =0; i != factor_num; ++i) {
        std::cout << factors[i] << ' ';
    }

    delete [] factors;
    return 0;
}