#include "Systematics/BARTOLSystematic_SolarActivity.h"
#include <TDirectory.h>
#include <TH2.h>
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <TFile.h>
#include <array>
#include <cmath>
#include <TMath.h>
#include <iostream>
#include <stdexcept>
#include <TCanvas.h>
namespace{
  const char* neutrinoNames[] = {"Nue", "Antinue", "Numu", "Antinumu"};
  enum neutrinoID : size_t{
    Nue,
    Antinue,
    Numu,
    Antinumu,
    numOfNeuts

};

constexpr std::array<int, numOfNeuts> neutrinoPDGNumbers = {12, -12, 14, -14};
}

double getLogCentreY(const TH2D& hist, int binNumber){
  auto lower = hist.GetYaxis()->GetBinLowEdge(binNumber);
  auto upper = hist.GetYaxis()->GetBinUpEdge(binNumber);
  return TMath::Log(TMath::Sqrt(lower*upper));
}

std::size_t mapFlavourToHistogramIndex(int x) {
  switch (x) {
      case 12:  return 0;
      case -12: return 1;
      case 14:  return 2;
      case -14: return 3;
      default: return numOfNeuts;
  }
}

template<typename T>
BARTOLSystematic_SolarActivity<T>::BARTOLSystematic_SolarActivity() : BARTOLSystematicBase<T>("SolarActivity") {
  solarActivityFile= std::make_unique<TFile>("/data/t2k/users/coveneyt/T2KSKBartol/BARTOLFluxSystematics/Systematics/data/solarActivityRatio.root", "READ");
  if (!solarActivityFile || solarActivityFile->IsZombie()) {
    throw std::runtime_error("Error: Could not open solar activity ratio file.");
  }
  std::cout<<"hello there \n";
  for(auto neutName : neutrinoNames){
    TH2D* bartolHist_i = nullptr;
    solarActivityFile->GetObject(Form("solarActivityRatio_%s", neutName), bartolHist_i);
    if (!bartolHist_i) {
      throw std::runtime_error(Form("Error: Could not find bartolHist_iogram solarActivityRatio_ solar activity weights file.", neutName));
    }
    std::cout<<bartolHist_i->GetTitle()<<std::endl;
    TH2D temp = *(bartolHist_i);
    solarActivityHistograms.push_back(temp);
  }
  const TH2D exampleHist = solarActivityHistograms[0];
  minBinCentreX = exampleHist.GetXaxis()->GetBinCenter(1);
  maxBinCentreX = exampleHist.GetXaxis()->GetBinCenter(exampleHist.GetNbinsX());
  minBinCentreY = getLogCentreY(exampleHist, 1);
  maxBinCentreY = getLogCentreY(exampleHist, exampleHist->GetNbinsY());
  solarActivityFile->Close();
}

template<typename T>
BARTOLSystematic_SolarActivity<T>::~BARTOLSystematic_SolarActivity() {

}

