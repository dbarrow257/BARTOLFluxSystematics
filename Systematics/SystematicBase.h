#pragma once

#include <string>

/**
 * @file SystematicBase.h
 *
 * @class SystematicBase
 *
 * @brief 
 */
template <typename T>
class SystematicBase {
 public:
  // ========================================================================================================================================================================
  // Public functions which are calculation implementation agnostic

  /**
   * @brief Constructor
   */
  SystematicBase(std::string ParameterName_);
  
  /**
   * @brief Destructor
   */
  ~SystematicBase();

  virtual T CalculateWeight(T DialValue, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) = 0;

private:
  std::string ParameterName;
};

template class SystematicBase<float>;
template class SystematicBase<double>;
