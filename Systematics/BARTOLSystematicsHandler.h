#pragma once
#include <vector>
#include "Systematics/BARTOLSystematicBase.h"

/**
 * @file BARTOLSystematicsHandler.h
 *
 * @class BARTOLSystematicsHandler
 *
 * @brief 
 */ 
template<typename T>
class BARTOLSystematicsHandler {
public:
  // ========================================================================================================================================================================
  // Public functions which are calculation implementation agnostic

  /**
   * @brief Constructor
   */
  BARTOLSystematicsHandler();
  
  /**
   * @brief Destructor
   */
  ~BARTOLSystematicsHandler();

  void SetDialValues(std::vector<T> DialValues_);
  T CalculateWeight(int GeneratedNeutrinoFlavourPDG, T NeutrinoEnergy, T NeutrinoCosineZ);

private:
  std::vector<BARTOLSystematicBase<T>*> Systematics;
  std::vector<T> DialValues;
  size_t nParams {0};
};