template<typename T>
T BARTOLSystematic_SolarActivity<T>::interpolateBasedOnHistogram(const TH2D& bartolHist, T NeutrinoEnergy_, T NeutrinoCosineZ_) {
  if(bartolHist.IsZombie()){
    std::cout<<"Error something happened to the histogram!"<<std::endl;
    throw;
  }
  double logE = TMath::Log(NeutrinoEnergy_);
  logE = std::clamp(logE, minBinCentreY, maxBinCentreY);
  NeutrinoCosineZ_ = std::clamp(NeutrinoCosineZ_, minBinCentreX, maxBinCentreX);
  auto binX = (bartolHist.GetXaxis())->FindBin(NeutrinoCosineZ_);
  double binXCentre = (bartolHist.GetXaxis()->GetBinCenter(binX));
  auto binY = (bartolHist.GetYaxis()->FindBin(NeutrinoEnergy_));
  binY = std::clamp(binY, 1, bartolHist.GetNbinsY());
  auto binYCentre = getLogCentreY(bartolHist, binY);

  auto binXNN = (NeutrinoCosineZ_ > binXCentre) ? binX + 1 : binX - 1;
  auto binYNN = (logE > binYCentre) ? binY +1 : binY - 1;

  binXNN = std::clamp(binXNN, 1, bartolHist.GetNbinsX());
  binYNN = std::clamp(binYNN, 1, bartolHist.GetNbinsY());



  if (binX < 1 || binX > bartolHist.GetNbinsX() ||
  binY < 1 || binY > bartolHist.GetNbinsY() ||
  binXNN < 1 || binXNN > bartolHist.GetNbinsX() ||
  binYNN < 1 || binYNN > bartolHist.GetNbinsY()) {
    std::cout << "==== BARTOLSystematic_SolarActivity::interpolateBasedOnHistogram DEBUG ====\n";

    // bartolHistogram info
    std::cout << "bartolHistogram: " << bartolHist.GetTitle() <<"did it not appear?" << "\n"
              << "  NbinsX=" << bartolHist.GetNbinsX()
              << "  Xmin=" << minBinCentreX
              << "  Xmax=" << maxBinCentreX << "\n"
              << "  NbinsY=" << bartolHist.GetNbinsY()
              << "  Ymin=" << minBinCentreY
              << "  Ymax=" << maxBinCentreY << "\n"
              << "X and y axis" << bartolHist . GetXaxis()->GetName()<<" " << bartolHist.GetYaxis()->GetName()<< "\n";

    // Input values
    std::cout << "Input values:\n"
              << "  NeutrinoEnergy = " << NeutrinoEnergy_ << "\n"
              << "  logE(raw)      = " << TMath::Log(NeutrinoEnergy_) << "\n"
              << "  logE(clamped)  = " << logE << "\n"
              << "  CosZ(raw)      = " << NeutrinoCosineZ_ << "\n";

    // Axis centers
    std::cout << "Bin centers:\n"
              << "  binX = " << binX
              << "  Xcenter = " << binXCentre << "\n"
              << "  binY = " << binY
              << "  Ycenter(log) = " << binYCentre << "\n";


    std::cout << "Nearest-neighbor bins:\n"
              << "  binXNN = " << binXNN
              << "  XNNcenter = " << bartolHist.GetXaxis()->GetBinCenter(binXNN) << "\n"
              << "  binYNN = " << binYNN
              << "  YNNcenter(log) = " << getLogCentreY(bartolHist, binYNN) << "\n";

    std::cout << "Invalid bin indices detected!\n"
              << "  binX=" << binX << " (valid: 1-" << bartolHist.GetNbinsX() << ")\n"
              << "  binY=" << binY << " (valid: 1-" << bartolHist.GetNbinsY() << ")\n"
              << "  binXNN=" << binXNN << " (valid NN)\n";
              
              TFile outFile("debug_bartolHistograms.root", "RECREATE");
              outFile.WriteTObject(&bartolHist);
              outFile.Close();
              std::cout<<"output saved to debug_histograms.root"<<std::endl;
          

    
    throw std::runtime_error("failure - check above information");
  }
  double V00 = bartolHist.GetBinContent(binX, binY);
  double V10 = bartolHist.GetBinContent(binXNN, binY);
  double V01 = bartolHist.GetBinContent(binX, binYNN);
  double V11 = bartolHist.GetBinContent(binXNN, binYNN);

  double binXNNCentre = bartolHist.GetXaxis()->GetBinCenter(binXNN);
  double binYNNCentre = getLogCentreY(bartolHist, binYNN);

  double u = (binXNN != binX) ? (NeutrinoCosineZ_ - binXCentre) / (binXNNCentre - binXCentre) : 0.0;
  double v = (binYNN != binY) ? (logE - binYCentre) / (binYNNCentre - binYCentre) : 0.0;
  double P = (1-u)*(1-v)*V00 + u*(1-v)*V10 + (1-u)*v*V01 + u*v*V11;
  return P;

}

template<typename T>
T BARTOLSystematic_SolarActivity<T>::CalculateWeight(T DialValue, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) {
  auto histIndex = mapFlavourToHistogramIndex(GeneratedNeutrinoFlavourPDG_);
  if(histIndex >= solarActivityHistograms.size()){
    std::cout<<"Invalid Neutrino Flavour PDG must be plus or minus 12 or 14"<<std::endl;
    throw;
  }
  T minWeight = static_cast<T>(interpolateBasedOnHistogram( solarActivityHistograms.at(histIndex), NeutrinoEnergy_, NeutrinoCosineZ_));
  T weight = (1 -  DialValue*(1 - minWeight));
  return weight;
}
