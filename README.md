# PricingEngine — Moteur de Pricing d'Options en C++ & Python

**PricingEngine** est une bibliothèque de finance quantitative écrite en C++ moderne (C++17/C++20) avec une interface Python propulsée par **pybind11**. Elle permet de valoriser des options financières (européennes, asiatiques, digitales) et de calculer leurs sensibilités (*Greeks*) à l'aide de formules fermées (modèle de Black-Scholes) et de simulations de Monte-Carlo dotées de techniques avancées de réduction de variance.

---

## Fonctionnalités

### 1. Instruments et Payoffs supportés

- **Options Européennes** (`OptionCallEurope`, `OptionPutEurope`) : Payoff évalué à maturité $(S_T - K)_+$ ou $(K - S_T)_+$.
- **Options Asiatiques à moyenne arithmétique** (`OptionCallAsiat`, `OptionPutAsiat`) : Payoff dépendant de la trajectoire complète via la moyenne arithmétique des cours du sous-jacent.
- **Payoff Digital** (`PayoffDigital`) : Fonction indicatrice $\mathbb{I}_{\{S_T < K\}}$.

### 2. Modèle analytique de Black-Scholes

Pour les options européennes vanilles (`OptionCallEurope`, `OptionPutEurope`) :

- **Prix exact** (`bs_price`) par formule fermée de Black-Scholes.
- **Calcul analytique des Grecques** :
  - $\Delta$ (`delta`) : Sensibilité au prix spot du sous-jacent.
  - $\Gamma$ (`gamma`) : Sensibilité seconde au prix spot.
  - $\mathcal{V}$ (`vega`) : Sensibilité à la volatilité $\sigma$.
  - $\Theta$ (`theta`) : Sensibilité à l'écoulement du temps.
  - $\rho$ (`rho`) : Sensibilité au taux d'intérêt sans risque $r$.

### 3. Moteur de simulation Monte-Carlo (`MonteCarloPricer`)

Toutes les estimations Monte-Carlo retournent une structure `MonteCarloResult` contenant la valeur estimée ainsi que les bornes inférieure et supérieure de l'**intervalle de confiance à 95 %**.

| Méthode | Description |
| :--- | :--- |
| `price_v1` | **Monte-Carlo naïf** : Simulation directe du cours terminal $S_T$. |
| `price_v2` | **Variables antithétiques** : Réduction de variance en couplant chaque tirage gaussien $Z$ avec $-Z$. |
| `price_v3` | **Variable de contrôle (1 passe)** : Utilise le sous-jacent actualisé $X = e^{-rT} S_T$ ($\mathbb{E}[X] = S_0$) comme variable de contrôle en estimant le coefficient optimal $c^*$ sur l'échantillon unique. |
| `price_v4` | **Variable de contrôle (approche pilote)** : Estime le coefficient optimal $c^*$ sur un échantillon pilote de taille $n$, puis évalue le prix sur $N$ tirages indépendants. |
| `price_v5` | **Options dépendantes de la trajectoire (Asiatiques)** : Génération de trajectoires complètes de $m$ pas de temps via `generate_path`. |

### 4. Estimation des Grecques par Monte-Carlo

- **Différences finies / Bumping** (`delta_bumping`) : Approximation centrée avec semence aléatoire commune (*Common Random Numbers*) entre $S_0 + \varepsilon$ et $S_0 - \varepsilon$.
- **Méthode Pathwise** (`delta_pathwise`) : Dérivation directe le long de la trajectoire pour les options européennes :
  $$\Delta_{\text{pathwise}} = e^{-rT} \Phi'(S_T) \frac{S_T}{S_0}$$

---

## Structure du projet

```text
PricingEngine/
├── CMakeLists.txt                  # Configuration de compilation CMake (exécutable C++)
├── setup.py                        # Script de compilation du module Python (pybind11)
├── test.py                         # Exemple d'utilisation depuis Python
├── TODO.md                         # Pistes d'améliorations et refactorisations
├── pricer/
│   └── include/
│       ├── MathUtils.hpp           # Fonctions de répartition et de densité de la loi normale
│       ├── Payoff.hpp              # Hiérarchie des classes de Payoff (héritage virtuel)
│       ├── Option.hpp              # Classes Option, OptionEurope, OptionCall/Put (Europe & Asiat)
│       └── MonteCarloPricer.hpp    # Moteur Monte-Carlo et structure de résultat
└── src/
    ├── MathUtils.cpp               # Implémentation de norm_cdf et norm_pdf
    ├── Option.cpp                  # Formules de Black-Scholes et Grecques analytiques
    ├── MonteCarloPricer.cpp        # Algorithmes de simulation Monte-Carlo et réduction de variance
    ├── bindings.cpp                # Bindings Python via pybind11 (module pricer_module)
    └── main.cpp                    # Point d'entrée de l'exécutable C++
```

---

## Architecture logicielle

Le moteur repose sur une séparation claire entre la définition du contrat (`Option`), sa fonction de gain (`Payoff`), et la méthode numérique de résolution (`MonteCarloPricer`) :

