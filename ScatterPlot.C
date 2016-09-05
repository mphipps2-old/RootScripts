// This comes 3rd in the series of scripts needed to process xray scan data. This finds final correlation offsets and slope and then finds error functions
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>

#include <TIterator.h>
#include <TString.h>
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TList.h"
#include "TMath.h"
#include "TROOT.h"
#include "TObjString.h"
#include "TIterator.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TCanvas.h"


using namespace std;

void main() {  

  gROOT->SetBatch(kTRUE);

  Int_t nFiles = 100;
  TFile* file6 = TFile::Open("/data/mike/ZZ48Scan/globalCorrelation.root");
  if (!file6) return;
  gStyle->SetOptStat(0);
  file6->ls();
  TCanvas *c57 = (TCanvas*) file6->Get("c57");
  TH2F *globalCorrelation = (TH2F*) c57->GetPrimitive("globalCorrelation");
  TH2F *globalCorrelationNew = new TH2F("localCorrelation", "Motor Position vs. Centroid", 1500, 30.5, 60.5, 300,35.5,65.5);
  globalCorrelationNew->GetXaxis()->SetTitle("Board Position (mm)");
  globalCorrelationNew->GetYaxis()->SetTitle("Motor Position (mm)");
  TH2F *globalAcc = new TH2F("localError", "Local Error Vs Centroid Position", 300, 30.5, 60.5, 80,-1000,1000);
  TH2F *globalAccUniv = new TH2F("globalErrorUniversal", "Universal Error Vs Centroid Position", 100, 0, 1, 80,-1000,1000);
   TH2F *globalAccUniv2 = new TH2F("globalErrorUniversal2", "Universal Error Vs Centroid Position", 100, 0, 1, 80,-1000,1000);
  TH2F *globalAccUniv3 = new TH2F("globalErrorUniversal3", "Universal Error Vs Centroid Position", 100, 0, 1, 80,-1000,1000);
  //TH2F *globalAccUniv = new TH2F("globalErrorUniversal", "Universal Error Vs Centroid Position", 200, -1, 1, 80,-1000,1000);
  //TH2F *globalAccUniv2 = new TH2F("globalErrorUniversal2", "Universal Error Vs Centroid Position", 200, -1, 1, 80,-1000,1000);
  //TH2F *globalAccUniv3 = new TH2F("globalErrorUniversal3", "Universal Error Vs Centroid Position", 200, -1, 1, 80,-1000,1000);

  TH1F *errorFunction = new TH1F("errorFunctionUniversal", "Universal Error Function", 100,0,1);
  TH1F *errorFunction2 = new TH1F("errorFunctionUniversal2", "Universal Error Function: 2 Pad Events", 100,0,1);
  TH1F *errorFunction3 = new TH1F("errorFunctionUniversal3", "Universal Error Function: 3 Pad Events", 100,0,1);

  TProfile *profCorr = globalCorrelation->ProfileX();
  float oldConst = 11.178;
  float lowerLim = 42 ;// (2204.7246667 * 6.438856216/1000) + oldConst+0.5;
  float upperLim = 51; //(4724.41 * 6.438856216/1000) + oldConst-0.5;
  profCorr->Fit("pol1","RQ0","",lowerLim ,upperLim);
  TF1 *fit = profCorr->GetFunction("pol1");
  float offset = fit->GetParameter(0);  //unit of pads
  float slope = fit->GetParameter(1);  //unit of pads
  float convConst = 1 / slope;
  cout << " lower lim " << lowerLim << " upper lim " << upperLim << " offset " << offset << " convConst " << convConst << endl;
  float xPos;
  float yPos;
  int runNum;

 ifstream file3("postest_run_80000.txt"); 
  if(!file3.is_open()) cerr << " file not open" << endl;
  TString line;
  int fileNum = 0;

  
  while (line.ReadLine(file3) && fileNum < nFiles) {
    ++fileNum;
    gROOT->cd(0);
    // strip leading spaces and skip comments
    line.Remove(TString::kBoth, ' ');
    // Array of tokens separated by "|"
    TObjArray *tokens = line.Tokenize("|");
    TIter myIter(tokens);

    TObjString *st;
    int count = 0; 
    while( st = (TObjString*) myIter.Next()) {        
      if (count == 0) xPos =  (st->GetString().Remove(TString::kBoth,' ')).Atof();
      else if (count == 1) yPos =  (st->GetString().Remove(TString::kBoth,' ')).Atof();
      else if (count == 2) runNum =  (st->GetString().Remove(TString::kBoth,' ')).Atoi();
      ++count;
    }
    cout<<"========= RunNum: "<<runNum<<", xMotor: "<<xPos<<", yMotor: "<<yPos<<" ========"<<endl;

    
    
    
    char treeFile[150];
    sprintf(treeFile,"/data/Trees/ZZ48/TreeFITZZ48Scan1Run%d.root",runNum);
    


    TFile *f = new TFile(treeFile);


      TTree *event = (TTree*) f->Get("Event;1");
    TTree *cluster = (TTree*) f->Get("Cluster;1");
    TTree *hit = (TTree*) f->Get("Hit;1");

    Int_t nEvents = (Int_t) event->GetEntries();
    Int_t cCount = 0;
    Int_t eEventNum = 0;
    Int_t cEventNum = 0;
    event->SetBranchAddress("EventNo",&eEventNum);
    cluster->SetBranchAddress("ClusterEventNo",&cEventNum);
    Int_t eClusterMult = 0;
    event->SetBranchAddress("ClusterMult",&eClusterMult);
    Int_t cClusterSize = 0;
    cluster->SetBranchAddress("ClusterSize",&cClusterSize);
    Int_t cTimeLength = 0;
    cluster->SetBranchAddress("TimeLength",&cTimeLength);
    Float_t cPeakCharge = 0;
    cluster->SetBranchAddress("PeakCharge",&cPeakCharge);
    Float_t cClusterCharge = 0;
    cluster->SetBranchAddress("ClusterCharge",&cClusterCharge);
    Float_t cCentroid = 0;
    cluster->SetBranchAddress("Centroid",&cCentroid);
    Float_t hAvgCharge = 0;
    hit->SetBranchAddress("AvgCharge",&hAvgCharge);

    
    
    for (int i=0; i < nEvents; ++i) {
      event->GetEntry(i);
      if (eClusterMult > 60 ) { 
        cCount+=eClusterMult;
        continue;
      }
      // if you cut event, add cluster mult to cCount
      for (int j=0; j < eClusterMult; ++j) {
        cluster->GetEntry(cCount);
        cCentroid -= 336;

	if (cCentroid < 0 || cCentroid > 48) {++cCount; continue;}
        if (cEventNum != eEventNum) {
         
          cerr << "Cluster and event info mismatched: event " << i << " cluster " << cEventNum << " real event number " << eEventNum <<  endl;
          return -1;
	}
        if (cClusterCharge <= 2000 || cTimeLength <= 8 ) {
          ++cCount;
	  continue;
        }

        Float_t xCentroid = (cCentroid*2000)+1000;
    
    
        if (xCentroid == xCentroid && xCentroid != 0 ) {
          if (cClusterSize > 1) {
            globalCorrelationNew->Fill(xCentroid/1000,(xPos*(-6.09219948)/1000)+100+5.278);
          }
        }
        ++cCount;
      }

    }
  }

  // file3.close();

  TProfile *profCorrNew = globalCorrelationNew->ProfileX();
  TCanvas *cTest = new TCanvas("cTest","cTest");
  globalCorrelationNew->Draw();
    globalCorrelation->Draw();
  cTest->SaveAs("/data/mike/ZZ48Scan/globalCorrelationCorrected.root");  
    profCorrNew->Fit("pol1","RQ0","",lowerLim ,upperLim);
  TF1 *fitNew = profCorrNew->GetFunction("pol1");
  float offsetNew = fitNew->GetParameter(0);  //unit of pads
  float slopeNew = fitNew->GetParameter(1);  //unit of pads

    cout << " new slope " << slopeNew << " new offset " << offsetNew << endl;


  ifstream file4("postest_run_80000.txt"); 
  if(!file4.is_open()) cerr << " file not open" << endl;
  TString line;
  int fileNum = 0;
  while (line.ReadLine(file4) && fileNum < nFiles) {
    ++fileNum;
    gROOT->cd(0);
    // strip leading spaces and skip comments
    line.Remove(TString::kBoth, ' ');
    // Array of tokens separated by "|"
    TObjArray *tokens = line.Tokenize("|");
    TIter myIter(tokens);

    TObjString *st;
    int count = 0; 
    while( st = (TObjString*) myIter.Next()) {        
      if (count == 0) xPos =  (st->GetString().Remove(TString::kBoth,' ')).Atof();
      else if (count == 1) yPos =  (st->GetString().Remove(TString::kBoth,' ')).Atof();
      else if (count == 2) runNum =  (st->GetString().Remove(TString::kBoth,' ')).Atoi();
      ++count;
    }
    cout<<"========= RunNum: "<<runNum<<", xMotor: "<<xPos<<", yMotor: "<<yPos<<" ========"<<endl;

    char treeFile[150];

    sprintf(treeFile,"/data/Trees/ZZ48/TreeFITZZ48Scan1Run%d.root",runNum);   


    TFile *f = new TFile(treeFile);
      TTree *event = (TTree*) f->Get("Event;1");
    TTree *cluster = (TTree*) f->Get("Cluster;1");
    TTree *hit = (TTree*) f->Get("Hit;1");


    Int_t nEvents = (Int_t) event->GetEntries();
    Int_t cCount = 0;
    Int_t eEventNum = 0;
    Int_t cEventNum = 0;
    event->SetBranchAddress("EventNo",&eEventNum);
    cluster->SetBranchAddress("ClusterEventNo",&cEventNum);
    Int_t eClusterMult = 0;
    event->SetBranchAddress("ClusterMult",&eClusterMult);
    Int_t cClusterSize = 0;
    cluster->SetBranchAddress("ClusterSize",&cClusterSize);
    Int_t cTimeLength = 0;
    cluster->SetBranchAddress("TimeLength",&cTimeLength);
    Float_t cPeakCharge = 0;
    cluster->SetBranchAddress("PeakCharge",&cPeakCharge);
    Float_t cClusterCharge = 0;
    cluster->SetBranchAddress("ClusterCharge",&cClusterCharge);
    Float_t cCentroid = 0;
    cluster->SetBranchAddress("Centroid",&cCentroid);
    Float_t hAvgCharge = 0;
    hit->SetBranchAddress("AvgCharge",&hAvgCharge);


    
    for (int i=0; i < nEvents; ++i) {
      event->GetEntry(i);
      
      if (eClusterMult > 10 ) { 
        cCount+=eClusterMult;
        continue;
      }
      // if you cut event, add cluster mult to cCount
      for (int j=0; j < eClusterMult; ++j) {
        cluster->GetEntry(cCount);
        cCentroid -= 336;

	if (cCentroid < 0 || cCentroid > 48) {++cCount; continue;}
        if (cEventNum != eEventNum) {
         
          cerr << "Cluster and event info mismatched: event " << i << " cluster " << cEventNum << " real event number " << eEventNum <<  endl;
          return -1;
	}
        if (cClusterCharge <= 2000 || cTimeLength <= 8  ) {
          ++cCount;
	  continue;
        }

        Float_t xCentroid = (cCentroid*2000)+1000;
    
    
        if (xCentroid == xCentroid && xCentroid != 0 ) {
          if (cClusterSize > 1) {
	      float error = xCentroid-(((xPos*(-6.09219948))+100000))+ 5401; 
	    //            float error = xCentroid-(xPos*convConst)+(offsetNew*1000);
            float position = fmod((xCentroid/1000),2)-1;
		     globalAcc->Fill((xCentroid/1000),error);  
	    	    if (position < 0) error *= -1;
            
		    globalAccUniv->Fill(TMath::Abs(position),error);
	    	    //globalAccUniv->Fill(position,error);
	    //cout << " xcentroid " << xCentroid

	    if (cClusterSize == 5) globalAccUniv2->Fill(TMath::Abs(position), error);
            //if (cClusterSize == 5) globalAccUniv2->Fill(position, error);
	    if (cClusterSize == 6) globalAccUniv3->Fill(TMath::Abs(position), error);
	    //    if (cClusterSize == 6) globalAccUniv3->Fill(position, error);
         
          }
        }
        ++cCount;
      }

    }
  }


 
    globalAcc->Smooth(1);
  globalAccUniv->Smooth(1);
  globalAccUniv2->Smooth(1);
  globalAccUniv3->Smooth(1);
  TProfile *prof = globalAccUniv->ProfileX();
  TProfile *prof2 = globalAccUniv2->ProfileX();
  TProfile *prof3 = globalAccUniv3->ProfileX();
  TProfile *profLocal = globalAcc->ProfileX();

  TCanvas *c56 = new TCanvas("c56","c56");
  prof->SetTitle("Universal Error vs Pad Position; Position across Chevron (mm); Mean Residual (#mum)");
  prof->Draw();
  c56->SaveAs("/data/mike/ZZ48Scan/errorProfileSmoothUnivHalf.root");  

  TCanvas *c66 = new TCanvas("c66","c66");
  prof2->SetTitle("Universal Error vs Pad Position: 5 Pad Events; Position across Chevron (mm); Mean Residual (#mum)");
  prof2->Draw();
  c66->SaveAs("/data/mike/ZZ48Scan/errorProfileSmoothUniv2Half.root");  

  TCanvas *c76 = new TCanvas("c76","c76");
  prof3->SetTitle("Universal Error vs Pad Position: 6 Pad Events; Position across Chevron (mm); Mean Residual (#mum)");
  prof3->Draw();
  c76->SaveAs("/data/mike/ZZ48Scan/errorProfileSmoothUniv3Half.root");  
 
  TCanvas *c78 = new TCanvas("c78","c78");
  profLocal->SetTitle("Local Error vs Board Position; Position across Chevron (mm); Mean Residual (#mum)");
  profLocal->Draw();
  c78->SaveAs("/data/mike/ZZ48Scan/errorProfileSmoothLocal.root");   

 TCanvas *c79 = new TCanvas("c79","c79");
  prof2->SetTitle("Universal Error vs Pad Position; Position across Chevron (mm); Mean Residual (#mum)");
  prof2->SetLineColor(4);
  // prof2->SetLineWidth(5);
  prof2->Draw();
  prof3->SetLineColor(2);
  // prof3->SetLineWidth(5);
  prof3->Draw("same");
  TLegend *legX = new TLegend(0.66,0.7,0.9,0.9);
  legX->SetFillColor(0);
  legX->AddEntry(prof2, "5 Pad Correction","pel");
  legX->AddEntry(prof3, "6 Pad Correction","pel");
  legX->Draw();
  c79->SaveAs("/data/mike/ZZ48Scan/errorProfileNPadOverlayedSmooth.root");  

  TCanvas *c47 = new TCanvas("c47","c47");
  globalAccUniv->SetTitle("Universal Error vs Pad Position; Position across Chevron (mm); Residual (#mum)");
  globalAccUniv->Draw();
  prof->SetLineColor(2);
  prof->SetLineWidth(5);
  prof->Draw("same");
  c47->SaveAs("/data/mike/ZZ48Scan/errorSpreadSmoothUnivHalf.root"); 
 
  TCanvas *c67 = new TCanvas("c67","c67");
  globalAccUniv2->SetTitle("Universal Error vs Pad Position: 5 Pad Events; Position across Chevron (mm); Residual (#mum)");
  globalAccUniv2->Draw();
  prof2->SetLineColor(2);
  prof2->SetLineWidth(5);
  prof2->Draw("same");
  c67->SaveAs("/data/mike/ZZ48Scan/errorSpreadSmoothUniv5Half.root");  

  TCanvas *c77 = new TCanvas("c77","c77");
  globalAccUniv3->SetTitle("Universal Error vs Pad Position: 6 Pad Events; Position across Chevron (mm); Residual (#mum)");
  globalAccUniv3->Draw();
  prof3->SetLineColor(2);
  prof3->SetLineWidth(5);
  
  prof3->Draw("same");
  c77->SaveAs("/data/mike/ZZ48Scan/errorSpreadSmoothUniv6Half.root");  

 

  TCanvas *c87 = new TCanvas("c87","c87");
  globalAcc->SetTitle("Local Error vs Board Position; Board Position (mm); Residual (#mum)");
  globalAcc->Draw();
  profLocal->SetLineColor(2);
  profLocal->SetLineWidth(5);
  profLocal->Draw("same");
  c87->SaveAs("/data/mike/ZZ48Scan/errorSpreadSmoothLocal.root");  


  /*
  ofstream file;
  file.open ("correlationConstants.txt");
  file << convConst << endl;
  file << offsetNew << endl;
  file.close();
  */
  /*
  TH1D *projY; TH1D *projY2; TH1D *projY3; 

  for (int i=0; i < globalAccUniv->GetXaxis()->GetLast()+1; ++i) {
    projY = globalAccUniv->ProjectionY("py", i, i);
    projY2 = globalAccUniv2->ProjectionY("py", i, i);
    projY3 = globalAccUniv3->ProjectionY("py", i, i);
    // cout << " entries " << projY->GetEntries() << endl;
     
      float centroid = globalAccUniv->GetXaxis()->GetBinCenter(i+1);
      int nBins = projY->GetSize() - 2;
      float span = projY->GetXaxis()->GetBinCenter(projY->GetSize() - 1);
      float binSize = span/nBins;
      float mean = projY->GetMean();
      float rms = projY->GetRMS();
      if (projY->GetEntries() > 5 && projY->GetRMS() > binSize) {
        float upperLim =  mean+TMath::Abs(.5*rms);
        float lowerLim = mean-TMath::Abs(.5*rms);
        fit = new TF1("fit", "gaus", 5*lowerLim, 5*upperLim);
        fit->SetParameters( (projY->GetEntries()/(2.35*projY->GetRMS() )), mean, rms); 
        fit->SetParLimits(1, mean-0.2, mean+0.2);  //***Very important***
        fit->SetParLimits(2, -3*rms, 3*rms);
        projY->Fit("fit", "Q0", "", lowerLim, upperLim); 
        mean = fit->GetParameter(1);  //unit of pads
        delete fit;
      }
      if (mean > 500 || mean < -500 ) mean = 0;
      errorFunction->Fill(centroid,mean);

      float mean2 = projY2->GetMean();
      float rms2 = projY2->GetRMS();
      if (projY2->GetEntries() > 5 && projY2->GetRMS() > binSize) {
        float upperLim =  mean2+TMath::Abs(.5*rms);
        float lowerLim = mean2-TMath::Abs(.5*rms);
        fit2 = new TF1("fit2", "gaus", 5*lowerLim, 5*upperLim);
        fit2->SetParameters( (projY2->GetEntries()/(2.35*projY2->GetRMS() )), mean2, rms2); 
        fit2->SetParLimits(1, mean2-0.2, mean2+0.2);  //***Very important***
        fit2->SetParLimits(2, -3*rms2, 3*rms2);
        projY2->Fit("fit2", "QO", "", lowerLim, upperLim); 
        mean2 = fit2->GetParameter(1);  //unit of pads
        delete fit2;
      }
      if (mean2 > 500 || mean2 < -500 ) mean2 = 0;
      errorFunction2->Fill(centroid,mean2);

      float mean3 = projY3->GetMean();
      float rms3 = projY3->GetRMS();
      if (projY3->GetEntries() > 5 && projY3->GetRMS() > binSize) {
        float upperLim =  mean3+TMath::Abs(.5*rms3);
        float lowerLim = mean3-TMath::Abs(.5*rms3);
        fit3 = new TF1("fit3", "gaus", 5*lowerLim, 5*upperLim);
        fit3->SetParameters( (projY3->GetEntries()/(2.35*projY3->GetRMS() )), mean3, rms3); 
        fit3->SetParLimits(1, mean3-0.2, mean3+0.2);  //***Very important***
        fit3->SetParLimits(2, -3*rms3, 3*rms3);
        projY3->Fit("fit3", "Q0", "", lowerLim, upperLim); 
        mean3 = fit3->GetParameter(1);  //unit of pads
        delete fit3;
      }
      if (mean3 > 500 || mean3 < -500 ) mean3 = 0;
      errorFunction3->Fill(centroid,mean3);

  }
  TCanvas *c55 = new TCanvas("c55","c55");
  errorFunction->SetMarkerStyle(20);
  errorFunction->SetMarkerColor(4);
  errorFunction->Draw("PL");
  c55->SaveAs("/data/mike/ZZ48Scan/errorFunctionSmoothUnivHalf.root");

  TCanvas *c65 = new TCanvas("c65","c65");
  errorFunction2->SetMarkerStyle(20);
  errorFunction2->SetMarkerColor(4);
  errorFunction2->Draw("PL");
  c65->SaveAs("/data/mike/ZZ48Scan/errorFunctionSmoothUniv5Half.root");

  TCanvas *c75 = new TCanvas("c75","c75");
  errorFunction3->SetMarkerStyle(20);
  errorFunction3->SetMarkerColor(4);
  errorFunction3->Draw("PL");
  c75->SaveAs("/data/mike/ZZ48Scan/errorFunctionSmoothUniv6Half.root");

  */
}

 


