#pragma once
#include <random>
#include <vector>
#include "Option.hpp"

using std::mt19937;
using std::vector;

struct MonteCarloResult
{
    double estimated_value;
    double lower_bound;
    double upper_bound;
};

ostream& operator<<(ostream& os, const MonteCarloResult& res);


class MonteCarloPricer 
{
protected:
    int N;
public:
    MonteCarloPricer(int N) : N(N) {}

    // méthode naïve
    MonteCarloResult price_v1(const Option& option, double spot, double r, double sigma) const ;

    // réduction de variance par variables antithétiques
    MonteCarloResult price_v2(const Option& option, double spot, double r, double sigma) const;

    // réduction de variance par variable de contrôle

    // approche à une passe
    MonteCarloResult price_v3(const Option& option, double spot, double r, double sigma) const;

    // approche pilote
    MonteCarloResult price_v4(const Option& option, double spot, double r, double sigma, int n) const;

    // calcul des grecques

    // calcul du Delta par bumping
    MonteCarloResult delta_bumping(const Option& option, double spot, double r, double sigma, double epsilon) const;

    // calcul du Delta par pathwise
    MonteCarloResult delta_pathwise(const OptionEurope& option, double spot, double r, double sigma) const;

    // discrétisation d'Euler-Maruyama
    vector<double> generate_path(double spot, double T, int m, double r, double sigma, mt19937& gen) const;

    // calcul pour une option asiatique
    MonteCarloResult price_v5(const Option& option, double spot, double r, double sigma, int m) const;

};
