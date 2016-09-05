#include <fstream>
#include <vector>
#include <iostream>

#include <TH1F.h>
#include <TF1.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TROOT.h>

using namespace std;

int main() {
   

      char h1Name[256];
      sprintf(h1Name,"maxTB%d");
      TH1F *maxTBHisto = new TH1F(h1Name,h1Name,27, -0.5, 26.5);
      
      char h2Name[256];
      sprintf(h2Name,"meanTime%d");
      TH1F *meanTimeHisto = new TH1F(h2Name,h2Name,300,5 , 18);
      
  for (int z = 5; z < 17; ++z) { 
      char fileName[256];
      if (z<10) sprintf(fileName,"/data/mike/ExtPulserScan/ExtPulserScan/TreeTimingJitter2Fe55Run8010%d.root",z);
      else sprintf(fileName,"/data/mike/ExtPulserScan/ExtPulserScan/TreeTimingJitter2Fe55Run801%d.root",z);
      TFile *file = TFile::Open(fileName);
      if (!file) {cerr << "file not open " << endl; return 1;}
      TTree *hitTree = new TTree("Hit","Hit");   
      TTree *eventTree = new TTree("Event","Event");   
      hitTree = (TTree*) file->Get("Hit;1");
      eventTree = (TTree*) file->Get("Event;1");
  
      Int_t nEvents = (Int_t) eventTree->GetEntries();

     
      vector<Float_t> *timeBinChargeVec1 = 0;
      hitTree->SetBranchAddress("TimeBinChargeVec",&timeBinChargeVec1);
      int padNo = 0;
      hitTree->SetBranchAddress("PadNo",&padNo);
     
     
      int count = 0;
      int eventNo = 0;
      
      int tbCount = 0;
      int maxTB = 0;
      Float_t pedRMS = 0;
      Float_t maxCharge = 0;
      hitTree->SetBranchAddress("MaxCharge",&maxCharge);
      hitTree->SetBranchAddress("MaxTimeBin",&maxTB);
      hitTree->SetBranchAddress("EventNo",&eventNo);
      //hitTree->SetBranchAddress("PedRMS",&pedRMS);
      for (int i=0; i < nEvents; ++i) {
	// 	cout << "i " << i << " eventNo " << eventNo << endl;
        eventTree->GetEntry(i);
        eventNo = i;
        

        while (eventNo == i && eventNo <nEvents) {
	  //	  cout << " count " << endl;
          hitTree->GetEntry(count);
	  
          if (eventNo != i) { continue;}
	  //cout << " count " << count << " pad " << padNo << " event " << eventNo << " i " << i << endl;
          char pulseName[100];
          sprintf(pulseName,"pulse%d",i);
          TH1F *pulseHisto = new TH1F(pulseName,pulseName,27,0,26);
	  
	  ++count;
          
          if (padNo == 346) {

            for (int k=0; k < 27 ; ++k) {

              pulseHisto->Fill(k,(*timeBinChargeVec1)[k]);

	    }

	    //	    cout << " rms " << pedRMS << endl;
	    if (maxCharge < 555 || maxCharge > 565) { delete pulseHisto; continue;}



            maxTBHisto->Fill(maxTB);
            meanTimeHisto->Fill(pulseHisto->GetMean());
	    //cout << " mean " << pulseHisto->GetMean() << endl;
          }
       
          
          delete pulseHisto; 
        }
        
      }
      delete eventTree;
      delete hitTree;
  }
      //       delete h1;
      //    delete h2;
    
  cout << " drawing " << endl;
  // for (int i=0; i<(int) meanTimeHisto.size(); ++i) {
  //  cout << " i " << i << endl;
      TCanvas *c4 = new TCanvas("c4","c4");
      meanTimeHisto->Draw();
      char fName[256];
      sprintf(fName, "../ExtPulserTiming/Fe55Pad346MeanTime2.root");
      char title[256];
      sprintf(title, "Average time bin for Fe55 Peripheral Hit; Time Bin; Counts");
      meanTimeHisto->SetTitle(title);  
      c4->SaveAs(fName);
      delete c4;  
    

    
    
      TCanvas *c4 = new TCanvas("c4","c4");
      maxTBHisto->Draw();
      char fName[256];
      sprintf(fName, "../ExtPulserTiming/Fe55Pad346MaxTB2b.root");
      char title[256];
      sprintf(title, "Max time bin for Fe55 Peripheral Hit; Time Bin; Counts");
      maxTBHisto->SetTitle(title);  
      c4->SaveAs(fName);
      delete c4;  
    
    return 0;
}
