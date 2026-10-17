#include <memory>
#include <algorithm>
#include <iostream>
#include <cmath>

class Payoff
{
public:
    double strike;
    virtual double calculer(double spot) const = 0;
    virtual ~Payoff() = default;
};

class PayoffCall : public Payoff
{
private:
    double strike;

public:
    PayoffCall(double K) : strike(K) {}
    double calculer(double spot) const override
    {
        return std::max(spot - strike, 0.0);
    }
};

class PayoffPut : public Payoff
{
private:
    double strike;

public:
    PayoffPut(double K) : strike(K) {}
    double calculer(double spot) const override
    {
        return std::max(strike - spot, 0.0);
    }
};

class PayoffDigital : public Payoff
{
private:
    double strike;

public:
    PayoffDigital(double K) : strike(K) {}
    double calculer(double spot) const override
    {
        return (spot < strike) ? 1.0 : 0.0;
    }
};

class Option
{
private:
    double maturite;
    std::unique_ptr<Payoff> payoff;

public:
    Option(double T, std::unique_ptr<Payoff> p) : maturite(T), payoff(std::move(p)) {}
    double calculerPayoff(double spot) const
    {
        return payoff->calculer(spot);
    }
    double bs_call_price(double spot, double r, double sigma) {
        double d_1 = (std::log(spot/payoff->strike) + (r + 0.5*sigma*sigma)*maturite)/(sigma*std::sqrt(maturite));
        double d_2 = d_1 - sigma*std::sqrt(maturite);
        return spot*norm_cdf(d_1) - payoff->strike*std::exp(-r*maturite)*norm_cdf(d_2);
    }
};


double bs_call_price(double spot, double strike, double maturite, double r, double sigma)
{
    
}

double norm_cdf(double x)
{
    return 0.5 * (1 + std::erf(x / std::sqrt(2.0)));
}



int main()
{
    auto payoff = std::make_unique<PayoffCall>(100.0);
    const Option monOption(1.0, std::move(payoff));
    std::cout << monOption.calculerPayoff(110.0) << "\n"; //calcul du payoff avec cours spot terminal S_T = 110.0 ;
    std::cout << bs_call_price(100, 100, 1.0, 0.05, 0.2);
    return 0;
}