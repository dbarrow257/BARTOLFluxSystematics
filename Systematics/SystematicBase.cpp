#include "Systematics/SystematicBase.h"

#include <iostream>

template<typename T>
SystematicBase<T>::SystematicBase(std::string ParameterName_) {
  ParameterName = ParameterName_;

  std::cout << ParameterName << " initialised" << std::endl;
}

template<typename T>
SystematicBase<T>::~SystematicBase() {
}
