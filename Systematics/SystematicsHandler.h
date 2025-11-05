#pragma once

#include <vector>
#include "Systematics/SystematicBase.h"

/**
 * @file SystematicBase.h
 *
 * @class SystematicBase
 *
 * @brief 
 */
template<typename T>
class SystematicsHandler {
public:
  // ========================================================================================================================================================================
  // Public functions which are calculation implementation agnostic

  /**
   * @brief Constructor
   */
  SystematicsHandler();
  
  /**
   * @brief Destructor
   */
  ~SystematicsHandler();

  void SetDialValues(std::vector<T> DialValues_);
  T CalculateWeight(int GeneratedNeutrinoFlavourPDG, T NeutrinoEnergy, T NeutrinoCosineZ);

private:
  std::vector<SystematicBase<T>*> Systematics;
  std::vector<T> DialValues;
  size_t nParams;
};
