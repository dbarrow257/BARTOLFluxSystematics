#include "Systematics/SystematicsHandler.h"

#include "Systematics/Systematic_SolarActivity.h"
#include <iostream>

template <typename T>
SystematicsHandler<T>::SystematicsHandler() {
  std::cout << "Initialising Atmospheric Flux Systematics..." << std::endl;
  Systematics.emplace_back(new Systematic_SolarActivity<T>());

  nParams = Systematics.size();
  std::cout << "Initialised " << nParams << " parameters" << "\n\n" << std::endl;
}

template <typename T>
SystematicsHandler<T>::~SystematicsHandler() {
}

template <typename T>
void SystematicsHandler<T>::SetDialValues(std::vector<T> DialValues_) {
  if (DialValues_.size() != nParams) {
    std::cerr << "Invalid number of dial values passed to FluxSystematicsEvaluator::SystematicsHandler" << std::endl;
    std::cerr << "DialValues_.size():" << DialValues_.size() << std::endl;
    std::cerr << "Systematics.size():" << Systematics.size() << std::endl;
    throw;
  }

  DialValues = DialValues_;
}

template <typename T>
T SystematicsHandler<T>::CalculateWeight(int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) {
  T Weight = 1.0;
  for (size_t iPar=0;iPar<nParams;iPar++) {
    Weight *= Systematics[iPar]->CalculateWeight(DialValues[iPar], GeneratedNeutrinoFlavourPDG_,NeutrinoEnergy_,NeutrinoCosineZ_);
  }
  return Weight;
}

template class SystematicsHandler<float>;
template class SystematicsHandler<double>;
