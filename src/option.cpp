#include <cmath>
#include <random>
#include <chrono>

#include "Option.hpp"
#include "MathUtils.hpp"

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