#pragma once

#include "SystematicBase.h"

/**
 * @file Systematic_SolarActivity.h
 *
 * @class Systematic_SolarActivity
 *
 * @brief 
 */
template <typename T>
class Systematic_SolarActivity : public SystematicBase<T> {
 public:
  // ========================================================================================================================================================================
  // Public functions which are calculation implementation agnostic

  /**
   * @brief Constructor
   */
  Systematic_SolarActivity();
  
  /**
   * @brief Destructor
   */
  ~Systematic_SolarActivity();

  T CalculateWeight(T DialValue, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) override;
};

template class Systematic_SolarActivity<float>;
template class Systematic_SolarActivity<double>;
