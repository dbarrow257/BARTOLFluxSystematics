#pragma once
#include "BARTOLSystematicBase.h"
/**
 * @file BARTOLSystematic_CosmicNormalisation.h
 *
 * @class BARTOLSystematic_CosmicNormalisation
 *
 * @brief 
 */
template <typename T>
class BARTOLSystematic_CosmicNormalisation : public BARTOLSystematicBase<T> {
 public:
  // ========================================================================================================================================================================
  // Public functions which are calculation implementation agnostic

  /*
   * @brief Constructor
   */
  BARTOLSystematic_CosmicNormalisation();
  
  /**
   * @brief Destructor
   */
  ~BARTOLSystematic_CosmicNormalisation();

  T CalculateWeight(const std::vector<T>& DialValues, size_t position, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) override;
};

template class BARTOLSystematic_CosmicNormalisation<float>;
template class BARTOLSystematic_CosmicNormalisation<double>;

