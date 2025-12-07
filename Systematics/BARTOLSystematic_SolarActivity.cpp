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

double getLogCentreY(TH2D* hist, int binNumber){
  auto lower = hist->GetYaxis()->GetBinLowEdge(binNumber);
  auto upper = hist->GetYaxis()->GetBinUpEdge(binNumber);
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
  std::unique_ptr<TFile> solarActivityFile= std::make_unique<TFile>("data/solar_activity/solarActivityRatio.root", "READ");
  if (!solarActivityFile || solarActivityFile->IsZombie()) {
    throw std::runtime_error("Error: Could not open solar activity ratio file.");
  }
  for(auto neutName : neutrinoNames){
    TH2D* hist = nullptr;
    solarActivityFile->GetObject(Form("solarActivityRatio_1.000_%s", neutName), hist);
    if (!hist) {
      throw std::runtime_error(Form("Error: Could not find histogram solarActivityRatio_1.000_%s in solar activity weights file.", neutName));
    }
    solarActivityHistograms.push_back(static_cast<TH2D*>(hist->Clone()));
  }
  TH2D* exampleHist = solarActivityHistograms[0];
  minBinCentreX = exampleHist->GetXaxis()->GetBinCenter(1);
  maxBinCentreX = exampleHist->GetXaxis()->GetBinCenter(exampleHist->GetNbinsX());
  minBinCentreY = getLogCentreY(exampleHist, 1);
  maxBinCentreY = getLogCentreY(exampleHist, exampleHist->GetNbinsY());
}

template<typename T>
BARTOLSystematic_SolarActivity<T>::~BARTOLSystematic_SolarActivity() {
  for(auto hist : solarActivityHistograms){
    delete hist;
  }
}

template<typename T>
T BARTOLSystematic_SolarActivity<T>::interpolateBasedOnHistogram(TH2D* hist, T NeutrinoEnergy_, T NeutrinoCosineZ_) {

  double logE = TMath::Log(NeutrinoEnergy_);
  logE = std::clamp(logE, minBinCentreY, maxBinCentreY);
  NeutrinoCosineZ_ = std::clamp(NeutrinoCosineZ_, minBinCentreX, maxBinCentreX);
  
  auto binX = (hist->GetXaxis())->FindBin(NeutrinoCosineZ_);
  double binXCentre = (hist->GetXaxis()->GetBinCenter(binX));
  auto binY = (hist->GetYaxis()->FindBin(NeutrinoEnergy_));
  auto binYCentre = getLogCentreY(hist, binY);

  auto binXNN = (NeutrinoCosineZ_ > binXCentre) ? binX + 1 : binX - 1;
  auto binYNN = (logE > binYCentre) ? binY +1 : binY - 1;

  binXNN = std::clamp(binXNN, 1, hist->GetNbinsX());
  binYNN = std::clamp(binYNN, 1, hist->GetNbinsY());

  double V00 = hist->GetBinContent(binX, binY);
  double V10 = hist->GetBinContent(binXNN, binY);
  double V01 = hist->GetBinContent(binX, binYNN);
  double V11 = hist->GetBinContent(binXNN, binYNN);

  double binXNNCentre = hist->GetXaxis()->GetBinCenter(binXNN);
  double binYNNCentre = getLogCentreY(hist, binYNN);

  double u = (binXNN != binX) ? (NeutrinoCosineZ_ - binXCentre) / (binXNNCentre - binXCentre) : 0.0;
  double v = (binYNN != binY) ? (logE - binYCentre) / (binYNNCentre - binYCentre) : 0.0;
  double P = (1-u)*(1-v)*V00 + u*(1-v)*V10 + (1-u)*v*V01 + u*v*V11;
  return P;

}

template<typename T>
T BARTOLSystematic_SolarActivity<T>::CalculateWeight(T DialValue, int GeneratedNeutrinoFlavourPDG_, T NeutrinoEnergy_, T NeutrinoCosineZ_) {
  auto histIndex = mapFlavourToHistogramIndex(GeneratedNeutrinoFlavourPDG_);
  if(histIndex == numOfNeuts){
    std::cout<<"Invalid Neutrino Flavour PDG must be plus or minus 12 or 14"<<std::endl;
    throw;
  }
  T minWeight = static_cast<T>(interpolateBasedOnHistogram( solarActivityHistograms[histIndex], NeutrinoEnergy_, NeutrinoCosineZ_));
  T weight = (1 -  DialValue*(1 - minWeight));
  return weight;
}