- **`Payoff`** : Utilise l'héritage virtuel (`PayoffCall`, `PayoffPut`, `PayoffEurope`) pour combiner la nature de l'option (Call/Put) et ses propriétés mathématiques (ex. dérivabilité du payoff via `derivative(spot)` ou évaluation sur trajectoire `operator()(const vector<double>& path)`).
- **`Option`** : Encapsule la maturité $T$ et possède un pointeur intelligent (`std::unique_ptr<Payoff>`) vers son payoff.
- **`MonteCarloPricer`** : Paramétré par le nombre de simulations $N$, il agit sur n'importe quelle instance de `Option`.

---

## Prérequis

- **Compilateur C++** compatible C++17 / C++20 (ex. MinGW-w64 `g++`, MSVC, ou Clang).
- **CMake** ($\ge 3.12$) pour compiler l'exécutable C++.
- **Python** ($\ge 3.10$) avec les paquets `pybind11` et `setuptools` pour compiler l'extension Python.

---

## Installation et Compilation

### 1. Compiler le module Python (`pricer_module`)

Activez votre environnement virtuel Python, installez `pybind11` et `setuptools` si nécessaire, puis compilez l'extension C++ sur place (`--inplace`) :

```bash
# Installation et activation de l'environnement virtuel (Windows PowerShell)
python -m venv env
.\env\Scripts\Activate.ps1

# Installation des dépendances
pip install -r requirements.txt

# Compilation avec MinGW64 sous Windows
python setup.py build_ext --inplace --compiler=mingw32
```

Cela génère un fichier binaire `.pyd` (sous Windows) ou `.so` (sous Linux/macOS) directement importable dans Python sous le nom `pricer_module`.

### 2. Compiler l'exécutable C++ avec CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Vous pouvez également compiler directement en ligne de commande avec `g++` :

```bash
g++ -std=c++17 -O3 -I pricer/include src/main.cpp src/Option.cpp src/MathUtils.cpp src/MonteCarloPricer.cpp -o main.exe
```

---

## Exemples d'utilisation

### En Python

> **Note sous Windows (MinGW)** : Assurez-vous d'ajouter le dossier contenant les DLL de MinGW (`C:\mingw64\bin`) via `os.add_dll_directory` avant d'importer `pricer_module`.

```python
import os
os.add_dll_directory(r"C:\mingw64\bin")

from pricer_module import OptionCallAsiat, MonteCarloPricer

# Paramètres du marché et de l'option
call_asiat = OptionCallAsiat(T=1.0, strike=100.0)
pricer = MonteCarloPricer(N=100_000)

# Pricing d'une option asiatique sur m = 1000 pas de discrétisation
result = pricer.price_v5(
    option=call_asiat,
    spot=100.0,
    r=0.05,
    sigma=0.2,
    m=1000
)

print(result)
# Accès direct aux attributs :
print(f"Prix : {result.estimated_value:.4f}")
print(f"IC 95% : [{result.lower_bound:.4f}, {result.upper_bound:.4f}]")
```

### En C++

```cpp
#include <iostream>
#include "Option.hpp"
#include "MonteCarloPricer.hpp"

int main()
{
    double T = 1.0;
    double strike = 100.0;
    double spot = 100.0;
    double r = 0.05;
    double sigma = 0.2;

    // 1. Option Européenne : Black-Scholes vs Monte-Carlo
    OptionCallEurope call_eu(T, strike);
    std::cout << "Prix Black-Scholes : " << call_eu.bs_price(spot, r, sigma) << "\n";
    std::cout << "Delta analytique   : " << call_eu.delta(spot, r, sigma) << "\n\n";

    MonteCarloPricer pricer(100000);
    std::cout << "--- Monte-Carlo (Variable de contrôle 1 passe) ---\n";
    std::cout << pricer.price_v3(call_eu, spot, r, sigma) << "\n";

    std::cout << "--- Delta Pathwise ---\n";
    std::cout << pricer.delta_pathwise(call_eu, spot, r, sigma) << "\n";

    // 2. Option Asiatique
    OptionCallAsiat call_asiat(T, strike);
    std::cout << "--- Option Call Asiatique (m = 1000 pas) ---\n";
    std::cout << pricer.price_v5(call_asiat, spot, r, sigma, 1000);

    return 0;
}
```

---

## Rappel du modèle mathématique

Sous la probabilité risque-neutre $\mathbb{Q}$, la dynamique du sous-jacent suit un mouvement brownien géométrique :

$$\mathrm{d}S_t = r S_t \, \mathrm{d}t + \sigma S_t \, \mathrm{d}W_t$$

La solution exacte utilisée pour simuler le cours terminal (`price_v1` à `price_v4`) ou les pas successifs sur une grille $t_k = k \frac{T}{m}$ (`generate_path` / `price_v5`) est donnée par :

$$S_{t+\Delta t} = S_t \exp\left( \left(r - \frac{1}{2}\sigma^2\right)\Delta t + \sigma \sqrt{\Delta t} \, Z \right), \quad Z \sim \mathcal{N}(0, 1)$$
