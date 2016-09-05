// To run script, first compile with: g++ `root-config --cflags` srsCalib.C `root-config --glibs`. Then execute with ./a.out

// This script takes internal pulser data from the srs and produces a nonlinearity plot. You can use the MultiMux plots afterwards to put multiple curves on same plot

#include <iostream>
#include <fstream>
#include <vector>

#include "TGraphErrors.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TBranch.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include "TLegend.h"


using namespace std;


int main() {
  int nElectrons = 625;
  char fileName[256];
  int firstFile = 80;
  int lastFile = 210;

  ofstream file1;
  file1.open("IntChannels.txt", ofstream::out | ofstream::trunc);
  vector<Float_t> totalMean;
  vector<Float_t> totalICAL;
  vector<Float_t> zeros;
  vector<Float_t> meanErrors;
  vector<Float_t> ch0Mean;
  vector<Float_t> ch0Errors;
  vector<Float_t> ch1Mean;
  vector<Float_t> ch1Errors;
  vector<Float_t> ch2Mean;
  vector<Float_t> ch2Errors;
  vector<Float_t> ch3Mean;
  vector<Float_t> ch3Errors;
  vector<Float_t> ch4Mean;
  vector<Float_t> ch4Errors;
 
  vector<Float_t> meanArrayVec[60];
  vector<Float_t> errorArrayVec[60];  
  for (int i=firstFile; i<= lastFile; i+=2) {
    cout << "File: "<< i << endl;

    char fileName[256];
    sprintf(fileName,"/data/Trees/ExtPulserIcalScan/TreeExtPulserIcalScanRun%d.root",i);
    TFile *file = TFile::Open(fileName);
    if (!file) return 1;

    TTree *hitTree = new TTree("Hit","Hit");   
    hitTree = (TTree*) file->Get("Hit;1");
    //    cout << "1" << endl; 
    Int_t nEvents = (Int_t) hitTree->GetEntries();

    Int_t stripNo = 0;
    Float_t charge = 0;    
  
    TH1F *ch0Histo = new TH1F("ch0Histo","ch0Histo",112, 0, 2200);
  TH1F *ch1Histo = new TH1F("ch1Histo","ch1Histo",112, 0, 2200);
  TH1F *ch2Histo = new TH1F("ch2Histo","ch2Histo",112, 0, 2200);
  TH1F *ch3Histo = new TH1F("ch3Histo","ch3Histo",112, 0, 2200);
  TH1F *ch4Histo = new TH1F("ch4Histo","ch4Histo",112, 0, 2200);  
  TH1F *chHisto = new TH1F("chHisto","chHisto",480, 0, 479);
      TH1F *histoArray[480];
    hitTree->SetBranchAddress("PadNo",&stripNo);
    hitTree->SetBranchAddress("MaxCharge",&charge);

    vector<float> tpVec[480];
    for (int j = 0; j < 480; ++j) {
      char hName[100];
      sprintf(hName,"ch%d",j);
      histoArray[j]= new TH1F(hName,hName,112, 0, 2200);  
    }
    for (Int_t j = 0; j < nEvents; ++j) {   

        hitTree->GetEntry(j);

        tpVec[stripNo].push_back(charge);
        if (stripNo == 5) ch0Histo->Fill(charge);
 if (stripNo == 13) ch1Histo->Fill(charge);
 if (stripNo == 21) ch2Histo->Fill(charge);
 if (stripNo == 25) ch3Histo->Fill(charge);
 if (stripNo == 33) ch4Histo->Fill(charge);
 histoArray[stripNo]->Fill(charge);
    }
    Float_t tpMean[480];
    TH1F *meanHisto = new TH1F("meanHisto","meanHisto",112, 0, 2200);
    for (int j=0; j<480; ++j) {
      
      float total  = 0;
      for (int k = 0; k<(int)tpVec[j].size(); ++k) {
	if ((int) tpVec[j].size() > 200)  total += tpVec[j][k];
      }
      if ((int) tpVec[j].size() > 200) tpMean[j] = total / (int)tpVec[j].size();
      else tpMean[j] = 0;
      chHisto->Fill(j,tpMean[j]);
    }
    int z =0;
    for (int j=0; j<128; ++j) {
      if (histoArray[j]->GetEntries() < 500 /*||  j > 256 */|| j > 127  ) continue;
      if (tpMean[j] != 0) meanHisto->Fill(tpMean[j]);
      if (j==5) {ch0Mean.push_back(ch0Histo->GetMean()); ch0Errors.push_back(ch0Histo->GetMeanError());}
  if (j==13) {ch1Mean.push_back(ch1Histo->GetMean()); ch1Errors.push_back(ch1Histo->GetMeanError());}
  if (j==21) {ch2Mean.push_back(ch2Histo->GetMean()); ch2Errors.push_back(ch2Histo->GetMeanError());}
  if (j==25) {ch3Mean.push_back(ch3Histo->GetMean()); ch3Errors.push_back(ch3Histo->GetMeanError());}
  if (j==33) {ch4Mean.push_back(ch4Histo->GetMean()); ch4Errors.push_back(ch4Histo->GetMeanError());}
 
  //    cout << " j " << j << " z " << z << endl;
 meanArrayVec[z].push_back(histoArray[j]->GetMean()); errorArrayVec[z].push_back(histoArray[j]->GetMeanError());

 // if (i == 50) file1 << j << ", " ;
 //  cout << " i " << i << " error " << histoArray[j]->GetMeanError() << endl;
 ++z;
   
      // if ( i == 400 ) cout << "mean " << tpMean[j] << " j " << j << endl;
    } 
    // if (i == 50 && z != 60) {cout << "warning wrote file but wrong z" << endl; cin >> z;}
    //    if (z > 60 ) {cout << " z LARGE " << z << endl; cin >> z;} 
    Float_t mean = meanHisto->GetMean();
    TCanvas *c1 = new TCanvas("c1","c1");
    meanHisto->Draw();
    char fName[256];
    sprintf(fName, "../ExtPulserIcalScan/MeanDist%dmV.png",i);
    char title[256];
    sprintf(title, "Mean ADC all Channels ical %d; ADC; ",i);
    meanHisto->SetTitle(title);  
    // c1->SaveAs(fName);

TCanvas *c1A = new TCanvas("c1A","c1A");
    chHisto->Draw();
    char fNameA[256];
    sprintf(fNameA, "../ExtPulserIcalScan/ChHisto%dmV.root",i);
    char titleA[256];
    sprintf(titleA, "Mean ADC per Channel ical %d; ADC; ",i);
    chHisto->SetTitle(titleA);  
    //c1A->SaveAs(fNameA);
    //cout << " mv: " << i << " mean " << mean << endl;
    totalMean.push_back(mean);
    totalICAL.push_back(i*nElectrons);
    zeros.push_back(0);
    meanErrors.push_back(meanHisto->GetMeanError());
    for (int j=0; j < 480; ++j) delete histoArray[j];
    delete meanHisto;
    delete c1;
    delete c1A;
    delete chHisto;
    delete ch0Histo;
    delete ch1Histo;
    delete ch2Histo;
    delete ch3Histo;
    delete ch4Histo;
delete hitTree;
  }
  
  int count = (int) totalMean.size();
  TCanvas *c2 = new TCanvas("c2","c2");
  TGraphErrors *g1 = new TGraphErrors(count,&totalICAL[0],&totalMean[0],&zeros[0],&meanErrors[0]);
  g1->SetTitle("SRS Calibration with Internal Pulser; N_{electrons}; Avg ADC");
  char g1Name[256];
  g1->Draw("APL");
  g1->SetMarkerStyle(20);
  g1->SetMarkerColor(4);
  g1->SetMarkerSize(.25);
  sprintf(g1Name,"../ExtPulserIcalScan/Calibration.root");
  c2->SaveAs(g1Name); 
  delete c2; 


  TCanvas *c2 = new TCanvas("c2","c2");
  TGraphErrors *g1 = new TGraphErrors(count,&totalMean[0],&totalICAL[0],&meanErrors[0],&zeros[0]);
  g1->SetTitle("SRS Calibration with Internal Pulser; Avg ADC; N_{electrons}");
  char g1Name[256];
  g1->Draw("APL");
  g1->SetMarkerStyle(20);
  g1->SetMarkerColor(4);
  g1->SetMarkerSize(.25);
  sprintf(g1Name,"../ExtPulserIcalScan/CalibrationReverse.root");
  c2->SaveAs(g1Name); 
  delete c2; 

TCanvas *c3 = new TCanvas("c3","c3");
  TMultiGraph *mg = new TMultiGraph();
  mg->SetTitle("SRS Calibration; N_{electrons}; Avg ADC");
  
  TGraphErrors *gCh0 = new TGraphErrors(count,&totalICAL[0],&ch0Mean[0],&zeros[0],&ch0Errors[0]);
   TGraphErrors *gCh1 = new TGraphErrors(count,&totalICAL[0],&ch1Mean[0],&zeros[0],&ch1Errors[0]);
 TGraphErrors *gCh2 = new TGraphErrors(count,&totalICAL[0],&ch2Mean[0],&zeros[0],&ch2Errors[0]);
 TGraphErrors *gCh3 = new TGraphErrors(count,&totalICAL[0],&ch3Mean[0],&zeros[0],&ch3Errors[0]);
 TGraphErrors *gCh4 = new TGraphErrors(count,&totalICAL[0],&ch4Mean[0],&zeros[0],&ch4Errors[0]);
  
  gCh0->SetMarkerStyle(20);
  gCh0->SetMarkerColor(4);
  gCh0->SetLineColor(4);
  gCh0->SetMarkerSize(.25);
 gCh1->SetMarkerStyle(21);
  gCh1->SetMarkerColor(3);
 gCh1->SetLineColor(3);
  gCh1->SetMarkerSize(.25);
 gCh2->SetMarkerStyle(22);
  gCh2->SetMarkerColor(6);
 gCh2->SetLineColor(6);
  gCh2->SetMarkerSize(.25);
 gCh3->SetMarkerStyle(23);
  gCh3->SetMarkerColor(7);
 gCh3->SetLineColor(7);
  gCh3->SetMarkerSize(.25);
 gCh4->SetMarkerStyle(24);
  gCh4->SetMarkerColor(2);
 gCh4->SetLineColor(2);
  gCh4->SetMarkerSize(.25);

  mg->Add(gCh0);
  mg->Add(gCh1);
  mg->Add(gCh2);
  mg->Add(gCh3);
  mg->Add(gCh4);

  mg->Draw("APL");

  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();
  
  TLegend *leg = new TLegend(0.46,0.7,0.7,0.9);
  //  leg->SetTextFont(72);
  //leg->SetTextSize(0.04);
  leg->SetFillColor(0);
  leg->AddEntry(gCh0,"Ch 5","ple");
  leg->AddEntry(gCh1,"Ch 13","ple");
  leg->AddEntry(gCh2,"Ch 21","ple");
  leg->AddEntry(gCh3,"Ch 25","ple");
  leg->AddEntry(gCh4,"Ch 33","ple");
  leg->Draw();
  char mgName[256];
  sprintf(mgName,"../ExtPulserIcalScan/CalibrationCh0-4.root");
  c3->SaveAs(mgName); 



TCanvas *c5 = new TCanvas("c5","c5");
  TMultiGraph *mg3 = new TMultiGraph();
  mg3->SetTitle("SRS Calibration; N_{electrons}; Avg ADC");
  
  TGraphErrors *gArray[60];

  //TLegend *leg3 = new TLegend(0.46,0.7,0.7,0.9);
  //leg3->SetFillColor(0);
  for (int j=0; j<16; ++j) {
    
    gArray[j] = new TGraphErrors((errorArrayVec[j]).size(),&totalICAL[0],&(meanArrayVec[j])[0],&zeros[0],&(errorArrayVec[j])[0]);
    
    gArray[j]->SetMarkerStyle(20);
    gArray[j]->SetMarkerSize(0.25);
    mg3->Add(gArray[j]);
    //  for (int k=0; k< (meanArrayVec[j]).size(); ++k) cout << " j " << j << " k " << k <<  errorArrayVec[j][k] << endl; 

    //leg3->AddEntry(gArray[j],"Ch,"ple");
  }


   mg3->Draw("APL");

    mg3->GetXaxis()->CenterTitle();
    mg3->GetYaxis()->CenterTitle();
  char mg3Name[256];
  sprintf(mg3Name,"../ExtPulserIcalScan/calibrationAllCh.root");
  c5->SaveAs(mg3Name); 

  for (int j=0; j<16; ++j) delete gArray[j];
  delete mg;  delete mg3;
  file1.close();
  return 0;
}
