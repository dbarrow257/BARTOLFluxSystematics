#include "Systematics/BARTOLSystematicsHandler.h"

#include <iostream>
#include <iomanip>

int main() {
  BARTOLSystematicsHandler<float> SystHandler = BARTOLSystematicsHandler<float>();

  std::vector<float> DialValues = {1.5};
  SystHandler.SetDialValues(DialValues);

  int NeutrinoFlavourPDG = 12;
  float NeutrinoEnergy = 1.0; //In GeV
  float NeutrinoCosineZ = 0.8; //+1 defined as directly downgoing, -1 defined as directly upgoing

  std::cout << "Calculating weight for -" << std::endl;
  std::cout << std::setw(25) << "Dial Value" << " = " << DialValues[0] << std::endl;
  std::cout << std::setw(25) << "NeutrinoFlavourPDG" << " = " << NeutrinoFlavourPDG << std::endl;
  std::cout << std::setw(25) << "NeutrinoEnergy" << " = " << NeutrinoEnergy << std::endl;
  std::cout << std::setw(25) << "NeutrinoCosineZ" << " = " << NeutrinoCosineZ << std::endl;
  std::cout << std::endl;
  std::cout << std::setw(25) << "Weight" << " = " << SystHandler.CalculateWeight(NeutrinoFlavourPDG,NeutrinoEnergy,NeutrinoCosineZ) << std::endl;
}
