#include "Systematics/Systematic_SolarActivity.h"

template<typename T>
Systematic_SolarActivity<T>::Systematic_SolarActivity() : SystematicBase<T>("SolarActivity") {
}

template<typename T>
Systematic_SolarActivity<T>::~Systematic_SolarActivity() {
}

template<typename T>
T Systematic_SolarActivity<T>::CalculateWeight(T DialValue, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) {
  T Weight = DialValue*NeutrinoEnergy_/1000.;
  return Weight;
}
