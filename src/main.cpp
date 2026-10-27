#include <iostream>
#include <windows.h>

#include "../pricer/include/Option.hpp"
#include "../pricer/include/MonteCarloPricer.hpp"

int main()
{
    SetConsoleOutputCP(65001);

    double T = 1.0;
    double strike = 100.0;
    double spot = 100.0;
    double r = 0.05;
    double sigma = 0.2;
    int N = 100000;
    int n = 1000;
    double epsilon = 1e-6;
    OptionCall call(T, strike);
    std::cout << "prix via Black-Scholes : " << call.bs_price(spot, r, sigma) << "\n";
    MonteCarloPricer pricer(N);
    double delta = call.delta(spot, r, sigma);
    MonteCarloResult bumping_result = pricer.delta_bumping(call, spot, r, sigma, epsilon);
    MonteCarloResult pathwise_result = pricer.delta_pathwise(call, spot, r, sigma);
    std::cout << "delta exact : " << delta << "\n";
    std::cout << "delta approché par méthode bumping : " << bumping_result.estimated_value << "\n";
    std::cout << "intervalle de confiance : [" << bumping_result.lower_bound << "," << bumping_result.upper_bound << "]" << "\n";
    std::cout << "delta approché par méthode pathwise" << pathwise_result.estimated_value << "\n";
    std::cout << "intervalle de confiance : [" << pathwise_result.lower_bound << "," << pathwise_result.upper_bound << "]" << "\n";
}
