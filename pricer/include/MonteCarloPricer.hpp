#include "Option.hpp"

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
    MonteCarloResult price_v4(const Option& option, double spot, double r, double sigma) const;
};