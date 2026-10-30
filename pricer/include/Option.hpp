#pragma once
#include <memory>
#include <vector>

#include "Payoff.hpp"

using std::make_unique;
using std::move;
using std::unique_ptr;
using std::vector;
using std::ostream;


class Option
{
protected:
    double maturite;
    unique_ptr<Payoff> payoff;

public:
    Option(double T, unique_ptr<Payoff> p) : maturite(T), payoff(move(p)) {}

    template <typename T>
    double calculerPayoff(const T& input) const
    {
        return payoff->operator()(input);
    }

    double getMaturite() const
    {
        return maturite;
    }
    virtual ~Option() = default;
};


class OptionEurope : public Option
{
protected:
    double d_1(double spot, double r, double sigma) const;
    double d_2(double spot, double r, double sigma) const;
public:
    OptionEurope(double T, unique_ptr<PayoffEurope> p) : Option(T, move(p)) {}
    double derivePayoff(double spot) const;

    // Black-Scholes pricer
    virtual double bs_price(double spot, double r, double sigma) const = 0;

    // greeks
    double gamma(double spot, double r, double sigma) const;
    double vega(double spot, double r, double sigma) const;
    virtual double delta(double spot, double r, double sigma) const = 0;
    virtual double theta(double spot, double r, double sigma) const = 0;
    virtual double rho(double spot, double r, double sigma) const = 0;
};



class OptionCallEurope : public OptionEurope
{
public:
    OptionCallEurope(double T, double strike) : OptionEurope(T, make_unique<PayoffCallEurope>(strike)) {}
    double bs_price(double spot, double r, double sigma) const override;
    double delta(double spot, double r, double sigma) const override;
    double theta(double spot, double r, double sigma) const override;
    double rho(double spot, double r, double sigma) const override;
};


class OptionPutEurope : public OptionEurope
{
public:
    OptionPutEurope(double T, double strike) : OptionEurope(T, make_unique<PayoffPutEurope>(strike)) {}
    double bs_price(double spot, double r, double sigma) const override;
    double delta(double spot, double r, double sigma) const override;
    double theta(double spot, double r, double sigma) const override;
    double rho(double spot, double r, double sigma) const override;
};

class OptionCallAsiat : public Option
{
public:
    OptionCallAsiat(double T, double strike) : Option(T, make_unique<PayoffCallAsiat>(strike)) {};
};

class OptionPutAsiat : public Option
{
public:
    OptionPutAsiat(double T, double strike) : Option(T, make_unique<PayoffPutAsiat>(strike)) {};
};