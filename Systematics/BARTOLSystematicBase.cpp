#include "Systematics/BARTOLSystematicBase.h"
#include <iostream>

template<typename T>
BARTOLSystematicBase<T>::BARTOLSystematicBase(std::string ParameterName_, size_t numParams_) {
  ParameterName = ParameterName_;
  numParams = numParams_;
  std::cout << ParameterName << " initialised" << std::endl;
}

template<typename T>
size_t BARTOLSystematicBase<T>::getNumParams(){
  return numParams;
}

template<typename T>
BARTOLSystematicBase<T>::~BARTOLSystematicBase() {
}
