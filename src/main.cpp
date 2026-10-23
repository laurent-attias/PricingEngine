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
    OptionCall call(T, strike);
    std::cout << "prix via Black-Scholes : " << call.bs_price(spot, r, sigma) << "\n";
    MonteCarloPricer pricer(N);
    MonteCarloResult result_v1 = pricer.price_v1(call, spot, r, sigma);
    MonteCarloResult result_v2 = pricer.price_v2(call, spot, r, sigma);
    MonteCarloResult result_v3 = pricer.price_v3(call, spot, r, sigma);
    MonteCarloResult result_v4 = pricer.price_v4(call, spot, r, sigma, n);
    std::cout << "prix via Monte-Carlo naïf  : " << result_v1.price << "\n";
    std::cout << "intervalle de confiance : [" << result_v1.lower_bound << "," << result_v1.upper_bound << "]" << "\n";
    std::cout << "prix via Monte-Carlo avec variables antithétiques  : " << result_v2.price << "\n";
    std::cout << "intervalle de confiance : [" << result_v2.lower_bound << "," << result_v2.upper_bound << "]" << "\n";
    std::cout << "prix via Monte-Carlo avec variables de contrôle en une passe  : " << result_v3.price << "\n";
    std::cout << "intervalle de confiance : [" << result_v3.lower_bound << "," << result_v3.upper_bound << "]" << "\n";
    std::cout << "prix via Monte-Carlo avec variables de contrôle pilote  : " << result_v4.price << "\n";
    std::cout << "intervalle de confiance : [" << result_v4.lower_bound << "," << result_v4.upper_bound << "]" << "\n";
}
