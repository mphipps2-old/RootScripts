// This is the second script in the sequence. It takes the pmonitor ttrees as input and then stitches all individual files together and outputs a series of analysis and diagnostic plots.

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

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
#include "TLegend.h"
#include "TGraph.h"
#include "TSystem.h"

using namespace std;


int main()
{

  gROOT->SetBatch(kTRUE);
  //gSystem->Load("libAnalysis.so");
 char baseDir[150] = "/data/mike/ZZ48Scan";
 char treeDir[150] = "/data/Trees/ZZ48";
 //char baseName[50] = "FineChevron5Sigma";
  const int totalFiles = 100;
  //int numEvents = 5000;
  // Float_t singleRes = 2000/TMath::Sqrt(12); 
  //  float initialPos;
  Float_t errorVec[161];
  Float_t centroidVec[161];
  TH1F *h_corr = new TH1F("corr", "Mean Centroid Vs Motor Position", 730, 0, 70);
  h_corr->GetXaxis()->SetTitle("Motor Position (mm)");
  h_corr->GetYaxis()->SetTitle("Centroid dist. Mean (mm)");
  
  TH1F *errorCorrected = new TH1F("error", "Accuracy of Clusters Vs  Motor Position after Error Correction", 700, 0, 70);
  errorCorrected->GetXaxis()->SetTitle("Motor Position (mm)");
  errorCorrected->GetYaxis()->SetTitle("Centroid dist. Mean minus Motor Postion (um)");

  
  TH1F *localResMulti = new TH1F("localResMulti", "Resolution Vs Motor Position", 700, 0, 70);
  localResMulti->GetXaxis()->SetTitle("Motor Position (mm)");
  localResMulti->GetYaxis()->SetTitle("Centroid dist. Sigma (um)");

  TH1F *localAcc = new TH1F("localAccuracy", "Accuracy of Clusters Vs Motor Position", 700, 0, 70);
  localAcc->GetXaxis()->SetTitle("Motor Position (mm)");
  localAcc->GetYaxis()->SetTitle("Centroid Mean minus Motor Position (um)");

  TH2F *globalAcc = new TH2F("globalAccuracy", "Error of Clusters Vs Centroid Position", 300, 30.5, 60.5, 80,-1000,1000);
  globalAcc->GetXaxis()->SetTitle("Centroid (mm)");
  globalAcc->GetYaxis()->SetTitle("Centroid minus Motor Position (um)");

  TH2F *globalAccUniv = new TH2F("globalAccuracyUniversal", "Universal Error of Clusters Vs Centroid Position", 200, -1, 1, 80,-1000,1000);
  globalAccUniv->GetXaxis()->SetTitle("Mod 2 Centroid (mm)");
  globalAccUniv->GetYaxis()->SetTitle("Centroid minus Motor Position (um)");

  TH2F *globalAccUniv5 = new TH2F("globalAccuracyUniversal5", "Universal Error of Clusters Vs Centroid Position", 200, -1, 1, 80,-1000,1000);
  globalAccUniv5->GetXaxis()->SetTitle("Mod 2 Centroid (mm)");
  globalAccUniv5->GetYaxis()->SetTitle("Centroid minus Motor Position (um)");

  TH2F *globalAccUniv6 = new TH2F("globalAccuracyUniversal6", "Universal Error of Clusters Vs Centroid Position", 200, -1, 1, 80,-1000,1000);
  globalAccUniv6->GetXaxis()->SetTitle("Mod 2 Centroid (mm)");
  globalAccUniv6->GetYaxis()->SetTitle("Centroid minus Motor Position (um)");

  TH2F *globalCorrelation = new TH2F("globalCorrelation", "Centroid vs. Motor Position: Corrected Constant", 300, 20.5, 55.5, 150,25.5,60.5);
  globalCorrelation->GetXaxis()->SetTitle("Centroid (mm)");
  globalCorrelation->GetYaxis()->SetTitle("Motor Position (mm)");

  TH2F *globalCorrelationOld = new TH2F("globalCorrelationOld", "Centroid vs. Motor Position: Original Constant", 1500, 25.5, 55.5,300,4,9);
  globalCorrelationOld->GetXaxis()->SetTitle("Centroid (mm)");
  globalCorrelationOld->GetYaxis()->SetTitle("Motor Position (mm)");

  TH2F *globalAccMotor = new TH2F("globalAccuracyMotor", "Accuracy of Clusters Vs Centroid Position", 600, 0, 70, 40,-500,500);
  globalAccMotor->GetXaxis()->SetTitle("Motor (mm)");
  globalAccMotor->GetYaxis()->SetTitle("Centroid Mean minus Motor Position (um)");

  TH2F *globalAccCorr = new TH2F("globalAccuracyCorr", "Corrected Accuracy of Clusters Vs Centroid Position", 600, 0, 70, 100,-50,50);
  globalAccCorr->GetXaxis()->SetTitle("Centroid (mm)");
  globalAccCorr->GetYaxis()->SetTitle("Centroid minus Motor Position (um)");

  TH1F *globalCentroid = new TH1F("globalCentroid", "Global Centroid", 500, -2000,2000 );
  globalCentroid->GetXaxis()->SetTitle("Centroid [per event] (um)");
  globalCentroid->GetYaxis()->SetTitle("Counts");
  
  TH1F *globalCentroidMulti = new TH1F("globalCentroidMulti", "Global Residual Multi Clusters", 500, -2000,2000 );
  globalCentroidMulti->GetXaxis()->SetTitle("Centroid Residual (um)");
  globalCentroidMulti->GetYaxis()->SetTitle("Counts");

  TH1F *globalCentroidMultiCorrected = new TH1F("globalCentroidMultiCorrected", "Global Centroid Multi Clusters with Error Correction", 500, -2000,2000 );
  globalCentroidMultiCorrected->GetXaxis()->SetTitle("Centroid [per event] (um)");
  globalCentroidMultiCorrected->GetYaxis()->SetTitle("Counts");

    TH1F *globalCentroidIdeal = new TH1F("globalCentroidIdeal", "Global Centroid", 500, -2000,2000 );
  globalCentroidIdeal->GetXaxis()->SetTitle("Centroid [per event] (um)");
  globalCentroidIdeal->GetYaxis()->SetTitle("Counts");

  TH1F *globalCentroidIdealMulti = new TH1F("globalCentroidIdealMulti", "Global Centroid Multi Clusters", 500, -2000,2000 );
  globalCentroidIdealMulti->GetXaxis()->SetTitle("Centroid [per event] (um)");
  globalCentroidIdealMulti->GetYaxis()->SetTitle("Counts");

  TH1F *globalStitch = new TH1F("globalCentroidStitch", "Global Centroid Stitch",1076, 20, 74 );
  globalStitch->GetXaxis()->SetTitle("Centroid [per event] (um)");
  globalStitch->GetYaxis()->SetTitle("Counts");

  TH1F *globalStitchCorrected = new TH1F("globalCentroidStitchCorrected", "Global Centroid Stitch with Error Correction",1600, 0,45000 );
  globalStitchCorrected->GetXaxis()->SetTitle("Centroid [per event] (um)");
  globalStitchCorrected->GetYaxis()->SetTitle("Counts");

   TH1F *globalClusterSize1Motor = new TH1F("globalClusterSize1Motor", "Global Cluster Size",1076, 20, 74 );
  globalClusterSize1Motor->GetXaxis()->SetTitle("Motor Position (mm)");
  globalClusterSize1Motor->GetYaxis()->SetTitle("Fractional Number of Events");

   TH1F *globalClusterSize5Motor = new TH1F("globalClusterSize5Motor", "Global Cluster Size",1076, 20, 74 );
  globalClusterSize5Motor->GetXaxis()->SetTitle("Motor Position (mm)");
  globalClusterSize5Motor->GetYaxis()->SetTitle("Fractional Number of Events");

   TH1F *globalClusterSize6Motor = new TH1F("globalClusterSize6Motor", "Global Cluster Size",1076, 20, 74 );
  globalClusterSize6Motor->GetXaxis()->SetTitle("Motor Position (mm)");
  globalClusterSize6Motor->GetYaxis()->SetTitle("Fractional Number of Events");

   TH1F *globalClusterSize1Cent = new TH1F("globalClusterSize1Cent", "Global Cluster Size",3200, 0,90 );
  globalClusterSize1Cent->GetXaxis()->SetTitle("Centroid [per event] (um)");
  globalClusterSize1Cent->GetYaxis()->SetTitle("Fractional Number of Events");

   TH1F *globalClusterSize5Cent = new TH1F("globalClusterSize5Cent", "Global Cluster Size",3200, 0,90 );
  globalClusterSize5Cent->GetXaxis()->SetTitle("Centroid [per event] (um)");
  globalClusterSize5Cent->GetYaxis()->SetTitle("Fractional Number of Events");

   TH1F *globalClusterSize6Cent = new TH1F("globalClusterSize6Cent", "Global Cluster Size",3200, 0,90 );
  globalClusterSize6Cent->GetXaxis()->SetTitle("Centroid [per event] (um)");
  globalClusterSize6Cent->GetYaxis()->SetTitle("Fractional Number of Events");

   TH1F *globalChargeSharing1Motor = new TH1F("globalChargeSharing1Motor", "Global Charge Sharing",1076, 20, 74);
  globalChargeSharing1Motor->GetXaxis()->SetTitle("Motor Position (mm)");
  globalChargeSharing1Motor->GetYaxis()->SetTitle("Fraction of Total Charge on Primary Pad");

   TH1F *globalChargeSharing5Motor = new TH1F("globalChargeSharing5Motor", "Global Charge Sharing",1076, 20, 74 );
  globalChargeSharing5Motor->GetXaxis()->SetTitle("Motor Position (mm)");
  globalChargeSharing5Motor->GetYaxis()->SetTitle("Fraction of Total Charge on Primary Pad");

   TH1F *globalChargeSharing6Motor = new TH1F("globalChargeSharingMotor6", "Global Charge Sharing",1076, 20, 74 );
  globalChargeSharing6Motor->GetXaxis()->SetTitle("Motor Position (mm)");
  globalChargeSharing6Motor->GetYaxis()->SetTitle("Fraction of Total Charge on Primary Pad");

   TH1F *globalChargeSharing1Cent = new TH1F("globalChargeSharing1Cent", "Global Charge Sharing",3200, 0,90000 );
  globalChargeSharing1Cent->GetXaxis()->SetTitle("Centroid [per event] (um)");
  globalChargeSharing1Cent->GetYaxis()->SetTitle("Fraction of Total Charge on Primary Pad");

   TH1F *globalChargeSharing5Cent = new TH1F("globalChargeSharing5Cent", "Global Charge Sharing",3200, 0,90000 );
  globalChargeSharing5Cent->GetXaxis()->SetTitle("Centroid [per event] (um)");
  globalChargeSharing5Cent->GetYaxis()->SetTitle("Fraction of Total Charge on Primary Pad");

   TH1F *globalChargeSharing6Cent = new TH1F("globalChargeSharingCent6", "Global Charge Sharing",3200, 0,90000 );
  globalChargeSharing6Cent->GetXaxis()->SetTitle("Centroid [per event] (um)");
  globalChargeSharing6Cent->GetYaxis()->SetTitle("Fraction of Total Charge on Primary Pad");

  TH1F *globalAvgClusterSize = new TH1F("globalAvgClusterSize", "Global Average Cluster Size",1076, 20, 74 );
  globalAvgClusterSize->GetXaxis()->SetTitle("Motor Position (mm)");
  globalAvgClusterSize->GetYaxis()->SetTitle("Average Cluster Size");

  ifstream file("postest_run_80000.txt"); 
  if(!file.is_open()) cerr << " file not open" << endl;
  TString line;
  int nFiles = 0;
  while (line.ReadLine(file) && nFiles < totalFiles) {
    ++nFiles;
    gROOT->cd(0);
    // strip leading spaces and skip comments
    line.Remove(TString::kBoth, ' ');
    // Array of tokens separated by "|"
    TObjArray *tokens = line.Tokenize("|");
    TIter myIter(tokens);
    int count = 0;
    float xPos = 0;
    float yPos = 0;
    int runNum = 0;
    TH1F *localCentroid = new TH1F("localCentroid", "Local Centroid", 3200, 0,90000 );
    localCentroid->GetXaxis()->SetTitle("Centroid [per event] (um)");
    localCentroid->GetYaxis()->SetTitle("Counts");

    TH1F *localCentroidSingle = new TH1F("localCentroidSingle", "Local Centroid Single Pad", 3200, 0,90000 );
    localCentroidSingle->GetXaxis()->SetTitle("Centroid [per event] (um)");
    localCentroidSingle->GetYaxis()->SetTitle("Counts");

    TH1F *localCentroidMulti = new TH1F("localCentroidMulti", "Local Centroid Multi Pad", 3200, 0,90000 );
    localCentroidMulti->GetXaxis()->SetTitle("Centroid [per event] (um)");
    localCentroidMulti->GetYaxis()->SetTitle("Counts");
    

    
    TH1F *localCentroidMultiCorr = new TH1F("localCentroidMultiCorr", "Local Centroid Multi Pad Corrected", 3200, 0,90000 );
    localCentroidMultiCorr->GetXaxis()->SetTitle("Centroid [per event] (um)");
    localCentroidMultiCorr->GetYaxis()->SetTitle("Counts");

    TH1F *localClusterSizeX = new TH1F("localClusterSizeX", "Cluster Size X", 10, -.5, 9.5);
    localClusterSizeX->SetXTitle("Number of Hits in X Clusters");
    localClusterSizeX->SetYTitle("Frequency");
    localClusterSizeX->GetYaxis()->SetTitleOffset(1.4);

    TH1F *localChargeSpectrum = new TH1F("localChargeSpectrum","Charge Spectrum", 4000, 0, 25000);
    localChargeSpectrum->SetXTitle("Total Cluster Charge [ADC Counts]");
    localChargeSpectrum->SetYTitle("Frequency");
    localChargeSpectrum->GetYaxis()->SetTitleOffset(1.4);


    TH1F *localCentroidTest = new TH1F("localCentroidTest", "Local Centroid Multi Pad Test", 3200, 0,90000 );
    localCentroidTest->GetXaxis()->SetTitle("Centroid [per event] (um)");
    localCentroidTest->GetYaxis()->SetTitle("Counts");

    vector<Float_t> avgChSh2;
    vector<Float_t> avgChSh3;


    TObjString *st;
    while( st = (TObjString*) myIter.Next()) {        
      if (count == 0) xPos =  (st->GetString().Remove(TString::kBoth,' ')).Atof();
      else if (count == 1) yPos =  (st->GetString().Remove(TString::kBoth,' ')).Atof();
      else if (count == 2) runNum =  (st->GetString().Remove(TString::kBoth,' ')).Atoi();
      ++count;
    }
    cout<<"========= RunNum: "<<runNum<<", xMotor: "<<xPos<<", yMotor: "<<yPos<<" ========"<<endl;

    char treeFile[150];
    sprintf(treeFile,"%s/TreeFITZZ48Scan1Run%d.root",treeDir,runNum);
    
    TFile *f = TFile::Open(treeFile);
    if (!f) return -1;
    TTree *event = (TTree*) f->Get("Event;1");
    TTree *cluster = (TTree*) f->Get("Cluster;1");
    TTree *hit = (TTree*) f->Get("Hit;1");



    Int_t nEvents = (Int_t) event->GetEntries();
    Int_t cCount = 0;

    
    Int_t cEventNum = 0;
    Int_t eEventNum = 0;
     
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
    //cout << " nevents " << nEvents << " ncluster events " << (Int_t) cluster->GetEntries() << endl;
    for (int i=0; i < nEvents; ++i) {
      event->GetEntry(i);
      if (eClusterMult > 10 ) { 
        cCount+=eClusterMult;
	cout << " skipping event " << i << " cluster mult " << eClusterMult << endl;
        continue;
      }
      // if you cut event, add cluster mult to cCount
      for (int j=0; j < eClusterMult; ++j) {
        cluster->GetEntry(cCount);
        cCentroid -= 336;

	if (cCentroid < 0 || cCentroid > 48) {++cCount; continue;}
	//	cout << " cCentroid " << cCentroid << endl;
	//if (i==4860 || i ==4861 || i ==4862 || i ==4863)cout << " event " << i << " cluster " << cEventNum << " real event number " << eEventNum <<  endl;
	//int check; 
	//if (i==4864 && j == 0) 
	//	int check; cin >> check;
	
	
        if (eEventNum != cEventNum) {
          //--cCount; // sometimes become out of synch if cluster size cut from analysis comes into play
	 	  cerr << "Cluster and event info mismatched: event " << i << " cluster " << cEventNum << " real event number ... continuing to next cluster " << eEventNum <<  endl;
          continue;
	  // return -1;
	}
	//++cCount;
	//continue;
        if (cClusterCharge < 2000 /*cPeakCharge <= 200*/ || cTimeLength <= 8 /* || cluster->GetYCentroid() != 3*/) {
          ++cCount;
	  //	 	  cout << " continuinggggggg charge " << cPeakCharge << " length " << cTimeLength <<  endl;
	  continue;
        }
        Float_t xCentroid = (cCentroid*2000)+1000;
        if (xCentroid == xCentroid && xCentroid != 0 ) {

          localCentroid->Fill(xCentroid);
          if (cClusterSize > 1) {
            localCentroidMulti->Fill(xCentroid);
	    //	    cout << " xcentroid " << xCentroid << " test value  " << xCentroid - (39847.6 - (((xPos*(-6.09219948))+100000))+ 5278) << endl;
            

	  }
	  if (cClusterSize == 1) localCentroidSingle->Fill(xCentroid);
	  //if (cluster->GetClusterSizeX() > 1) localCentroidTest->Fill(xCentroid - (39847.6 - (((xPos*(-6.09219948))+100000))+ 5278));
	  //          localChargeSpectrum->Fill(cClusterCharge);
          localClusterSizeX->Fill(cClusterSize);
        }
        ++cCount;
      }
    }
    //return -1;

    float avg = localCentroidMulti->GetMean();
    float rms = localCentroidMulti->GetRMS();
    localCentroidMulti->SetAxisRange(avg-(0.6*rms),avg+(0.6*rms),"X");
    avg = localCentroidMulti->GetMean();
    rms = localCentroidMulti->GetRMS();
    localCentroidMulti->SetAxisRange(avg-(0.6*rms),avg+(0.6*rms),"X");
    avg = localCentroidMulti->GetMean();
    rms = localCentroidMulti->GetRMS();
    //cout << " avg " << avg << " rms " << rms << endl;
      /*  if (runNum < 80040)*/    localCentroidMulti->Fit("gaus", "Q0", "", avg-rms, avg+rms);
      //    else    localCentroidMulti->Fit("gaus", "Q0", "", avg-(2*rms), avg+(2*rms));
    //  Int_t checkFit = resultPtr;
    
    TF1 *fit = localCentroidMulti->GetFunction("gaus");
    float mean = fit->GetParameter(1);  //unit of pads
    float sigma = fit->GetParameter(2);  //unit of pads
    cout << " runNum " << runNum << " mean " << mean << endl;

    //TCanvas *c1000 = new TCanvas("c1000","c1000"); localCentroidMulti->Draw();   char file160[150]; sprintf(file160,"%s/mean%d.root",baseDir,runNum); c1000->SaveAs(file160,"q");  delete c1000;
    Int_t hCount = 0;
    Int_t cCount2 = 0;
   
   
    for (int i=0; i < nEvents; ++i) {
      event->GetEntry(i);
      if (eClusterMult > 10 ) { 
        cCount2+=eClusterMult;
        continue;
      }
      for (int j=0; j < eClusterMult; ++j) {
        hCount += eClusterMult;
       	cluster->GetEntry(cCount2);
        cCentroid -= 336;
	if (cCentroid < 0 || cCentroid > 48) {++cCount2; continue; }
        if (eEventNum != cEventNum) {
	  cerr << "2nd pass: Cluster and event info mismatched. EventNum " << eEventNum << " cluster event " << cEventNum << " ccount " << cCount2 << endl;
	  //    return -1;
	  continue;
	}
        
        if (/*cPeakCharge <= 200*/ cClusterCharge < 2000 || cTimeLength <= 8 /*|| cluster->GetYCentroid() != 3 */) {
            ++cCount2;
	    continue;
	 
        }
        Float_t xCentroid = (cCentroid*2000) + 1000;

        if (xCentroid == xCentroid && xCentroid !=0) {
          globalCentroid->Fill(xCentroid - (((xPos*(-6.09219948))+100000)) + 5401);
          globalCentroidIdeal->Fill(xCentroid - mean);
	  //	  cout << " ideal " << xCentroid - mean << " nonideal " << xCentroid - (((xPos*(-6.09219948))+100000)) + 5278 << endl;
	  // globalStitch->Fill(xCentroid);
	  //   if (xCentroid < 27.73 && xCentroid < 27.77) cout << " xCentroid " << xCentroid << endl;

          if (cClusterSize == 1) {
            globalClusterSize1Cent->Fill(xCentroid);
            globalChargeSharing1Cent->Fill(xCentroid,1);
	  
          }
          if (cClusterSize == 5) {
	    // Note: come back and fix: Match ClusterNo with individual hits
	    hit->GetEntry(hCount-4); 
            Float_t charge0 =  hAvgCharge;
	    hit->GetEntry(hCount-3);
            Float_t charge1 =  hAvgCharge;
	    hit->GetEntry(hCount-2);
	    Float_t charge2 =  hAvgCharge;
            hit->GetEntry(hCount-1);
            Float_t charge3 =  hAvgCharge;
	    hit->GetEntry(hCount);
            Float_t charge4 =  hAvgCharge;
            Float_t maxCharge; Float_t otherCharge;
            
	    if (charge0 > charge1  && charge0 > charge2 && charge0 > charge3 && charge0 > charge4) {maxCharge=charge0; otherCharge=charge1+charge2+charge3+charge4;}
            else if (charge1 > charge0  && charge1 > charge2 && charge1 > charge3  && charge1 > charge4) {maxCharge=charge1; otherCharge=charge0+charge2+charge3+charge4;}
            else if (charge2 > charge0  && charge2 > charge1 && charge2 > charge3  && charge2 > charge4) {maxCharge=charge2; otherCharge=charge0+charge2+charge3+charge4;}
            else if (charge3 > charge0  && charge3 > charge1 && charge3 > charge2  && charge3 > charge4) {maxCharge=charge3; otherCharge=charge0+charge2+charge3+charge4;}
            else {maxCharge=charge4; otherCharge=charge0+charge1+charge2+charge3;}

            Float_t chSh = maxCharge / (maxCharge+otherCharge);
	    // cout << " chSh " << chSh << " maxCh " << maxCharge << " other " << otherCharge << endl;
            avgChSh2.push_back(chSh);
	    globalChargeSharing5Cent->Fill(xCentroid,chSh);
	    globalClusterSize5Cent->Fill(xCentroid/1000);
	    // cout << " filled 2 " << endl;
	    //  if (xCentroid < 27.73 && xCentroid < 27.77) cout << " cluster size 2 " << endl;
          }
          if (cClusterSize== 6) {
	    //  cout << " filling 3 " << endl;
	    hit->GetEntry(hCount-5); 
            Float_t charge0 =  hAvgCharge;
	    hit->GetEntry(hCount-4);
            Float_t charge1 =  hAvgCharge;
	    hit->GetEntry(hCount-3);
	    Float_t charge2 =  hAvgCharge;
            hit->GetEntry(hCount-2);
            Float_t charge3 =  hAvgCharge;
	    hit->GetEntry(hCount-1);
            Float_t charge4 =  hAvgCharge;
	    hit->GetEntry(hCount);
	    Float_t charge5 =  hAvgCharge;
            Float_t maxCharge; Float_t otherCharge;
            
	    if (charge0 > charge1  && charge0 > charge2 && charge0 > charge3 && charge0 > charge4 && charge0 > charge5) {maxCharge=charge0; otherCharge=charge1+charge2+charge3+charge4+charge5;}
            else if (charge1 > charge0  && charge1 > charge2 && charge1 > charge3  && charge1 > charge4 && charge1 > charge5) {maxCharge=charge1; otherCharge=charge0+charge2+charge3+charge4+charge5;}
            else if (charge2 > charge0  && charge2 > charge1 && charge2 > charge3  && charge2 > charge4 && charge2 > charge5) {maxCharge=charge2; otherCharge=charge0+charge2+charge3+charge4+charge5;}
            else if (charge3 > charge0  && charge3 > charge1 && charge3 > charge2  && charge3 > charge4 && charge3 > charge5) {maxCharge=charge3; otherCharge=charge0+charge2+charge3+charge4+charge5;}
            else if (charge4 > charge0  && charge4 > charge1 && charge4 > charge2  && charge4 > charge3 && charge4 > charge5) {maxCharge=charge4; otherCharge=charge0+charge2+charge3+charge4+charge5;}
            else {maxCharge=charge5; otherCharge=charge0+charge1+charge2+charge3+charge4;}
            Float_t chSh = maxCharge / (maxCharge+otherCharge);
            avgChSh3.push_back(chSh);
            globalChargeSharing6Cent->Fill(xCentroid,chSh);
            globalClusterSize6Cent->Fill(xCentroid/1000);
	    //     cout << " filled 3 " << endl;
	    //  if (xCentroid < 27.73 && xCentroid < 27.77)  cout << " cluster size 3 " << endl;
          }
          if (cClusterSize > 1) {

            globalCentroidMulti->Fill(xCentroid - (((xPos*(-6.09219948))+100000)) + 5401);
            globalCentroidIdealMulti->Fill(xCentroid - mean);
        
          
	    //    cout << " filled old " << endl;
          }
        }
        ++cCount2;
      }
    }
      // cuts to consider: cluster multiplicity, max cluster hit (>1000) cluster size time (>10)
    cout << " xpos*6.36/1000 " << ((xPos*(-6.09219948))/1000)+100+5.401 << " mean " << mean/1000 << endl;
    h_corr->Fill(((xPos*(-6.09219948))/1000)+100+5.401, mean/1000); // unit of (mm, mm) //we mult. by 2 since 2 is the pitch in mm

    for (int i=0; i < localCentroidSingle->GetNbinsX(); ++i) {
      if (localCentroidSingle->GetBinContent(i+1) < localCentroidSingle->GetBinContent(localCentroidSingle->GetMaximumBin())) localCentroidSingle->SetBinContent(i+1,0);
    }
    //    Float_t weightedAcc = (localCentroidSingle->GetBinContent(localCentroidSingle->GetMaximumBin()) * (localCentroidSingle->GetMean() - (((xPos*(-6.09219948))+100000))+ 5278) + (localCentroidMulti->GetEntries()) * (mean-(((xPos*(-6.09219948))+100000))+ 5278) )/ (localCentroidSingle->GetBinContent(localCentroidSingle->GetMaximumBin()) + localCentroidMulti->GetEntries()) ;

    localResMulti->Fill(((xPos*(-6.09219948))/1000)+100+5.401, sigma); //unit of (mm, um)
    Float_t error =  mean - (((xPos*(-6.09219948))+100000))+ 5401;
    localAcc->Fill(((xPos*(-6.09219948))/1000)+100+5.401, error); 

    // errorCorrected->Fill(((xPos*(-6.09219948))/1000)+100, meanCorr-(((xPos*(-6.09219948))+100000))+ 5278); 
    Float_t chSh2 = 0; Float_t chSh3 = 0;
    for (int i=0; i < (int)avgChSh2.size(); ++i) {
      chSh2 += avgChSh2[i];
    }
    chSh2 = chSh2 / avgChSh2.size();
    for (int i=0; i < (int)avgChSh3.size(); ++i) {
      chSh3 += avgChSh3[i];
    }
 
    chSh3 = chSh3 / (int) avgChSh3.size();
    //    globalClusterSize1Motor->Fill(((((xPos*(-6.09219948))/1000)+100)-0.23)/2,localClusterSizeX->GetBinContent(2)/localClusterSizeX->GetEntries());
    globalClusterSize5Motor->Fill(((xPos*(-6.09219948))/1000)+100+5.401, localClusterSizeX->GetBinContent(6)/localClusterSizeX->GetEntries());
    globalClusterSize6Motor->Fill(((xPos*(-6.09219948))/1000)+100+5.401,localClusterSizeX->GetBinContent(7)/localClusterSizeX->GetEntries());
    //    globalChargeSharing1Motor->Fill(((((xPos*(-6.09219948))/1000)+100)-0.23)/2,1);
    globalChargeSharing5Motor->Fill(((xPos*(-6.09219948))/1000)+100+5.401,chSh2);
    globalChargeSharing6Motor->Fill(((xPos*(-6.09219948))/1000)+100+5.401,chSh3);
			     //cout << " pos " << ((((xPos*(-6.09219948))/1000)+100)-0.23) << " cluster size " << localClusterSizeX->GetBinContent(6)/localClusterSizeX->GetEntries() << endl;






    globalAvgClusterSize->Fill(((xPos*(-6.09219948))/1000)+100+5.401,localClusterSizeX->GetMean());
    //    if (nFiles == 1) initialPos = xPos;
    //    float diff = (((xPos*(-6.09219948))+100000)) - initialPos*10;
    Int_t cCount3 = 0;
    //Float_t xPosOld = xPos;
    //  cout << " inital xpos " << xPos * 10 << endl;
    // cout << " 2 " << 0.12 * 10 << endl;
    // cout << " calibration " << xPos * 6.3488 << endl;
    //if (nFiles != 1) xPos = xPos +((diff/100)*.12);
    
    //   cout << " 3 " << xPos * 10 << endl;
    for (int i=0; i < nEvents; ++i) {
      event->GetEntry(i);
      if (eClusterMult > 10 ) { 
        cCount3+=eClusterMult;
        continue;
      }
      for (int j=0; j < eClusterMult; ++j) {
	cluster->GetEntry(cCount3);
        cCentroid -= 336;
	if (cCentroid < 0 || cCentroid > 48) {++cCount3; continue;}
        if (eEventNum != cEventNum) {
	  cerr << "3rd pass: Cluster and event info mismatched. EventNum " << eEventNum << " cluster event " << cEventNum << " ccount " << cCount2 << endl;
	  //   return -1;
	  continue;
       	}

        if (/*cPeakCharge <= 200*/ cClusterCharge < 2000 || cTimeLength <= 8 /*|| cluster->GetYCentroid() != 3 */) {
            ++cCount3;
	    continue;
	 
        }
        Float_t xCentroid = (cCentroid*2000) + 1000;
	//       	cout << " centroid " << cCentroid << " xCentroid " << xCentroid << endl;
        if (xCentroid == xCentroid && xCentroid !=0) {

          globalCentroid->Fill(xCentroid - (((xPos*(-6.09219948))+100000)) + 5401); // 58856216
          globalCentroidIdeal->Fill(xCentroid - mean);
          globalStitch->Fill(xCentroid/1000);
          

          if (cClusterSize > 1) {
            globalAccUniv->Fill(fmod((xCentroid/1000),2)-1, xCentroid-(((xPos*(-6.09219948))+100000))+ 5401);
            
	    //            cout << " centroid mm " << xCentroid/1000 << " centroid um " << xCentroid << " xPos * 6.95 " << xPos*6.95 << endl;
            globalAcc->Fill((xCentroid/1000),xCentroid-(((xPos*(-6.09219948))+100000))+ 5401);
            globalCorrelation->Fill(xCentroid/1000,(((xPos*(-6.09219948))/1000)+100) + 5.401);
	    //cout << " centroid " << xCentroid/1000 << " motor " << (((xPos*(-6.09219948))/1000)+100) << endl;
            globalCorrelationOld->Fill(xCentroid/1000,((xPos*(-6.09219948))/1000)+100+5.401         ) ;
            globalCentroidMultiCorrected->Fill(xCentroid - (mean - (((xPos*(-6.09219948))+100000))+ 5401) - (((xPos*(-6.09219948))+100000)) + 5401); // is just the ideal case. not truly corrected
            localCentroidMultiCorr->Fill(xCentroid - (mean - (((xPos*(-6.09219948))+100000))+ 5278));
	    //cout <<" xcentroid " << xCentroid << " mean " << mean << " xpos " << (((xPos*(-6.09219948))+100000)) + 5278 << endl;
	    //cout << " multicorr value " << xCentroid - (mean - (((xPos*(-6.09219948))+100000))+ 5278) << endl;
          }
          if (cClusterSize == 5) globalAccUniv5->Fill(fmod((xCentroid/1000),2)-1, xCentroid-(((xPos*(-6.09219948))+100000))+ 5401);
          if (cClusterSize == 6) globalAccUniv6->Fill(fmod((xCentroid/1000),2)-1, xCentroid-(((xPos*(-6.09219948))+100000))+ 5401);

          //if (cluster->GetClusterSizeX() == 1) localCentroidSingleCorr->Fill(xCentroid);
        }
        ++cCount3;
      }
    }

 
    float avgCorr = localCentroidMultiCorr->GetMean();
    float rmsCorr = localCentroidMultiCorr->GetRMS();
    // TFitResultPtr resultPtr;
    localCentroidMultiCorr->SetAxisRange(avgCorr-(rmsCorr),avgCorr+(rmsCorr),"X");
    avgCorr = localCentroidMultiCorr->GetMean();
    rmsCorr = localCentroidMultiCorr->GetRMS();
    localCentroidMultiCorr->SetAxisRange(avgCorr-(0.6*rmsCorr),avgCorr+(rmsCorr),"X");
    avgCorr = localCentroidMultiCorr->GetMean();
    rmsCorr = localCentroidMultiCorr->GetRMS();
					 //    cout << " avgcorr " << avgCorr << " rmsCorr " << rmsCorr << endl;
    localCentroidMultiCorr->Fit("gaus", "Q0", "", avgCorr-(rmsCorr), avgCorr+(rmsCorr));
					 //    cout << " check21" << endl;
    //  Int_t checkFit = resultPtr;
    TF1 *fitCorr = localCentroidMultiCorr->GetFunction("gaus");
    float meanCorr = fitCorr->GetParameter(1);  //unit of pads
    //float sigmaCorr = fitCorr->GetParameter(2);  //unit of pads

    errorVec[nFiles-1] =mean-(((xPos*(-6.09219948))+100000))+ 5401;
    centroidVec[nFiles-1] = mean/1000;
    // cout << " centroid " << mean/1000 << " error " << mean-(((xPos*(-6.09219948))+100000))+ 5278  << endl;
    /*
    float avgCorrTest = localCentroidTest->GetMean();
    float rmsCorrTest = localCentroidTest->GetRMS();
    // TFitResultPtr resultPtr;
    localCentroidTest->Fit("gaus", "Q0", "", avgCorrTest-(.5*rmsCorrTest), avgCorrTest+(.5*rmsCorrTest));
    //  Int_t checkFit = resultPtr;
    TF1 *fitCorrTest = localCentroidTest->GetFunction("gaus");
    float meanCorrTest = fitCorrTest->GetParameter(1);  //unit of pads
    float sigmaCorrTest = fitCorrTest->GetParameter(2);  //unit of pads
    
    delete fitCorrTest;
    */ 
    if (((xPos*(-6.09219948))/1000)+100 > 18.3 && ((xPos*(-6.09219948))/1000)+100 < 18.4) {
      //   cout << " mean corr " << meanCorr << " mean " << mean << " total after correction " << meanCorr-(((xPos*(-6.09219948))+100000))+ 5278 << " total before correction " << mean - (((xPos*(-6.09219948))+100000))+ 5278 << endl;
      //  cout << " mean test " << meanCorrTest << " test after correction " << meanCorrTest-(((xPos*(-6.09219948))+100000))+ 5278<< endl;
      // cout << "corrected bounds " <<  avgCorr-(rmsCorr) << " - " <<  avgCorr+(rmsCorr) << endl;
      //   cout << " initial bounds " <<  avg-(rms) << " - " <<  avg+(rms) << endl;
  TCanvas *c50 = new TCanvas("c50","c50");
  c50->Divide(2,1);
  c50->cd(1);
  localCentroidMulti->Draw();
  c50->cd(2);
  localCentroidMultiCorr->Draw();
  //  c50->SaveAs("DEBUG.root");
  delete c50; 
    }




    //    Float_t weightedAccCorr = (localCentroidSingle->GetBinContent(localCentroidSingle->GetMaximumBin()) * (localCentroidSingle->GetMean() - (((xPos*(-6.09219948))+100000))+ 5278) + (localCentroidMultiCorr->GetEntries()) * (meanCorr-(((xPos*(-6.09219948))+100000))+ 5278) )/ (localCentroidSingle->GetBinContent(localCentroidSingle->GetMaximumBin()) + localCentroidMultiCorr->GetEntries()) ;

    Float_t correctedError =  meanCorr-(((xPos*(-6.09219948))+100000))+ 5401;
    errorCorrected->Fill(((xPos*(-6.09219948))/1000)+100+5.401, correctedError); 
    delete fitCorr;

  Int_t cCount4 = 0;
    
    for (int i=0; i < nEvents; ++i) {
      event->GetEntry(i);
      if (eClusterMult > 10 ) { 
        cCount4+=eClusterMult;
        continue;
      }
      for (int j=0; j < eClusterMult; ++j) {
        cluster->GetEntry(cCount4);
        cCentroid -= 336;
	if (cCentroid < 0 || cCentroid > 48) {++cCount4; continue;}
        if (eEventNum != cEventNum) {
	  cerr << "4th pass: Cluster and event info mismatched. EventNum " << eEventNum << " cluster event " << cEventNum << " ccount " << cCount4 << endl;
	  //          return -1;
	  continue;
	}

        if (/*cPeakCharge <= 1000*/ cClusterCharge < 2000 || cTimeLength <= 8 /*|| cluster->GetYCentroid() != 3 */) {
            ++cCount4;
	    continue;
	 
        }
        Float_t xCentroid = (cCentroid*2000) + 1000;
                    
        if (xCentroid == xCentroid && xCentroid !=0) {
globalAccMotor->Fill((((xPos*(-6.09219948))/1000)+100)+5.401,xCentroid-(((xPos*(-6.09219948))+100000))+ 5401);
//cout << " motor  " << (((xPos*(-6.09219948))/1000)+100)-11.178 << " error centroid " << xCentroid << " mean "<< mean <<" motor " << (((xPos*(-6.09219948))+100000))+ 5278 << endl;
// cout << " corrected error " << endl;
 // cout << " motor  " << (((xPos*(-6.09219948))/1000)+100)-11.178 << " value corrected " << xCentroid-meanCorr-(((xPos*(-6.09219948))+100000))+ 5278 << endl;
          if (cClusterSize > 1) {
            globalAccCorr->Fill((xCentroid/1000),xCentroid-error-(((xPos*(-6.09219948))+100000))+ 5401);
          }
          //if (cluster->GetClusterSizeX() == 1) localCentroidSingleCorr->Fill(xCentroid);
        }
        ++cCount4;
      }
    }
    /*
   TCanvas *c6 = new TCanvas("c6","c6");
   char namec6[150];
   sprintf(namec6,"LocalCentroidXRun%d.png",runNum);
   localCentroid->Draw();
   // c6->SaveAs(namec6,"Q0");
   sprintf(namec6,"LocalCentroidXRun%d.root",runNum);
   c6->SaveAs(namec6,"Q0");

   TCanvas *c16 = new TCanvas("c16","c16");
   char namec16[150];
   sprintf(namec16,"LocalCentroidXSingleRun%d.png",runNum);
   localCentroidSingle->Draw();
   //  c16->SaveAs(namec16,"Q0");
   sprintf(namec16,"LocalCentroidXSingleRun%d.root",runNum);
   c16->SaveAs(namec16,"Q0");

   TCanvas *c17 = new TCanvas("c17","c17");
   char namec17[150];
   sprintf(namec17,"LocalCentroidXMultiRun%d.png",runNum);
   localCentroidMulti->Draw();
   // c17->SaveAs(namec17,"Q0");
   sprintf(namec17,"LocalCentroidXMultiRun%d.root",runNum);
   c17->SaveAs(namec17,"Q0");

   TCanvas *c10 = new TCanvas("c10","c10");
   char namec10[150];
    sprintf(namec10,"LocalClusterSizeXRun%d.png",runNum);
   localClusterSizeX->Draw();
   // c10->SaveAs(namec10,"Q0");
   sprintf(namec10,"LocalClusterSizeXRun%d.root",runNum);
   c10->SaveAs(namec10,"Q0");

   TCanvas *c11 = new TCanvas("c11","c11");
   char namec11[150];
   sprintf(namec11,"LocalChargeSpectrumRun%d.png",runNum);
   localChargeSpectrum->Draw();
   //  c11->SaveAs(namec11,"Q0");
   sprintf(namec11,"LocalChargeSpectrumRun%d.root",runNum);
   c11->SaveAs(namec11,"Q0");
    */
   delete localCentroid; delete localCentroidSingle; delete localCentroidMulti; delete localChargeSpectrum; delete localClusterSizeX; delete localCentroidMultiCorr;              delete localCentroidTest;
   //  delete c6;  delete c10; delete c11; delete c16; delete c17;
  }
  //  globalChargeSharing1Cent->Divide(globalClusterSize1Cent);
  globalChargeSharing5Cent->Divide(globalClusterSize5Cent);
  globalChargeSharing6Cent->Divide(globalClusterSize6Cent);

    for (int i=0; i < globalClusterSize6Cent->GetNbinsX(); ++i) {
      int totalEntries = globalClusterSize5Cent->GetBinContent(i+1)+globalClusterSize6Cent->GetBinContent(i+1);
      if (totalEntries != 0) {
	//        globalClusterSize1Cent->SetBinContent(i+1,globalClusterSize1Cent->GetBinContent(i+1)/totalEntries);
        globalClusterSize5Cent->SetBinContent(i+1,globalClusterSize5Cent->GetBinContent(i+1)/totalEntries);
        globalClusterSize6Cent->SetBinContent(i+1,globalClusterSize6Cent->GetBinContent(i+1)/totalEntries);
      }
    }




  TCanvas *c40 = new TCanvas("c40","c40");
  c40->Divide(3,2);
  c40->cd(1);
  globalCentroidMulti->SetMarkerStyle(20);
  globalCentroidMulti->SetMarkerColor(4);
  globalCentroidMulti->Draw();
  c40->cd(2);
  globalAcc->Draw();
  c40->cd(3);
  localAcc->SetMarkerStyle(20);
  localAcc->SetMarkerColor(4);
  localAcc->Draw("LP");
  c40->cd(4);
  globalCentroidMultiCorrected->SetMarkerStyle(20);
  globalCentroidMultiCorrected->SetMarkerColor(4);
  globalCentroidMultiCorrected->Draw();
  c40->cd(5);
  globalAccCorr->Draw();
  c40->cd(6);
  errorCorrected->SetMarkerStyle(20);
  errorCorrected->SetMarkerColor(4);
  errorCorrected->Draw("LP");

  char file160[150];
  sprintf(file160,"%s/ResAndCorrection.root",baseDir);
  c40->SaveAs(file160,"q");
  TCanvas *c30 = new TCanvas("c30","c30");
  
  /*
  globalClusterSize1Cent->SetMarkerStyle(20);
  globalClusterSize1Cent->SetMarkerColor(2);
  globalClusterSize1Cent->SetLineColor(2);
  globalClusterSize1Cent->GetXaxis()->SetLabelSize(.025);
  globalClusterSize1Cent->GetYaxis()->SetTitleOffset(1.3);
  globalClusterSize1Cent->Draw("L");
  */
  globalClusterSize5Cent->SetMarkerStyle(20);
  globalClusterSize5Cent->SetMarkerColor(4);
  globalClusterSize5Cent->SetLineColor(4);
  globalClusterSize5Cent->Draw("L");

  globalClusterSize6Cent->SetMarkerStyle(20);
  globalClusterSize6Cent->SetMarkerColor(8);
  globalClusterSize6Cent->SetLineColor(8);
  globalClusterSize6Cent->Draw("Lsame");

  TLegend *legend2C = new TLegend(0.76,0.76,0.99,0.94);
  // legend2C->SetTextFont(72);
  // legend2C->SetTextSize(0.04);
  legend2C->SetFillColor(0);
  //  legend2C->AddEntry(globalClusterSize1Cent, "1 Pad Clusters", "l");
  legend2C->AddEntry(globalClusterSize5Cent, "5 Pad Clusters", "l");
  legend2C->AddEntry(globalClusterSize6Cent, "6 Pad Clusters", "l");
  legend2C->Draw();


  char file60[150];
  sprintf(file60,"%s/globalClusterSizeCentroid.root",baseDir);
  c30->SaveAs(file60,"q");
  
 TCanvas *c61 = new TCanvas("c61","c61");
  c61->Divide(2,1);
  c61->cd(1);
  globalStitch->Draw("L");

  c61->cd(2);
  /*
 globalClusterSize1Cent->SetMarkerStyle(20);
  globalClusterSize1Cent->SetMarkerColor(2);
  globalClusterSize1Cent->SetLineColor(2);
  globalClusterSize1Cent->GetXaxis()->SetLabelSize(.025);
  globalClusterSize1Cent->GetYaxis()->SetTitleOffset(1.3);
  globalClusterSize1Cent->Draw("L");
  */
  globalClusterSize5Cent->SetMarkerStyle(20);
  globalClusterSize5Cent->SetMarkerColor(4);
  globalClusterSize5Cent->SetLineColor(4);
  globalClusterSize5Cent->Draw("L");

  globalClusterSize6Cent->SetMarkerStyle(20);
  globalClusterSize6Cent->SetMarkerColor(8);
  globalClusterSize6Cent->SetLineColor(8);
  globalClusterSize6Cent->Draw("Lsame");

  TLegend *legend3C = new TLegend(0.76,0.76,0.99,0.94);
  // legend2C->SetTextFont(72);
  // legend2C->SetTextSize(0.04);
  legend3C->SetFillColor(0);
  //  legend3C->AddEntry(globalClusterSize1Cent, "1 Pad Clusters", "l");
  legend3C->AddEntry(globalClusterSize5Cent, "5 Pad Clusters", "l");
  legend3C->AddEntry(globalClusterSize6Cent, "6 Pad Clusters", "l");
  legend3C->Draw();
  char file2[150];
  sprintf(file2,"%s/GlobalStitch.root",baseDir);
  c61->SaveAs(file2,"q");

  /*

    for (int i=0; i < globalAvgClusterSize->GetNbinsX(); ++i) {
      int totalEntries = globalAvgClusterSize->GetBinContent(i+1);
      float binCenter = globalAvgClusterSize->GetXaxis()->GetBinCenter(i+1);
      if (totalEntries == 0 && binCenter > 10.75 && binCenter < 11) {
        if ( globalAvgClusterSize->GetBinContent(i+2) != 0 && globalAvgClusterSize->GetBinContent(i) != 0){
	  globalAvgClusterSize->SetBinContent(i+1, (globalAvgClusterSize->GetBinContent(i+2)+ globalAvgClusterSize->GetBinContent(i))/2 );
	  //	  globalChargeSharing1Motor->SetBinContent(i+1,(globalChargeSharing1Motor->GetBinContent(i+2)+globalChargeSharing1Motor->GetBinContent(i))/2);
	  globalChargeSharing5Motor->SetBinContent(i+1,(globalChargeSharing5Motor->GetBinContent(i+2)+globalChargeSharing5Motor->GetBinContent(i))/2);
	  globalChargeSharing6Motor->SetBinContent(i+1,(globalChargeSharing6Motor->GetBinContent(i+2)+globalChargeSharing6Motor->GetBinContent(i))/2);
	  // globalClusterSize1Motor->SetBinContent(i+1,globalClusterSize1Motor->GetBinContent(i+2));
 globalClusterSize5Motor->SetBinContent(i+1,globalClusterSize5Motor->GetBinContent(i+2));
 globalClusterSize6Motor->SetBinContent(i+1,globalClusterSize6Motor->GetBinContent(i+2));
	}
        else  {
 globalAvgClusterSize->SetBinContent(i+1, globalAvgClusterSize->GetBinContent(i));
 // globalChargeSharing1Motor->SetBinContent(i+1,globalChargeSharing1Motor->GetBinContent(i));
 globalChargeSharing5Motor->SetBinContent(i+1,globalChargeSharing5Motor->GetBinContent(i));
 globalChargeSharing6Motor->SetBinContent(i+1,globalChargeSharing6Motor->GetBinContent(i));
 // globalClusterSize1Motor->SetBinContent(i+1,globalClusterSize1Motor->GetBinContent(i));
 globalClusterSize5Motor->SetBinContent(i+1,globalClusterSize5Motor->GetBinContent(i));
 globalClusterSize6Motor->SetBinContent(i+1,globalClusterSize6Motor->GetBinContent(i));
	}
      }
    }
  
  */

 TCanvas *cf1 = new TCanvas("cf1","cf1");
  globalStitch->SetMarkerStyle(20);
  globalStitch->SetMarkerColor(4);
  globalStitch->Draw("L");
  globalStitch->GetXaxis()->SetRangeUser(18.6,26.4);
  char file3[150];
  sprintf(file3,"%s/FinalPlot1.root",baseDir);
  cf1->SaveAs(file3);

 TCanvas *cf2 = new TCanvas("cf2","cf2");
  globalAvgClusterSize->Draw("PL");
  globalAvgClusterSize->GetXaxis()->SetRangeUser(18.6,26.4);
  char file4[150];
  sprintf(file4,"%s/FinalPlot2.root",baseDir);
  cf2->SaveAs(file4);

 TCanvas *cf3 = new TCanvas("cf3","cf3");
 /*
  globalChargeSharing1Motor->SetMarkerStyle(20);
  globalChargeSharing1Motor->SetMarkerColor(2);
  globalChargeSharing1Motor->SetLineColor(2);
  globalChargeSharing1Motor->GetYaxis()->SetTitleOffset(1.3);
  globalChargeSharing1Motor->Draw("L");
  globalChargeSharing1Motor->GetYaxis()->SetRangeUser(0,1);
  globalChargeSharing1Motor->GetXaxis()->SetRangeUser(18.6,26.4);
 */
  globalChargeSharing5Motor->SetMarkerStyle(20);
  globalChargeSharing5Motor->SetMarkerColor(4);
  globalChargeSharing5Motor->SetLineColor(4);
  globalChargeSharing5Motor->Draw("L");

  globalChargeSharing6Motor->SetMarkerStyle(20);
  globalChargeSharing6Motor->SetMarkerColor(8);
  globalChargeSharing6Motor->SetLineColor(8);
  globalChargeSharing6Motor->Draw("Lsame");

  TLegend *legend1M = new TLegend(0.76,0.76,0.99,0.94);
  // legend1M->SetTextFont(72);
  // legend1M->SetTextSize(0.04);
  legend1M->SetFillColor(0);
  //  legend1M->AddEntry(globalChargeSharing1Motor, "1 Pad Clusters", "l");
  legend1M->AddEntry(globalChargeSharing5Motor, "5 Pad Clusters", "l");
  legend1M->AddEntry(globalChargeSharing6Motor, "6 Pad Clusters", "l");
  legend1M->Draw();
  char file5[150];
  sprintf(file5,"%s/FinalPlot3.root",baseDir);
  cf3->SaveAs(file5);

 TCanvas *cf4 = new TCanvas("cf4","cf4");
  globalClusterSize5Motor->SetMarkerStyle(20);
  globalClusterSize5Motor->SetMarkerColor(4);
  globalClusterSize5Motor->SetLineColor(4);
  globalClusterSize5Motor->GetYaxis()->SetTitleOffset(1.3);
  globalClusterSize5Motor->Draw("L");
  globalClusterSize5Motor->GetYaxis()->SetRangeUser(0,1);
  globalClusterSize5Motor->GetXaxis()->SetRangeUser(18.6,26.4);
  /*
  globalClusterSize1Motor->SetMarkerStyle(20);
  globalClusterSize1Motor->SetMarkerColor(2);
  globalClusterSize1Motor->SetLineColor(2);
  globalClusterSize1Motor->Draw("Lsame");
  */
  globalClusterSize6Motor->SetMarkerStyle(20);
  globalClusterSize6Motor->SetMarkerColor(8);
  globalClusterSize6Motor->SetLineColor(8);
  globalClusterSize6Motor->Draw("Lsame");

  TLegend *legend2M = new TLegend(0.76,0.76,0.99,0.94);
  // legend2M->SetTextFont(72);
  // legend2M->SetTextSize(0.04);
  legend2M->SetFillColor(0);
  // legend2M->AddEntry(globalClusterSize1Motor, "1 Pad Clusters", "l");
  legend2M->AddEntry(globalClusterSize5Motor, "5 Pad Clusters", "l");
  legend2M->AddEntry(globalClusterSize6Motor, "6 Pad Clusters", "l");
  legend2M->Draw();
  char file6[150];
  sprintf(file6,"%s/FinalPlot4.root",baseDir);
  cf4->SaveAs(file6);
  
 TCanvas *c31 = new TCanvas("c31","c31");
  c31->Divide(2,2);

  c31->cd(1);
  globalStitch->SetMarkerStyle(20);
  globalStitch->SetMarkerColor(4);
  globalStitch->Draw("L");

  c31->cd(2);
  //globalAvgClusterSize->SetMarkerStyle(20);
  //globalAvgClusterSize->SetMarkerColor(4);
  globalAvgClusterSize->Draw("L");

  c31->cd(3);
  /*
  globalChargeSharing1Motor->SetMarkerStyle(20);
  globalChargeSharing1Motor->SetMarkerColor(2);
  globalChargeSharing1Motor->SetLineColor(2);
  globalChargeSharing1Motor->GetYaxis()->SetTitleOffset(1.3);
  globalChargeSharing1Motor->Draw("L");
  */
  globalChargeSharing5Motor->SetMarkerStyle(20);
  globalChargeSharing5Motor->SetMarkerColor(4);
  globalChargeSharing5Motor->SetLineColor(4);
  globalChargeSharing5Motor->Draw("L");

  globalChargeSharing6Motor->SetMarkerStyle(20);
  globalChargeSharing6Motor->SetMarkerColor(8);
  globalChargeSharing6Motor->SetLineColor(8);
  globalChargeSharing6Motor->Draw("Lsame");

  TLegend *legend3M = new TLegend(0.76,0.76,0.99,0.94);
  // legend1M->SetTextFont(72);
  // legend1M->SetTextSize(0.04);
  legend3M->SetFillColor(0);
  //  legend3M->AddEntry(globalChargeSharing1Motor, "1 Pad Clusters", "l");
  legend3M->AddEntry(globalChargeSharing5Motor, "5 Pad Clusters", "l");
  legend3M->AddEntry(globalChargeSharing6Motor, "6 Pad Clusters", "l");
  legend3M->Draw();

  c31->cd(4);

  globalClusterSize5Motor->SetMarkerStyle(20);
  globalClusterSize5Motor->SetMarkerColor(4);
  globalClusterSize5Motor->SetLineColor(4);
  globalClusterSize5Motor->GetYaxis()->SetTitleOffset(1.3);
  globalClusterSize5Motor->Draw("L");
  globalClusterSize5Motor->GetYaxis()->SetRangeUser(0,1);
  /*
  globalClusterSize1Motor->SetMarkerStyle(20);
  globalClusterSize1Motor->SetMarkerColor(2);
  globalClusterSize1Motor->SetLineColor(2);
  globalClusterSize1Motor->Draw("Lsame");
  */
  globalClusterSize6Motor->SetMarkerStyle(20);
  globalClusterSize6Motor->SetMarkerColor(8);
  globalClusterSize6Motor->SetLineColor(8);
  globalClusterSize6Motor->Draw("Lsame");

  TLegend *legend4M = new TLegend(0.76,0.76,0.99,0.94);
  // legend2M->SetTextFont(72);
  // legend2M->SetTextSize(0.04);
  legend4M->SetFillColor(0);
  //legend4M->AddEntry(globalClusterSize1Motor, "1 Pad Clusters", "l");
  legend4M->AddEntry(globalClusterSize5Motor, "5 Pad Clusters", "l");
  legend4M->AddEntry(globalClusterSize6Motor, "6 Pad Clusters", "l");
  legend4M->Draw();
  char file7[150];
  sprintf(file7,"%s/globalChargeSharingMotor.root",baseDir);
  c31->SaveAs(file7,"q");

  TCanvas *c7 = new TCanvas("c7","c7");
  h_corr->SetMarkerStyle(20);
  h_corr->SetMarkerColor(4);
  h_corr->Draw("LP");
  char file8[150];
  sprintf(file8,"%s/CorrelationPlotmm.root",baseDir);
  // c7->SaveAs("CorrelationPlot.png","q");
  c7->SaveAs(file8,"q");

  TCanvas *c3 = new TCanvas("c3","c3");
  c3->Divide(2,1);
  c3->cd(1);
  globalStitch->SetMarkerStyle(20);
  globalStitch->SetMarkerColor(4);
  globalStitch->Draw("L");
  c3->cd(2);
  localResMulti->SetMarkerStyle(20);
  localResMulti->SetMarkerColor(4);
  localResMulti->Draw("LP");
  // c3->SaveAs("LocalResolutionMulti.png","q");
  char file9[150];
  sprintf(file9,"%s/LocalResolutionMulti.root",baseDir);
  c3->SaveAs(file9,"q");
   TCanvas *c4 = new TCanvas("c4","c4");
  globalCentroid->SetMarkerStyle(20);
  globalCentroid->SetMarkerColor(4);
  globalCentroid->Draw();
  // globalCentroid->Fit("gaus", "Q0", "", globalCentroid->GetMean()-(0.5*globalCentroid->GetRMS()),globalCentroid->GetMean()+(0.5*globalCentroid->GetRMS()));
  // c4->SaveAs("GlobalResolution.png","q");
  char file10[150];
  sprintf(file10,"%s/GlobalResolution.root",baseDir);
  c4->SaveAs(file10,"q");

   TCanvas *c14 = new TCanvas("c14","c14");
  globalCentroidMulti->SetMarkerStyle(20);
  globalCentroidMulti->SetMarkerColor(4);
  globalCentroidMulti->Draw();
  // globalCentroidMulti->Fit("gaus", "Q0", "", globalCentroidMulti->GetMean()-(0.5*globalCentroidMulti->GetRMS()),globalCentroidMulti->GetMean()+(0.5*globalCentroidMulti->GetRMS()));
  // c14->SaveAs("GlobalResolutionMulti.png","q");
  char file11[150];
  sprintf(file11,"%s/GlobalResolution.root",baseDir);
   c14->SaveAs(file11,"q");

   TCanvas *c8 = new TCanvas("c8","c8");
  globalCentroidIdeal->SetMarkerStyle(20);
  globalCentroidIdeal->SetMarkerColor(4);
  globalCentroidIdeal->Draw();
  // globalCentroidIdeal->Fit("gaus", "Q0", "", globalCentroidIdeal->GetMean()-(0.5*globalCentroidIdeal->GetRMS()),globalCentroidIdeal->GetMean()+(0.5*globalCentroidIdeal->GetRMS()));
  // c8->SaveAs("GlobalResolutionIdeal.png","q");
  char file12[150];
  sprintf(file12,"%s/GlobalResolutionIdeal.root",baseDir);
  c8->SaveAs(file12,"q");

   TCanvas *c15 = new TCanvas("c15","c15");
  globalCentroidIdealMulti->SetMarkerStyle(20);
  globalCentroidIdealMulti->SetMarkerColor(4);
  globalCentroidIdealMulti->Draw();
  // globalCentroidIdealMulti->Fit("gaus", "Q0", "", globalCentroidIdealMulti->GetMean()-(0.5*globalCentroidIdealMulti->GetRMS()),globalCentroidIdealMulti->GetMean()+(0.5*globalCentroidIdealMulti->GetRMS()));
  // c15->SaveAs("GlobalResolutionIdealMulti.png","q");
  char file13[150];
  sprintf(file13,"%s/GlobalResolutionIdealMulti.root",baseDir);
  c15->SaveAs(file13,"q");

  // TCanvas *c24 = new TCanvas("c24","c24");

  //  globalStitch->Fit("gaus", "Q0", "", globalStitch->GetMean()-(0.5*globalStitch->GetRMS()),globalStitch->GetMean()+(0.5*globalStitch->GetRMS()));
  // c24->SaveAs("GlobalStitch.png","q");

  // c24->SaveAs("GlobalStitch.root","q");
  TCanvas *c44 = new TCanvas("c44","c44");
  globalCentroidMulti->Draw();
  char file14[150];
  sprintf(file14,"%s/globalCentroidMulti.root",baseDir);
  c44->SaveAs(file14);

  TCanvas *c45 = new TCanvas("c45","c45");
  globalCentroidMultiCorrected->Draw();
  char file15[150];
  sprintf(file15,"%s/globalCentroidMultiCorrected.root",baseDir);
  c45->SaveAs(file15);

  TCanvas *c49 = new TCanvas("c49","c49");
  c49->Divide(2,2);
  c49->cd(1);
  globalCentroidMulti->Draw();
  c49->cd(2);
  globalAcc->Draw();
  c49->cd(3);
  localAcc->SetMarkerStyle(20);
  localAcc->SetMarkerColor(4);
  localAcc->Draw("LP");
  c49->cd(4);
  globalCentroidMultiCorrected->Draw();
  c49->cd(5);
  globalAccCorr->Draw();
  c49->cd(6);
  errorCorrected->SetMarkerStyle(20);
  errorCorrected->SetMarkerColor(4);
  errorCorrected->Draw("LP");
  char file16[150];
  sprintf(file16,"%s/ResolutionAndCorrection.root",baseDir);
  c49->SaveAs(file16,"q");
  
  const Int_t nPoints = 100;
  TCanvas *c55 = new TCanvas("c55","c55");
  TGraph *errorCentroid = new TGraph(nPoints,centroidVec,errorVec);
  errorCentroid->Draw("ap");
  char file17[150];
  sprintf(file17,"%s/errorCentroid.root",baseDir);
  c55->SaveAs(file17);

    TCanvas *c56 = new TCanvas("c56","c56");
   globalAcc->Draw();
  char file18[150];
  sprintf(file18,"%s/globalAcc.root",baseDir);
  c56->SaveAs(file18);

  TCanvas *c57 = new TCanvas("c57","c57");
  globalCorrelation->Draw();
  char file19[150];
  sprintf(file19,"%s/globalCorrelation.root",baseDir);
  c57->SaveAs(file19);


    TCanvas *c58 = new TCanvas("c58","c58");
 globalAccMotor->Draw();
  char file20[150];
  sprintf(file20,"%s/globalAccMotor.root",baseDir);
  c58->SaveAs(file20);

    TCanvas *c59 = new TCanvas("c59","c59");
 globalAccUniv->Draw();
  char file21[150];
  sprintf(file21,"%s/globalAccUniv.root",baseDir);
  c59->SaveAs(file21);

    TCanvas *c69 = new TCanvas("c69","c69");
 globalAccUniv5->Draw();
  char file22[150];
  sprintf(file22,"%s/globalAccUniv5.root",baseDir);
  c69->SaveAs(file22);

    TCanvas *c79 = new TCanvas("c79","c79");
 globalAccUniv6->Draw();
  char file23[150];
  sprintf(file23,"%s/globalAccUniv6.root",baseDir);
  c79->SaveAs(file23);
  
  delete c7;  delete c3; delete c4; delete c8; delete c14; delete c15;  delete c30; delete c31; delete c40;  delete c44; delete c45; delete c49; delete c55; delete c56; delete c57; delete c58; delete c59; delete c69; delete c79;
  return 0;
}
