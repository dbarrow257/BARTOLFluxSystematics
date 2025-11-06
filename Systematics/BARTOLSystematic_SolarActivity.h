#pragma once

#include "BARTOLSystematicBase.h"

/**
 * @file BARTOLSystematic_SolarActivity.h
 *
 * @class BARTOLSystematic_SolarActivity
 *
 * @brief 
 */
template <typename T>
class BARTOLSystematic_SolarActivity : public BARTOLSystematicBase<T> {
 public:
  // ========================================================================================================================================================================
  // Public functions which are calculation implementation agnostic

  /**
   * @brief Constructor
   */
  BARTOLSystematic_SolarActivity();
  
  /**
   * @brief Destructor
   */
  ~BARTOLSystematic_SolarActivity();

  T CalculateWeight(T DialValue, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) override;
};

template class BARTOLSystematic_SolarActivity<float>;
template class BARTOLSystematic_SolarActivity<double>;
