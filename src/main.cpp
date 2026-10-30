#include <iostream>
#include <windows.h>
#include <chrono>

#include "Option.hpp"
#include "MonteCarloPricer.hpp"

using std::cout;
using std::chrono::system_clock;

int main()
{
    SetConsoleOutputCP(65001);

    double T = 1.0;
    double strike = 100.0;
    double spot = 100.0;
    double r = 0.05;
    double sigma = 0.2;
    int N = 100000;
    int m = 1000;
    double epsilon = 1e-6;
    auto seed = system_clock::now().time_since_epoch().count();
    mt19937 gen(static_cast<unsigned int>(seed));
    OptionCallAsiat call(T, strike);
    MonteCarloPricer pricer(N);
    MonteCarloResult result = pricer.price_v5(call, spot, r, sigma, m, gen);
    cout << result ;
}
