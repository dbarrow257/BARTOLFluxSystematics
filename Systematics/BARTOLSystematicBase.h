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
  BARTOLSystematicBase(std::string ParameterName_, size_t numParams);
  
  /**
   * @brief Destructor
   */
  ~BARTOLSystematicBase();


  virtual T CalculateWeight(T DialValues, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) = 0;
  size_t getNumParams();
private:
  std::string ParameterName;
  size_t numParams;
};

template class BARTOLSystematicBase<float>;
template class BARTOLSystematicBase<double>;
