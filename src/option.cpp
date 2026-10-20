#include <memory>
#include <algorithm>
#include <iostream>
#include <cmath>

class Payoff
{
private:
    double strike;
public:
    Payoff(double K) :strike(K) {}
    virtual double calculer(double spot) const = 0;
    virtual ~Payoff() = default;
    double getStrike() const {
        return strike;
    }
};

class PayoffCall : public Payoff
{
public:
    PayoffCall(double K) : Payoff(K) {}
    double calculer(double spot) const override
    {
        return std::max(spot - getStrike(), 0.0);
    }
};

class PayoffPut : public Payoff
{

public:
    PayoffPut(double K) : Payoff(K) {}
    double calculer(double spot) const override
    {
        return std::max(getStrike() - spot, 0.0);
    }
};

class PayoffDigital : public Payoff
{

public:
    PayoffDigital(double K) : Payoff(K) {}
    double calculer(double spot) const override
    {
        return (spot < getStrike()) ? 1.0 : 0.0;
    }
};

class Option
{
protected:
    double maturite;
    std::unique_ptr<Payoff> payoff;

public:
    Option(double T, std::unique_ptr<Payoff> p) : maturite(T), payoff(std::move(p)) {}
    double calculerPayoff(double spot) const
    {
        return payoff->calculer(spot);
    }
    virtual double bs_price(double spot, double r, double sigma) const = 0;
    virtual double delta(double spot, double r, double sigma) const = 0;
    virtual ~Option() = default;
};


double norm_cdf(double x)
{
    return 0.5 * (1 + std::erf(x / std::sqrt(2.0)));
}

class OptionCall : public Option
{
public:
    OptionCall(double T, double strike) : Option(T, std::make_unique<PayoffCall>(strike)) {}
    double bs_price(double spot, double r, double sigma) const override
    {
        double d_1 = (std::log(spot / payoff->getStrike()) + (r + 0.5 * sigma * sigma) * maturite) / (sigma * std::sqrt(maturite));
        double d_2 = d_1 - sigma * std::sqrt(maturite);
        return spot * norm_cdf(d_1) - payoff->getStrike() * std::exp(-r * maturite) * norm_cdf(d_2);
    }
    double delta(double spot, double r, double sigma) const override
    {
        double d_1 = (std::log(spot / payoff->getStrike()) + (r + 0.5 * sigma * sigma) * maturite) / (sigma * std::sqrt(maturite));
        return norm_cdf(d_1);
    }
};

class OptionPut : public Option
{

public:
    OptionPut(double T, double strike) : Option(T, std::make_unique<PayoffPut>(strike)) {}
    double bs_price(double spot, double r, double sigma) const override
    {
        double d_1 = (std::log(spot / payoff->getStrike()) + (r + 0.5 * sigma * sigma) * maturite) / (sigma * std::sqrt(maturite));
        double d_2 = d_1 - sigma * std::sqrt(maturite);
        return payoff->getStrike() * std::exp(-r * maturite) * norm_cdf(-d_2) - spot * norm_cdf(-d_1);
    }
    double delta(double spot, double r, double sigma) const override {
        double d_1 = (std::log(spot / payoff->getStrike()) + (r + 0.5 * sigma * sigma) * maturite) / (sigma * std::sqrt(maturite));
        return -norm_cdf(-d_1);
    }
};



int main()
{
    double T = 1.0;
    double strike = 100.0;
    double spot = 100.0;
    double r = 0.05;
    double sigma = 0.2;
    OptionPut put(T, strike);
    OptionCall call(T, strike);
    std::cout << "Prix de l'option put : " << put.bs_price(spot, r, sigma) << "\n";
    std::cout << "prix de l'option call : " << call.bs_price(spot, r, sigma);
}