#include <iostream>
#include "../pricer/include/Option.hpp"

int main()
{
    double T = 1.0;
    double strike = 100.0;
    double spot = 100.0;
    double r = 0.05;
    double sigma = 0.2;
    int N = 100000;
    OptionCall call(T, strike);
    std::cout << "prix via Black-Scholes : " << call.bs_price(spot, r, sigma) << "\n";
    MonteCarloResult result = call.mc_price(spot, r, sigma, N);
    std::cout << "prix via Monte-Carlo  : " << result.price << "\n";
    std::cout << "intervalle de confiance : [" << result.lower_bound << "," << result.upper_bound << "]" << "\n";
}
