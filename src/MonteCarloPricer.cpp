#include <cmath>
#include <random>
#include <chrono>

#include "../pricer/include/MonteCarloPricer.hpp"

MonteCarloResult MonteCarloPricer::price_v1(const Option& option, double spot, double r, double sigma) const
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::normal_distribution<double> dist(0.0, 1.0);

    double drift = (r - 0.5 * sigma * sigma) * option.getMaturite();
    double vol_term = sigma * std::sqrt(option.getMaturite());
    double actualization = std::exp(-r * option.getMaturite());
    double sum = 0.0;
    double sum_sq = 0.0;
    for (int i = 0; i < N; i++)
    {
        double Z = dist(gen);
        double spot_i = spot * std::exp(drift + vol_term * Z);
        double payoff_i = option.calculerPayoff(spot_i);
        double actualized_flow_i = actualization*payoff_i;
        sum += actualized_flow_i;
        sum_sq += actualized_flow_i * actualized_flow_i; 
    }
    double mean = sum / N;
    double variance = 1.0/(N-1) * (sum_sq - N * mean * mean);
    double error_margin = 1.96*std::sqrt(variance/N);
    return MonteCarloResult {mean, mean - error_margin, mean + error_margin};
}

MonteCarloResult MonteCarloPricer::price_v2(const Option& option, double spot, double r, double sigma) const
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::normal_distribution<double> dist(0.0, 1.0);

    double drift = (r - 0.5 * sigma * sigma) * option.getMaturite();
    double vol_term = sigma * std::sqrt(option.getMaturite());
    double actualization = std::exp(-r * option.getMaturite());
    double sum = 0.0;
    double sum_sq = 0.0;
    int M = N/2;
    for (int i = 0; i < M; i++)
    {
        double Z = dist(gen);
        double spot_i1 = spot * std::exp(drift + vol_term * Z);
        double spot_i2 = spot * std::exp(drift - vol_term * Z);
        double payoff_i1 = option.calculerPayoff(spot_i1);
        double payoff_i2 = option.calculerPayoff(spot_i2);
        double payoff_i = (payoff_i1 + payoff_i2) * 0.5;
        double actualized_flow_i = actualization*payoff_i;
        sum += actualized_flow_i ;
        sum_sq += actualized_flow_i * actualized_flow_i; 
    }
    double mean = sum / M;
    double variance = 1.0/(M-1) * (sum_sq - M * mean * mean);
    double error_margin = 1.96*std::sqrt(variance/M);
    return MonteCarloResult {mean, mean - error_margin, mean + error_margin};
}