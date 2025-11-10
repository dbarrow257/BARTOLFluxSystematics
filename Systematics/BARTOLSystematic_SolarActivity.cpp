#include "Systematics/BARTOLSystematic_SolarActivity.h"
//cosmic primary energy in GeV
#include <cerrno>
#include <cmath>
#include <stdexcept>

namespace{
const std::string systematicName {"SolarActivity"};
constexpr size_t numOfDials {1};
constexpr double protonMass {0.938272};
constexpr double rigidityCorrection1{4.96};
constexpr double rigidityCorrection2{1.34};
constexpr double solarModulationCutOff{100.}; //above 100GeV assume no solar modulation correction.

enum particleID{
    H,
    He
};
}

template <typename T>
T computeWeight(T rigidity, T solarActivity){
    T minWeight {static_cast<T>(1/(1+rigidityCorrection1/std::pow(rigidity, rigidityCorrection2)))};
    return static_cast<T>(1 -  solarActivity*(1 - minWeight));
}
template <typename T>
T eventweightSolar(T primaryEnergy,  particleID ID, T solarActivity){
    if (primaryEnergy > solarModulationCutOff)
        return 1.;
    T protonRigidity = std::sqrt(primaryEnergy*primaryEnergy - protonMass*protonMass);

    if (ID == H){
        return computeWeight(protonRigidity, solarActivity);
    }
    if (ID == He){
        return computeWeight(protonRigidity*2, solarActivity);
    }
    throw std::runtime_error("H or He expected");
}



template<typename T>
BARTOLSystematic_SolarActivity<T>::BARTOLSystematic_SolarActivity() : BARTOLSystematicBase<T>(systematicName, numOfDials) {
}

template<typename T>
BARTOLSystematic_SolarActivity<T>::~BARTOLSystematic_SolarActivity() {
}

template<typename T>
T BARTOLSystematic_SolarActivity<T>::CalculateWeight(const std::vector<T>& DialValues, size_t position, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) {
  T DialValue = DialValues[position];
  T Weight = eventweightSolar(NeutrinoEnergy_, H, DialValue); //supposed to be based on parent energy but leave as neutrino energy for now.
  return Weight;
}
