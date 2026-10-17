#include <iostream>
#include <vector>
#include <numeric>
#include "utils.hpp"

int main() {
    std::vector<int> values = {1, 2, 3, 4, 5};
    int sum = std::accumulate(values.begin(), values.end(), 0);

    std::cout << "Setup C++20 fonctionnel !" << std::endl;
    std::cout << "Somme des valeurs (1..5) = " << sum << std::endl;
    std::cout << "Test header (utils::add(10, 32)) = " << utils::add(10, 32) << std::endl;

    return 0;
}
