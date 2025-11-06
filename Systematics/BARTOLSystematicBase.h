#pragma once

#include <string>

/**
 * @file BARTOLSystematicBase.h
 *
 * @class BARTOLSystematicBase
 *
 * @brief 
 */
template <typename T>
class BARTOLSystematicBase {
 public:
  // ========================================================================================================================================================================
  // Public functions which are calculation implementation agnostic

  /**
   * @brief Constructor
   */
  BARTOLSystematicBase(std::string ParameterName_);
  
  /**
   * @brief Destructor
   */
  ~BARTOLSystematicBase();

  virtual T CalculateWeight(T DialValue, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) = 0;

private:
  std::string ParameterName;
};

template class BARTOLSystematicBase<float>;
template class BARTOLSystematicBase<double>;
