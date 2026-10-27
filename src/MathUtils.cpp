#include <cmath>
#include "MathUtils.hpp"

double norm_cdf(double x)
{
    return 0.5 * (1 + std::erf(x / std::sqrt(2.0)));
}

double norm_pdf(double x)
{
    return 1.0 / std::sqrt(2.0 * PI) * std::exp(-0.5 * x * x);
}