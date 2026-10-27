#pragma once
#include <memory>
#include "Payoff.hpp"

struct MonteCarloResult
{
    double estimated_value;
    double lower_bound;
    double upper_bound;
};

class Option
{
protected:
    double maturite;
    std::unique_ptr<Payoff> payoff;
    double d_1(double spot, double r, double sigma) const;
    double d_2(double spot, double r, double sigma) const;

public:
    Option(double T, std::unique_ptr<Payoff> p) : maturite(T), payoff(std::move(p)) {}

    double calculerPayoff(double spot) const
    {
        return payoff->operator()(spot);
    }
    double derivePayoff(double spot) const
    {
        return payoff->derivative(spot);
    }

    double getMaturite() const
    {
        return maturite;
    }

    // Black-Scholes pricer
    virtual double bs_price(double spot, double r, double sigma) const = 0;

    // greeks
    virtual double delta(double spot, double r, double sigma) const = 0;
    virtual ~Option() = default;
    double gamma(double spot, double r, double sigma) const;
    double vega(double spot, double r, double sigma) const;
    virtual double theta(double spot, double r, double sigma) const = 0;
    virtual double rho(double spot, double r, double sigma) const = 0;
};

class OptionCall : public Option
{
public:
    OptionCall(double T, double strike) : Option(T, std::make_unique<PayoffCall>(strike)) {}
    double bs_price(double spot, double r, double sigma) const override;
    double delta(double spot, double r, double sigma) const override;
    double theta(double spot, double r, double sigma) const override;
    double rho(double spot, double r, double sigma) const override;
};

class OptionPut : public Option
{
public:
    OptionPut(double T, double strike) : Option(T, std::make_unique<PayoffPut>(strike)) {}
    double bs_price(double spot, double r, double sigma) const override;
    double delta(double spot, double r, double sigma) const override;
    double theta(double spot, double r, double sigma) const override;
    double rho(double spot, double r, double sigma) const override;
};