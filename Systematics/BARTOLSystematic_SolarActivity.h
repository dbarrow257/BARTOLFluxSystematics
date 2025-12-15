#pragma once
#include <TH2.h>
#include <TH2D.h>
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
  private:
    std::vector<TH2D> solarActivityHistograms;
    T interpolateBasedOnHistogram(const TH2D& hist, T NeutrinoEnergy_, T NeutrinoCosineZ_);
    T minBinCentreX;
    T maxBinCentreX;
    double minBinCentreY;
    double maxBinCentreY;
    std::unique_ptr<TFile> solarActivityFile;
};

template class BARTOLSystematic_SolarActivity<float>;
template class BARTOLSystematic_SolarActivity<double>;
