#include <cmath>
#include <random>
#include <chrono>

#include "MonteCarloPricer.hpp"

MonteCarloResult MonteCarloPricer::price_v1(const Option &option, double spot, double r, double sigma) const
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::normal_distribution<double> dist(0.0, 1.0);

    double drift = (r - 0.5 * sigma * sigma) * option.getMaturite();
    double vol_term = sigma * std::sqrt(option.getMaturite());
    double actualization = std::exp(-r * option.getMaturite());
    double sum = 0.0;
    double sum_sq = 0.0;
    for (int i = 0; i < N; i++)
    {
        double Z = dist(gen);
        double spot_i = spot * std::exp(drift + vol_term * Z);
        double payoff_i = option.calculerPayoff(spot_i);
        double Y_i = actualization * payoff_i;
        sum += Y_i;
        sum_sq += Y_i * Y_i;
    }
    double mean = sum / N;
    double variance = 1.0 / (N - 1) * (sum_sq - N * mean * mean);
    double error_margin = 1.96 * std::sqrt(variance / N);
    return MonteCarloResult{mean, mean - error_margin, mean + error_margin};
}

MonteCarloResult MonteCarloPricer::price_v2(const Option &option, double spot, double r, double sigma) const
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::normal_distribution<double> dist(0.0, 1.0);

    double drift = (r - 0.5 * sigma * sigma) * option.getMaturite();
    double vol_term = sigma * std::sqrt(option.getMaturite());
    double actualization = std::exp(-r * option.getMaturite());
    double sum = 0.0;
    double sum_sq = 0.0;
    int M = N / 2;
    for (int i = 0; i < M; i++)
    {
        double Z = dist(gen);
        double spot_i1 = spot * std::exp(drift + vol_term * Z);
        double spot_i2 = spot * std::exp(drift - vol_term * Z);
        double payoff_i1 = option.calculerPayoff(spot_i1);
        double payoff_i2 = option.calculerPayoff(spot_i2);
        double payoff_i = (payoff_i1 + payoff_i2) * 0.5;
        double Y_i = actualization * payoff_i;
        sum += Y_i;
        sum_sq += Y_i * Y_i;
    }
    double mean = sum / M;
    double variance = 1.0 / (M - 1) * (sum_sq - M * mean * mean);
    double error_margin = 1.96 * std::sqrt(variance / M);
    return MonteCarloResult{mean, mean - error_margin, mean + error_margin};
}

MonteCarloResult MonteCarloPricer::price_v3(const Option &option, double spot, double r, double sigma) const
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::normal_distribution<double> dist(0.0, 1.0);

    double drift = (r - 0.5 * sigma * sigma) * option.getMaturite();
    double vol_term = sigma * std::sqrt(option.getMaturite());
    double actualization = std::exp(-r * option.getMaturite());
    double sum_Y = 0.0;
    double sum_X = 0.0;
    double sum_sq_Y = 0.0;
    double sum_sq_X = 0.0;
    double sum_YX = 0.0;
    for (int i = 0; i < N; i++)
    {
        double Z = dist(gen);
        double spot_i = spot * std::exp(drift + vol_term * Z);
        double X_i = actualization * spot_i;
        double payoff_i = option.calculerPayoff(spot_i);
        double Y_i = actualization * payoff_i;
        sum_Y += Y_i;
        sum_X += X_i;
        sum_sq_Y += Y_i * Y_i;
        sum_sq_X += X_i * X_i;
        sum_YX += Y_i * X_i;
    }
    double mean_Y = sum_Y / N;
    double mean_X = sum_X / N;
    double var_Y = 1.0 / (N - 1) * (sum_sq_Y - N * mean_Y * mean_Y);
    double var_X = 1.0 / (N - 1) * (sum_sq_X - N * mean_X * mean_X);
    double covar_YX = 1.0 / (N - 1) * (sum_YX - N * mean_X * mean_Y);
    double c = covar_YX / var_X;
    double mean_Y_cv = mean_Y - c * (mean_X - spot);
    double var_Y_cv = var_Y - covar_YX * covar_YX / var_X;
    double error_margin = 1.96 * std::sqrt(var_Y_cv / N);
    return MonteCarloResult{mean_Y_cv, mean_Y_cv - error_margin, mean_Y_cv + error_margin};
}

