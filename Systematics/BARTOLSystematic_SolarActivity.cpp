#include "Systematics/BARTOLSystematic_SolarActivity.h"

template<typename T>
BARTOLSystematic_SolarActivity<T>::BARTOLSystematic_SolarActivity() : BARTOLSystematicBase<T>("SolarActivity") {
}

template<typename T>
BARTOLSystematic_SolarActivity<T>::~BARTOLSystematic_SolarActivity() {
}

template<typename T>
T BARTOLSystematic_SolarActivity<T>::CalculateWeight(T DialValue, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) {
  T Weight = DialValue;
  return Weight;
}
