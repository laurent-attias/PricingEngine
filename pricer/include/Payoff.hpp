#pragma once
#include <algorithm>
#include <numeric>
#include <vector>
#include <stdexcept>

using std::accumulate;
using std::logic_error;
using std::max;
using std::vector;

class Payoff
{
private:
    double strike;

public:
    Payoff(double K) : strike(K) {}
    virtual double operator()(double spot) const = 0;
    virtual double operator()(const vector<double> &path) const
    {
        return operator()(path.back());
    }
    virtual ~Payoff() = default;
    double getStrike() const
    {
        return strike;
    }
};

class PayoffCall : virtual public Payoff
{
public:
    PayoffCall(double K) : Payoff(K) {}
    double operator()(double spot) const override
    {
        return max(spot - getStrike(), 0.0);
    }
};

class PayoffEurope : virtual public Payoff
{
public:
    PayoffEurope(double K) : Payoff(K) {}
    virtual double derivative(double spot) const = 0;
};

class PayoffCallEurope : public PayoffCall, public PayoffEurope
{
public:
    PayoffCallEurope(double K) : Payoff(K), PayoffCall(K), PayoffEurope(K) {}
    double derivative(double spot) const override
    {
        return (spot > getStrike()) ? 1.0 : 0.0;
    }
};

class PayoffCallAsiat : public PayoffCall
{
public:
    PayoffCallAsiat(double K) : Payoff(K), PayoffCall(K) {}
    double operator()(double spot) const override
    {
        throw logic_error("Ce payoff nécessite une trajectoire complète et non un spot terminal.");
    }
    double operator()(const vector<double> &path) const override
    {
        double mean_spot = accumulate(path.begin(), path.end(), 0.0) / path.size();
        return PayoffCall::operator()(mean_spot);
    }
};

class PayoffPut : virtual public Payoff
{
public:
    PayoffPut(double K) : Payoff(K) {}
    double operator()(double spot) const override
    {
        return max(getStrike() - spot, 0.0);
    }
};

class PayoffPutEurope : public PayoffPut, public PayoffEurope
{
public:
    PayoffPutEurope(double K) : Payoff(K), PayoffPut(K), PayoffEurope(K) {}
    double derivative(double spot) const override
    {
        return (spot < getStrike()) ? -1.0 : 0.0;
    }
};

class PayoffDigital : public Payoff
{
public:
    PayoffDigital(double K) : Payoff(K) {}
    double operator()(double spot) const override
    {
        return (spot < getStrike()) ? 1.0 : 0.0;
    }
};