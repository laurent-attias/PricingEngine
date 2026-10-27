#pragma once
#include <algorithm>

using std::max;

class Payoff
{
    private:
        double strike;
    public:
        Payoff(double K) : strike(K) {}
        virtual double operator()(double spot) const = 0;
        virtual ~Payoff() = default;
        double getStrike() const
        {
            return strike;
        }
        virtual double derivative(double spot) const = 0;
};

class PayoffCall : public Payoff
{
    public:
        PayoffCall(double K) : Payoff(K) {}
        double operator()(double spot) const override
        {
            return max(spot - getStrike(), 0.0);
        }
        double derivative(double spot) const override
        {
            return (spot > getStrike()) ? 1.0: 0.0;
        }
};

class PayoffPut : public Payoff
{
    public:
        PayoffPut(double K) : Payoff(K) {}
        double operator()(double spot) const override
        {
            return max(getStrike() - spot, 0.0);
        }
        double derivative(double spot) const override 
        {
            return (spot > getStrike()) ? 0.0: -1.0;
        }
};

class PayoffDigital : public Payoff
{
    public:
        PayoffDigital(double K);
        double operator()(double spot) const override
        {
            return (spot < getStrike()) ? 1.0 : 0.0;
        }
};
