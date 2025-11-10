#include <array>
#include <cmath>
#include <stdexcept>
#include "BARTOLSystematic_CosmicNormalisation.h"
#include <span>
namespace{
    //units are in (GeV/n)−1m−2s−1sr−1
    constexpr std::array<double, 4> normNominalH {14275, 2.44, -0.36, 2.75};
    const std::string systematicName{"CosmicNorm"};
    constexpr size_t numOfDials{4};
    //since helium has 4 nucleons, multiply by 4.
    constexpr std::array<double, 4> normNominalHe{4*531, 4*1.24, 4*(-0.36), 4*2.63};
    enum particleID{
        H,
        He
    };
    }
    
    //values for these come from Justin Evans et al. Uncertainties in atmospheric muon-neutrino fluxes arising from comsic-ray primaries;
    //Formula gives a global fit to data for the primary cosmic ray flux at solar minimum.
    //Might have to look into using alternative parameterisation for hydrogen given in Evans paper which is more accurate but I think Bartol uses the standard GSHL parameterisation.
    template <typename T>
    double fluxAtSolarMin(double energy, std::array<T, 4> normParams){
      return normParams[0]*std::pow((energy+normParams[1]*std::exp(normParams[2]*std::sqrt(energy))), -1*normParams[3]);
    }
    template <typename T>
    double fluxAtSolarMin(double energy, std::span<T, 4> normParams){
        return normParams[0]*std::pow((energy+normParams[1]*std::exp(normParams[2]*std::sqrt(energy))), -1*normParams[3]);
      }
    
    template <typename T>
    double eventWeightNormalisation(double energy, std::array<T, 4> normParams, particleID ID){
      if (ID == H){
          return fluxAtSolarMin(energy, normParams)/fluxAtSolarMin(energy, normNominalH);
      }
      if (ID == He){
          return fluxAtSolarMin(energy, normParams)/fluxAtSolarMin(energy, normNominalHe);       
      }
      throw std::runtime_error("H or He expected");
    }
    template <typename T>
    double eventWeightNormalisation(double energy, std::span<T, 4> normParams, particleID ID){
        if (ID == H){
            return fluxAtSolarMin(energy, normParams)/fluxAtSolarMin(energy, normNominalH);
        }
        if (ID == He){
            return fluxAtSolarMin(energy, normParams)/fluxAtSolarMin(energy, normNominalHe);       
        }
        throw std::runtime_error("H or He expected");
      }

template<typename T>
BARTOLSystematic_SolarActivity<T>::BARTOLSystematic_SolarActivity() : BARTOLSystematicBase<T>(systematicName, numOfDials) {
}

template<typename T>
T BARTOLSystematic_CosmicNormalisation<T>::CalculateWeight(const std::vector<T>& DialValues, size_t position, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_){
    std::span<T> Span {DialValues};
    std::span<T> normParams {Span.subspan(position, numOfDials)};
    return static_cast<T>(eventWeightNormalisation(NeutrinoEnergy_, normParams, H));


}
