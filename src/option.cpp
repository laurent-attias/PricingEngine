#include <cmath>
#include <random>
#include <chrono>
#include <stdexcept>

#include "Option.hpp"
#include "MathUtils.hpp"

using std::exp;
using std::log;
using std::sqrt;
using std::runtime_error;

double OptionEurope::d_1(double spot, double r, double sigma) const
{
    return (log(spot / payoff->getStrike()) + (r + 0.5 * sigma * sigma) * maturite) / (sigma * sqrt(maturite));
}

double OptionEurope::d_2(double spot, double r, double sigma) const
{
    return d_1(spot, r, sigma) - sigma * sqrt(maturite);
}

double OptionEurope::derivePayoff(double spot) const
{
    auto* payoff_europe = dynamic_cast<const PayoffEurope*>(payoff.get());
    if (!payoff_europe)
    {
        throw runtime_error("Erreur : le payoff associé n'est pas un PayoffEurope");
    }
    return payoff_europe->derivative(spot);
}


double OptionEurope::gamma(double spot, double r, double sigma) const
{
    return norm_pdf(d_1(spot, r, sigma)) / (spot * sigma * sqrt(maturite));
}
double OptionEurope::vega(double spot, double r, double sigma) const
{
    return spot * sqrt(maturite) * norm_pdf(d_1(spot, r, sigma));
}

double OptionCallEurope::bs_price(double spot, double r, double sigma) const
{
    return spot * norm_cdf(d_1(spot, r, sigma)) - payoff->getStrike() * exp(-r * maturite) * norm_cdf(d_2(spot, r, sigma));
}

double OptionCallEurope::delta(double spot, double r, double sigma) const
{
    return norm_cdf(d_1(spot, r, sigma));
}
double OptionCallEurope::theta(double spot, double r, double sigma) const
{
    return -spot * sigma * norm_pdf(d_1(spot, r, sigma)) / (2 * sqrt(maturite)) - r * payoff->getStrike() * exp(-r * maturite) * norm_cdf(d_2(spot, r, sigma));
}
double OptionCallEurope::rho(double spot, double r, double sigma) const
{
    return payoff->getStrike() * maturite * exp(-r * maturite) * norm_cdf(d_2(spot, r, sigma));
}

double OptionPutEurope::bs_price(double spot, double r, double sigma) const
{
    return payoff->getStrike() * exp(-r * maturite) * norm_cdf(-d_2(spot, r, sigma)) - spot * norm_cdf(-d_1(spot, r, sigma));
}
double OptionPutEurope::delta(double spot, double r, double sigma) const
{
    return -norm_cdf(-d_1(spot, r, sigma));
}
double OptionPutEurope::theta(double spot, double r, double sigma) const
{
    return -spot * sigma * norm_pdf(d_1(spot, r, sigma)) / (2 * sqrt(maturite)) + r * payoff->getStrike() * exp(-r * maturite) * norm_cdf(-d_2(spot, r, sigma));
}
double OptionPutEurope::rho(double spot, double r, double sigma) const
{
    return -payoff->getStrike() * maturite * exp(-r * maturite) * norm_cdf(-d_2(spot, r, sigma));
}
