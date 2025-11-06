#include "Systematics/BARTOLSystematicBase.h"

#include <iostream>

template<typename T>
BARTOLSystematicBase<T>::BARTOLSystematicBase(std::string ParameterName_) {
  ParameterName = ParameterName_;

  std::cout << ParameterName << " initialised" << std::endl;
}

template<typename T>
BARTOLSystematicBase<T>::~BARTOLSystematicBase() {
}