MonteCarloResult MonteCarloPricer::price_v4(const Option &option, double spot, double r, double sigma, int n) const
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::normal_distribution<double> dist(0.0, 1.0);

    double drift = (r - 0.5 * sigma * sigma) * option.getMaturite();
    double vol_term = sigma * std::sqrt(option.getMaturite());
    double actualization = std::exp(-r * option.getMaturite());

    // phase pilote

    double sum_Y = 0.0;
    double sum_X = 0.0;
    double sum_sq_Y = 0.0;
    double sum_sq_X = 0.0;
    double sum_YX = 0.0;
    for (int i = 0; i < n; i++)
    {
        double Z = dist(gen);
        double spot_i = spot * std::exp(drift + vol_term * Z);
        double X_i = actualization * spot_i;
        double payoff_i = option.calculerPayoff(spot_i);
        double Y_i = actualization * payoff_i;
        sum_Y += Y_i;
        sum_X += X_i;
        sum_sq_Y += Y_i * Y_i;
        sum_sq_X += X_i * X_i;
        sum_YX += Y_i * X_i;
    }
    double mean_Y = sum_Y / n;
    double mean_X = sum_X / n;
    double var_Y = 1.0 / (n - 1) * (sum_sq_Y - n * mean_Y * mean_Y);
    double var_X = 1.0 / (n - 1) * (sum_sq_X - n * mean_X * mean_X);
    double covar_YX = 1.0 / (n - 1) * (sum_YX - n * mean_X * mean_Y);
    double c = covar_YX / var_X;

    // phase principale

    double sum_Y_cv = 0.0;
    double sum_sq_Y_cv = 0.0;
    for (int i = 0; i < N; i++)
    {
        double Z = dist(gen);
        double spot_i = spot * std::exp(drift + vol_term * Z);
        double X_i = actualization * spot_i;
        double payoff_i = option.calculerPayoff(spot_i);
        double Y_i = actualization * payoff_i;
        double Y_cv_i = Y_i - c * (X_i - spot);
        sum_Y_cv += Y_cv_i;
        sum_sq_Y_cv += Y_cv_i * Y_cv_i;
    }
    double mean_Y_cv = sum_Y_cv / N;
    double var_Y_cv = 1.0 / (N - 1) * (sum_sq_Y_cv - N * mean_Y_cv * mean_Y_cv);
    double error_margin = 1.96 * std::sqrt(var_Y_cv / N);
    return MonteCarloResult{mean_Y_cv, mean_Y_cv - error_margin, mean_Y_cv + error_margin};
}


MonteCarloResult MonteCarloPricer::delta_bumping(const Option &option, double spot, double r, double sigma, double epsilon) const
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::normal_distribution<double> dist(0.0, 1.0);

    double drift = (r - 0.5 * sigma * sigma) * option.getMaturite();
    double vol_term = sigma * std::sqrt(option.getMaturite());
    double actualization = std::exp(-r * option.getMaturite());
    double sum = 0.0;
    double sum_sq = 0.0;
    for (int i = 0; i < N; i++)
    {
        double Z_i = dist(gen);
        double spot_i_plus = (spot + epsilon) * std::exp(drift + vol_term * Z_i);
        double spot_i_minus = (spot - epsilon) * std::exp(drift + vol_term * Z_i);
        double payoff_i_plus = option.calculerPayoff(spot_i_plus);
        double payoff_i_minus = option.calculerPayoff(spot_i_minus);
        double Delta_i = actualization * (payoff_i_plus - payoff_i_minus) / (2 * epsilon);
        sum += Delta_i;
        sum_sq += Delta_i * Delta_i;
    }

    double mean = sum / N;
    double variance = 1.0 / (N - 1) * (sum_sq - N * mean * mean);
    double error_margin = 1.96 * std::sqrt(variance / N);
    return MonteCarloResult{mean, mean - error_margin, mean + error_margin};
}

// calcul du Delta par pathwise

MonteCarloResult MonteCarloPricer::delta_pathwise(const Option& option, double spot, double r, double sigma) const
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned int>(seed));
    std::normal_distribution<double> dist(0.0, 1.0);

    double drift = (r - 0.5 * sigma * sigma) * option.getMaturite();
    double vol_term = sigma * std::sqrt(option.getMaturite());
    double actualization = std::exp(-r * option.getMaturite());
    double sum = 0.0;
    double sum_sq = 0.0;
    for (int i=0; i<N; i++)
    {
        double Z_i = dist(gen);
        double spot_i = spot * std::exp(drift + vol_term * Z_i);
        double delta_i = actualization * option.derivePayoff(spot_i) * spot_i / spot ;
        sum += delta_i ;
        sum_sq += delta_i * delta_i ;
    }
    double mean = sum / N;
    double variance = 1.0 / (N - 1) * (sum_sq - N * mean * mean);
    double error_margin = 1.96 * std::sqrt(variance / N);
    return MonteCarloResult{mean, mean - error_margin, mean + error_margin};
}

std::vector<double> MonteCarloPricer::generate_path(double spot, double T, int m, double r, double sigma, std::mt19937& gen) const
{
    std::normal_distribution<double> dist(0.0, 1.0);
    double dt = T / m ;
    double drift = (r - 0.5 * sigma * sigma) *  dt ;
    double vol_term = sigma * std::sqrt(dt);
    std::vector<double> S;
    S.reserve(m+1);
    S.push_back(spot);
    for (int k=1; k<m+1; k++)
    {
        double Z_k = dist(gen);
        S.push_back(
            S[k-1]*exp(drift + vol_term * Z_k)
        );
    }
    return S;
}

