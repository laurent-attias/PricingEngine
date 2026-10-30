import os

os.add_dll_directory(r"C:\mingw64\bin")
from pricer_module import OptionCallAsiat, MonteCarloPricer

MATURITY = 1
STRIKE = 100
SPOT = 100
RISK_FREE_INTEREST_RATE = 0.05
SIGMA = 0.2
N = 100000
M = 1000

call = OptionCallAsiat(
    T=MATURITY,
    strike=STRIKE,
)

pricer = MonteCarloPricer(N=N)

result = pricer.price_v5(
    option=call,
    spot=SPOT,
    r=RISK_FREE_INTEREST_RATE,
    sigma=SIGMA,
    m=M
)

print(result)
