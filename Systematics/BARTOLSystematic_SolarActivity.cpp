#include "Systematics/BARTOLSystematic_SolarActivity.h"
//cosmic primary energy in GeV
#include <cerrno>
#include <cmath>
#include <array>
#include <stdexcept>

constexpr double protonMass {0.938272};
constexpr double rigidityCorrection1{4.96};
constexpr double rigidityCorrection2{1.34};
constexpr double solarModulationCutOff{100.}; //above 100GeV assume no solar modulation correction.

//units are in (GeV/n)−1m−2s−1sr−1
constexpr std::array<double, 4> normNominalH {14275, 2.44, -0.36, 2.75};

//since helium has 4 nucleons, multiply by 4.
constexpr std::array<double, 4> normNominalHe{4*531, 4*1.24, 4*(-0.36), 4*2.63};

enum particleID{
    H,
    He
};
template <typename T>
T computeWeight(T rigidity, T solarActivity){
    T minWeight {static_cast<T>(1/(1+static_cast<T>(rigidityCorrection1/(std::pow(rigidity, rigidityCorrection2)))))};
    return static_cast<T>(1 -  solarActivity*(1 - minWeight));
}
template <typename T>
T eventweightSolar(T primaryEnergy,  particleID ID, T solarActivity){
    if (primaryEnergy > solarModulationCutOff)
        return 1.;
    auto protonRigidity {std::sqrt(primaryEnergy*primaryEnergy - protonMass*protonMass)};

    if (ID == H){
        return computeWeight(protonRigidity, solarActivity);
    }
    if (ID == He){
        return computeWeight(protonRigidity*2, solarActivity);
    }
    throw std::runtime_error("H or He expected");
}

//values for these come from Justin Evans et al. Uncertainties in atmospheric muon-neutrino fluxes arising from comsic-ray primaries;
//Formula gives a global fit to data for the primary cosmic ray flux at solar minimum.
//Might have to look into using alternative parameterisation for hydrogen given in Evans paper which is more accurate but I think Bartol uses the standard GSHL parameterisation.
double fluxAtSolarMin(double energy, std::array<double, 4> normParams){
    return normParams[0]*std::pow((energy+normParams[1]*std::exp(normParams[2]*std::sqrt(energy))), -1*normParams[3]);
}


double eventWeightNormalisation(double energy, std::array<double, 4> normParams, particleID ID){
    if (ID == H){
        return fluxAtSolarMin(energy, normParams)/fluxAtSolarMin(energy, normNominalH);
    }
    if (ID == He){
        return fluxAtSolarMin(energy, normParams)/fluxAtSolarMin(energy, normNominalHe);       
    }
    throw std::runtime_error("H or He expected");
}

template<typename T>
BARTOLSystematic_SolarActivity<T>::BARTOLSystematic_SolarActivity() : BARTOLSystematicBase<T>("SolarActivity") {
}

template<typename T>
BARTOLSystematic_SolarActivity<T>::~BARTOLSystematic_SolarActivity() {
}

template<typename T>
T BARTOLSystematic_SolarActivity<T>::CalculateWeight(T DialValue, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) {
  
    T Weight = eventweightSolar(NeutrinoEnergy_, H, DialValue); //supposed to be based on parent energy but leave as neutrino energy for now.
  return Weight;
}
