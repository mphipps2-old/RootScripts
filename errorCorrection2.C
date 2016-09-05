// File takes error function, motor pos text file and cluster trees as input and returns residuals

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

#include "CorrectionFunction.C"


using namespace std;

void main() {  

  gROOT->SetBatch(kTRUE);

  Int_t nFiles = 20;
  Int_t singlePadEvents = 0;
  TFile* file = TFile::Open("/data/mike/ZZ48Scan/errorProfileSmoothUnivHalf.root");
  if (!file) return;
  TFile* file2 = TFile::Open("/data/mike/ZZ48Scan/errorProfileSmoothUniv2Half.root");
  if (!file2) return;
  TFile* file3 = TFile::Open("/data/mike/ZZ48Scan/errorProfileSmoothUniv3Half.root");
  if (!file3) return;
  //  TFile* file6 = TFile::Open("errorProfileUnsmoothLocal.root");
  //  if (!file6) return;
  /*
  TFile* file = TFile::Open("run388_qres_prof_corrected.root");
  if (!file) return;
  file->ls();
  TF1 *f1 = (TF1*) file->Get("f_mult_univ_pf");
  TF1 *f2 = (TF1*) file->Get("f_2pad_univ_pf");
  TF1 *f3 = (TF1*) file->Get("f_3pad_univ_pf");
  */
  //TProfile *profLocal = (TProfile*) c56->Get("globalAccuracy_pfx");
  // TProfile *prof = (TProfile*) file->Get("qx_gemerr_p3_univ_pfx");
  //  TProfile *prof2 = (TProfile*) file->Get("qx_gemerr_p3_2pad_univ_pfx");
  // TProfile *prof3 = (TProfile*) file->Get("qx_gemerr_p3_3pad_univ_pfx");
  // 
  // TCanvas *c56 = (TCanvas*) file->Get("c78");
  //TProfile *profLocal = (TProfile*) c56->GetPrimitive("globalError_pfx");
  
  TCanvas *c59 = (TCanvas*) file->Get("c56");
  TProfile *prof = (TProfile*) c59->GetPrimitive("globalErrorUniversal_pfx");
  TCanvas *c69 = (TCanvas*) file2->Get("c66");
  TProfile *prof2 = (TProfile*) c69->GetPrimitive("globalErrorUniversal2_pfx");
  TCanvas *c79 = (TCanvas*) file3->Get("c76");
  TProfile *prof3 = (TProfile*) c79->GetPrimitive("globalErrorUniversal3_pfx");

  /*
  TCanvas *c59 = (TCanvas*) file->Get("c55");
  TProfile *func = (TProfile*) c59->GetPrimitive("errorFunctionUniversal");
  TCanvas *c69 = (TCanvas*) file2->Get("c65");
  TProfile *func2 = (TProfile*) c69->GetPrimitive("errorFunctionUniversal2");
  TCanvas *c79 = (TCanvas*) file3->Get("c75");
  TProfile *func3 = (TProfile*) c79->GetPrimitive("errorFunctionUniversal3");
  */
  TH2F *globalAccUniv = new TH2F("globalAccuracyUniversal", "Universal Error of Clusters Vs Centroid Position", 100, 0, 1, 80,-1000,1000);
  TH2F *globalAccUniv2 = new TH2F("globalAccuracyUniversal2", "Universal Error of Clusters Vs Centroid Position", 100,0 , 1, 80,-1000,1000);
  TH2F *globalAccUniv3 = new TH2F("globalAccuracyUniversal3", "Universal Error of Clusters Vs Centroid Position", 100, 0, 1, 80,-1000,1000);

  float xPos;
  float yPos;
  int runNum;
  /*
  TH1F *globalResidualLocal= new TH1F("globalResidualLocal", "Global Residual Local Correction", 500, -2000,2000 );
  globalResidualLocal->GetXaxis()->SetTitle("Centroid Residual (um)");
  globalResidualLocal->GetYaxis()->SetTitle("Counts"); 
  */

  TH1F *globalResidual= new TH1F("globalResidualMulti", "Global Residual Universal Correction", 500, -2000,2000 );
  globalResidual->GetXaxis()->SetTitle("Centroid Residual (um)");
  globalResidual->GetYaxis()->SetTitle("Counts");

  TH1F *globalResidualUncor= new TH1F("globalResidualUncorrected", "Global Residual Uncorrected", 500, -2000,2000 );
  globalResidualUncor->GetXaxis()->SetTitle("Centroid Residual (um)");
  globalResidualUncor->GetYaxis()->SetTitle("Counts");

  TH1F *globalResidualCS= new TH1F("globalResidual", "Global Residual Universal N Pad Correction", 500, -2000,2000 );
  globalResidualCS->GetXaxis()->SetTitle("Centroid Residual (#mum)");
  globalResidualCS->GetYaxis()->SetTitle("Counts");
  float corrConst = 6.43629;
  float offset = -11.1918;
  /*
  ifstream file7("correlationConstants.txt"); 
  if(!file7.is_open()) cerr << " file not open" << endl;
  TString line2;
  int fileNum2 = 0;
  int count2 = 0; 
  while (line2.ReadLine(file7)) {
    // strip leading spaces and skip comments
    line2.Remove(TString::kBoth, ' ');
    // Array of tokens separated by "|"
    TObjArray *tokens = line2.Tokenize("|");
    TIter myIter(tokens);

    TObjString *st;
  
    while( st = (TObjString*) myIter.Next()) {        
      if (count2 == 0) corrConst =  (st->GetString().Remove(TString::kBoth,' ')).Atof();
      else if (count2 == 1) offset =  (st->GetString().Remove(TString::kBoth,' ')).Atof();
    }
    ++count2;
  }
  file7.close();
  */

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



    
    Float_t centerBin1Local; 
    Float_t bin1ContentLocal;
    Float_t centerBin0Local;
    Float_t bin0ContentLocal;
    Float_t centerBin2Local;
    Float_t bin2ContentLocal;
    Int_t flip;
    Float_t centerBin1; 
    Float_t bin1Content;
    Float_t centerBin0;
    Float_t bin0Content;
    Float_t centerBin2;
    Float_t bin2Content;

    Float_t centerBin12; 
    Float_t bin1Content2;
    Float_t centerBin02;
    Float_t bin0Content2;
    Float_t centerBin22;
    Float_t bin2Content2;

    Float_t centerBin13; 
    Float_t bin1Content3;
    Float_t centerBin03;
    Float_t bin0Content3;
    Float_t centerBin23;
    Float_t bin2Content3;
    Float_t correction;
    Float_t correctionCS;
    Float_t correctionLocal;

    Float_t m; 

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
        if (eEventNum != cEventNum) {
          cerr << "Cluster and event info mismatched: event " << i << " cluster " << cEventNum << " real event number " << eEventNum <<  endl;
          return -1;
	}
        if (cClusterCharge <= 2000 || cTimeLength <= 8) {
          ++cCount;
	  continue;
        }

        Float_t centX = cCentroid*2+1;
        Float_t centXCS  = cCentroid*2+1;
        Float_t tempCent = fmod(centX,2) - 1;
   
	
       
        if (tempCent < 0) {
          flip = -1; 
	  tempCent *= -1.0;
        }
        else flip = 1; 
	
        Float_t centXLocal = centX;
        Float_t uncorCent = centX;

	//   if  (cluster->GetClusterSizeX() == 2) cout << " cent " << tempCent << " bounded correction " << CorrectionFunction(prof2, tempCent, 0.2) << " unbounded correction " <<  prof2->GetBinContent(prof2->GetXaxis()->FindBin(tempCent)) << endl;
	
        centerBin1 = prof->GetBinCenter(prof->GetXaxis()->FindBin(tempCent));
        bin1Content = prof->GetBinContent(prof->GetXaxis()->FindBin(tempCent));
        centerBin0 = prof->GetBinCenter(prof->GetXaxis()->FindBin(tempCent)-1);
        bin0Content = prof->GetBinContent(prof->GetXaxis()->FindBin(tempCent)-1);
        centerBin2 = prof->GetBinCenter(prof->GetXaxis()->FindBin(tempCent)+1);
        bin2Content = prof->GetBinContent(prof->GetXaxis()->FindBin(tempCent)+1);

        centerBin12 = prof2->GetBinCenter(prof2->GetXaxis()->FindBin(tempCent));
        bin1Content2 = prof2->GetBinContent(prof2->GetXaxis()->FindBin(tempCent));
        centerBin02 = prof2->GetBinCenter(prof2->GetXaxis()->FindBin(tempCent)-1);
        bin0Content2 = prof2->GetBinContent(prof2->GetXaxis()->FindBin(tempCent)-1);
        centerBin22 = prof2->GetBinCenter(prof2->GetXaxis()->FindBin(tempCent)+1);
        bin2Content2 = prof2->GetBinContent(prof2->GetXaxis()->FindBin(tempCent)+1);

        centerBin13 = prof3->GetBinCenter(prof3->GetXaxis()->FindBin(tempCent));
        bin1Content3 = prof3->GetBinContent(prof3->GetXaxis()->FindBin(tempCent));
        centerBin03 = prof3->GetBinCenter(prof3->GetXaxis()->FindBin(tempCent)-1);
        bin0Content3 = prof3->GetBinContent(prof3->GetXaxis()->FindBin(tempCent)-1);
        centerBin23 = prof3->GetBinCenter(prof3->GetXaxis()->FindBin(tempCent)+1);
        bin2Content3 = prof3->GetBinContent(prof3->GetXaxis()->FindBin(tempCent)+1);

	
	/*

        centerBin1Local = profLocal->GetBinCenter(profLocal->GetXaxis()->FindBin(centX));
        bin1ContentLocal = profLocal->GetBinContent(profLocal->GetXaxis()->FindBin(centX));
        centerBin0Local = profLocal->GetBinCenter((profLocal->GetXaxis()->FindBin(centX))-1);

        centerBin2Local = profLocal->GetBinCenter(profLocal->GetXaxis()->FindBin(centX)+1);
        bin2ContentLocal = profLocal->GetBinContent(profLocal->GetXaxis()->FindBin(centX)+1);
        bin0ContentLocal = profLocal->GetBinContent((profLocal->GetXaxis()->FindBin(centX))-1);
	*/
        if (tempCent < centerBin1) {
          m = (bin1Content - bin0Content) / (centerBin1 - centerBin0);
          correction = m*(tempCent-centerBin0) + bin0Content;
        }
        else {
          m = (bin2Content - bin1Content) / (centerBin2 - centerBin1);
          correction = m*(tempCent-centerBin1) + bin1Content;
        }

        if (tempCent < centerBin1 && cClusterSize == 5) {
          m = (bin1Content2 - bin0Content2) / (centerBin12 - centerBin02);
          correctionCS = m*(tempCent-centerBin02) + bin0Content2;
        }
        else if (tempCent > centerBin1 && cClusterSize == 5){
          m = (bin1Content2 - bin2Content2) / (centerBin12 - centerBin22);
          correctionCS = m*(tempCent-centerBin22) + bin2Content2;
        }
        else if (tempCent < centerBin1 && cClusterSize == 6) {
          m = (bin1Content3 - bin0Content3) / (centerBin13 - centerBin03);
          correctionCS = m*(tempCent-centerBin03) + bin0Content3;
        }
        else if  (tempCent > centerBin1 && cClusterSize == 6) {
          m = (bin1Content3 - bin2Content3) / (centerBin13 - centerBin2);
          correctionCS = m*(tempCent-centerBin23) + bin2Content3;
        }
	  
	/*
	
        if (centX < centerBin1Local) {
          m = (bin1ContentLocal - bin0ContentLocal) / (centerBin1Local - centerBin0Local);
          correctionLocal = m*(centX-centerBin0Local) + bin0ContentLocal;
        }
        else {
          m = (bin2ContentLocal - bin1ContentLocal) / (centerBin2Local - centerBin1Local);
	  correctionLocal = m*(centX-centerBin1Local) + bin1ContentLocal;
        }
	
       	*/
  
	//   correction = f1->GetParameter(0) + (f1->GetParameter(1) * tempCent)+(f1->GetParameter(2)*TMath::Power(tempCent,2)) + (f1->GetParameter(3)*TMath::Power(tempCent,3));
	//  if (cluster->GetClusterSizeX() == 2){ correctionCS = f2->GetParameter(0) + (f2->GetParameter(1)*tempCent)+(f2->GetParameter(2)*TMath::Power(tempCent,2)) + (f2->GetParameter(3)*TMath::Power(tempCent,3)); }
	//  else if (cluster->GetClusterSizeX() == 3) correctionCS = f3->GetParameter(0) + (f3->GetParameter(1)*tempCent)+(f3->GetParameter(2)*TMath::Power(tempCent,2)) + (f3->GetParameter(3)*TMath::Power(tempCent,3));
	//	cout << " param 0 " << f1->GetParameter(0) << " p1 " << f1->GetParameter(1) << " p2 " << f1->GetParameter(2) << " p3 " << f1->GetParameter(3) << " tempCent " << tempCent << endl;

	if (flip == -1) {correction *= -1; correctionCS *= -1;}
	//	correction += 20;
	//	correctionCS+=20;
	//		cout << "correction " << correction << " correction 2 " << correctionCS <<   " cent " << centX << endl;
        //centXLocal -= correctionLocal/1000;
	//	  centX -= (correction/1000);
	//  centXCS -= (correctionCS/1000);
		centX -= (correction/1000);
		//centX -= (correction);
	 	  centXCS -= (correctionCS/1000);     
		//centXCS -= (correctionCS);     
        if (centX == centX && centX != 0 ) {
	  centX *= 1000;
	  centXCS *= 1000;
          uncorCent *= 1000;
          //centXLocal *= 1000;
	  // if (cluster->GetClusterSizeX() != 1 ) {
	  //            globalResidualUncor->Fill(uncorCent - (xPos*corrConst)+15+(offset*1000) );
 globalResidualUncor->Fill(uncorCent - (((xPos*(-6.09219948))+100000))+ 5278+123.5 );
 //	    globalResidual->Fill(centX - (xPos*corrConst)+15+(offset*1000));
   globalResidual->Fill(centX - (((xPos*(-6.09219948))+100000))+ 5278+123.5);
   //   cout << " residual " << centX - (((xPos*(-6.09219948))+100000))+ 5278+123.5 << " centx " << centX << " motor " << (((xPos*(-6.09219948))+100000))+ 5278+123.5 << endl;
	    globalResidualCS->Fill(centXCS - (((xPos*(-6.09219948))+100000))+ 5278+123.5);
	    //	    globalResidualCS->Fill(centXCS - (xPos*corrConst) + 15+(offset*1000));
	    // globalResidualLocal->Fill(centXLocal-(xPos*corrConst) + offset*1000);

            float error = centX-(((xPos*(-6.09219948))+100000))+ 5278+123.5;
            float position = fmod((centX/1000),2)-1;
	    if (position < 0) error *= -1;
            if (cClusterSize == 1) singlePadEvents++;
            globalAccUniv->Fill(TMath::Abs(position),error);
	    if (cClusterSize == 5) globalAccUniv2->Fill(TMath::Abs(position), error);
	    if (cClusterSize == 6) globalAccUniv3->Fill(TMath::Abs(position), error);
	    //  }
        }
        ++cCount;
      }
    }
  }
  globalAccUniv->Smooth(1);
  globalAccUniv2->Smooth(1);
  globalAccUniv3->Smooth(1);

  cout << " Single Pad Events " << singlePadEvents << endl;
  TCanvas *c48 = new TCanvas("c48","c48");
  globalAccUniv->SetTitle("Universal Error of Clusters vs Pad Position; Position across Chevron (mm); Residual (#mum)");
  globalAccUniv->Draw();
  c48->SaveAs("/data/mike/ZZ48Scan/ErrorSpreadCorrSmoothUnivProfHalf.root"); 
 
  TCanvas *c58 = new TCanvas("c58","c58");
  globalAccUniv2->SetTitle("Universal Error of Clusters vs Pad Position: 5 Pad Events; Position across Chevron (mm); Residual (#mum)");
  globalAccUniv2->Draw();
  c58->SaveAs("/data/mike/ZZ48Scan/ErrorSpreadCorrSmoothUniv5ProfHalf.root");  

  TCanvas *c68 = new TCanvas("c68","c68");
  globalAccUniv3->SetTitle("Universal Error of Clusters vs Pad Position: 6 Pad Events; Position across Chevron (mm); Residual (#mum)");
  globalAccUniv3->Draw();
  c68->SaveAs("/data/mike/ZZ48Scan/ErrorSpreadCorrSmoothUniv6ProfHalf.root");  

  TCanvas *c60 = new TCanvas("c60","c60");
  globalResidualCS->Draw();
  c60->SaveAs("/data/mike/ZZ48Scan/GlobalResidualSmoothCorrectedUnivClusterSizeProfHalfAllEvents.root");

    TCanvas *c50 = new TCanvas("c50","c50");
  globalResidual->Draw();
  c50->SaveAs("/data/mike/ZZ48Scan/GlobalResidualSmoothCorrectedUnivProfHalfAllEvents.root");

    TCanvas *c80 = new TCanvas("c80","c80");
  globalResidualUncor->Draw();
  c80->SaveAs("/data/mike/ZZ48Scan/GlobalResidualUncorAllEvents.root");

  // TCanvas *c70 = new TCanvas("c70","c70");
  // globalResidualLocal->Draw();
  // c70->SaveAs("globalResidualCorrectedLocal.root");

  delete c50;
 delete c60; 
 //   delete c70;
   delete c80;
}

 


