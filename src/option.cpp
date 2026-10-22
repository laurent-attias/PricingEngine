#include <cmath>
#include <random>
#include <chrono>

#include "../pricer/include/Option.hpp"
#include "../pricer/include/MathUtils.hpp"

double Option::d_1(double spot, double r, double sigma) const
{
    return (std::log(spot / payoff->getStrike()) + (r + 0.5 * sigma * sigma) * maturite) / (sigma * std::sqrt(maturite));
}

double Option::d_2(double spot, double r, double sigma) const
{
    return d_1(spot, r, sigma) - sigma * std::sqrt(maturite);
}

double Option::gamma(double spot, double r, double sigma) const
{
    return norm_pdf(d_1(spot, r, sigma)) / (spot * sigma * std::sqrt(maturite));
}
double Option::vega(double spot, double r, double sigma) const
{
    return spot * std::sqrt(maturite) * norm_pdf(d_1(spot, r, sigma));
}

MonteCarloResult Option::mc_price(double spot, double r, double sigma, int N) const
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::normal_distribution<double> dist(0.0, 1.0);

    double drift = (r - 0.5 * sigma * sigma) * maturite;
    double vol_term = sigma * std::sqrt(maturite);
    double actualization = std::exp(-r * maturite);
    double sum = 0.0;
    double sum_sq = 0.0;
    for (int i = 0; i < N; ++i)
    {
        double Z = dist(gen);
        double spot_i = spot * std::exp(drift + vol_term * Z);
        double payoff_i = (*payoff)(spot_i);
        double actualized_flow_i = actualization*payoff_i;
        sum += actualized_flow_i ;
        sum_sq += actualized_flow_i * actualized_flow_i;
    }
    double mean = sum / N;
    double variance = 1.0/(N-1) * (sum_sq - N * mean * mean);
    double error_margin = 1.96*std::sqrt(variance/N);
    return MonteCarloResult {mean, mean - error_margin, mean + error_margin};
}

double OptionCall::bs_price(double spot, double r, double sigma) const
{
    return spot * norm_cdf(d_1(spot, r, sigma)) - payoff->getStrike() * std::exp(-r * maturite) * norm_cdf(d_2(spot, r, sigma));
}

double OptionCall::delta(double spot, double r, double sigma) const
{
    return norm_cdf(d_1(spot, r, sigma));
}
double OptionCall::theta(double spot, double r, double sigma) const
{
    return -spot * sigma * norm_pdf(d_1(spot, r, sigma)) / (2 * std::sqrt(maturite)) - r * payoff->getStrike() * std::exp(-r * maturite) * norm_cdf(d_2(spot, r, sigma));
}
double OptionCall::rho(double spot, double r, double sigma) const
{
    return payoff->getStrike() * maturite * std::exp(-r * maturite) * norm_cdf(d_2(spot, r, sigma));
}

double OptionPut::bs_price(double spot, double r, double sigma) const
{
    return payoff->getStrike() * std::exp(-r * maturite) * norm_cdf(-d_2(spot, r, sigma)) - spot * norm_cdf(-d_1(spot, r, sigma));
}
double OptionPut::delta(double spot, double r, double sigma) const
{
    return -norm_cdf(-d_1(spot, r, sigma));
}
double OptionPut::theta(double spot, double r, double sigma) const
{
    return -spot * sigma * norm_pdf(d_1(spot, r, sigma)) / (2 * std::sqrt(maturite)) + r * payoff->getStrike() * std::exp(-r * maturite) * norm_cdf(-d_2(spot, r, sigma));
}
double OptionPut::rho(double spot, double r, double sigma) const
{
    return -payoff->getStrike() * maturite * std::exp(-r * maturite) * norm_cdf(-d_2(spot, r, sigma));
}