#include <iostream>
#include "../pricer/include/Option.hpp"

int main()
{
    double T = 1.0;
    double strike = 100.0;
    double spot = 100.0;
    double r = 0.05;
    double sigma = 0.2;
    OptionPut put(T, strike);
    OptionCall call(T, strike);
    std::cout << "rho Call : " << call.rho(spot, r, sigma) << "\n";
    std::cout << "rho Put  : " << put.rho(spot, r, sigma) << "\n";
}
