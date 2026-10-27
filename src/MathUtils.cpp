#include <cmath>
#include "MathUtils.hpp"

using std::erf;
using std::exp;
using std::sqrt;

double norm_cdf(double x)
{
    return 0.5 * (1 + erf(x / sqrt(2.0)));
}

double norm_pdf(double x)
{
    return 1.0 / sqrt(2.0 * PI) * exp(-0.5 * x * x);
}
