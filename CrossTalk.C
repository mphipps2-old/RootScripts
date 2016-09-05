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
   

  vector<TH1F*> maxTBHisto;
  vector<TH1F*> meanTimeHisto;
  vector<TH1F*> leftCTHisto;
  vector<TH1F*> rightCTHisto;
  for (int z=5; z < 11; ++z) {
      char fileName[256];
      sprintf(fileName,"/data/mike/ExtPulserScan/ExtPulserScan/TreeExtPulserAmplitudeScanRun%d.root",z);
      TFile *file = TFile::Open(fileName);
      if (!file) {cerr << "file not open " << endl; return 1;}
      TTree *hitTree = new TTree("Hit","Hit");   
      TTree *eventTree = new TTree("Event","Event");   
      hitTree = (TTree*) file->Get("Hit;1");
      eventTree = (TTree*) file->Get("Event;1");
  
      Int_t nEvents = (Int_t) eventTree->GetEntries();

      char h3Name[256];
      sprintf(h3Name,"leftCrossTalk%dmV",z);
      TH1F *h3 = new TH1F(h3Name,h3Name,200,0 , 1);
      leftCTHisto.push_back(h3);
      char h4Name[256];
      sprintf(h4Name,"rightCrossTalk%dmV",z);
      TH1F *h4 = new TH1F(h4Name,h4Name,200,0 , 1);
      rightCTHisto.push_back(h4);
      
      
      int padNo = 0;
      hitTree->SetBranchAddress("PadNo",&padNo);
      float charge = 0;
      hitTree->SetBranchAddress("MaxCharge",&charge);
      int count = 0;
      int eventNo = 0;
      
      int tbCount = 0;
      int maxTB = 0;
      Float_t pedRMS = 0;

      hitTree->SetBranchAddress("EventNo",&eventNo);
      hitTree->SetBranchAddress("PedRMS",&pedRMS);
      for (int i=0; i < nEvents; ++i) {
	//	cout << "i " << i << " eventNo " << eventNo << endl;
        eventTree->GetEntry(i);
        eventNo = i;
        float leftCT1 = 0;
        float rightCT1 = 0;
        float primary = 0;
        float leftCT = 0;
        float rightCT = 0;

        while (eventNo == i && eventNo <nEvents) {
	  //cout << " count " << endl;
          hitTree->GetEntry(count);
	  
          if (eventNo != i) { continue;}
	  //cout << " count " << count << " pad " << padNo << " event " << eventNo << " i " << i << endl;

	  ++count;
          if (padNo == 80) {

            if (pedRMS > 10) leftCT1 = 0;
            else leftCT1 = charge;
          }
          else if (padNo == 81) {


	    //	    cout << " rms " << pedRMS << endl;
	    if (pedRMS > 10) {  primary = 0; rightCT1 = 0; rightCT1 = 0;continue;}

            primary = charge; 
          }
          else if (padNo == 82) {

            if (pedRMS > 10) rightCT1 = 0;
            else rightCT1 = charge;
          }
          
    
        }
        if (primary == 0) {leftCT = 0; rightCT = 0;}
        else { 

          leftCT = leftCT1/primary; rightCT = rightCT1/primary;
	  //	  cout << " z-5 " << z-5 << " leftCT " << leftCT << " right " << rightCT << endl;
          if (leftCT !=0) leftCTHisto[z-5]->Fill(leftCT);
	  
          if (rightCT != 0) rightCTHisto[z-5]->Fill(rightCT);
	  
        }
      }
      delete eventTree;
      delete hitTree;

      //       delete h1;
      //    delete h2;
    }
  cout << " drawing " << endl;
  
    for (int i=0; i<(int) leftCTHisto.size(); ++i) {
      TCanvas *c4 = new TCanvas("c4","c4");
      (leftCTHisto[i])->Draw();
      char fName[256];
      sprintf(fName, "../ExtPulserTiming/LeftCrossTalk%dmV.root",i+5);
      char title[256];
      sprintf(title, "Cross talk for left neighbor at %dmV; Fractional Charge; Counts",i+5);
      (leftCTHisto)[i]->SetTitle(title);  
      c4->SaveAs(fName);
      delete c4;  
    }
    for (int i=0; i<(int) rightCTHisto.size(); ++i) {
      TCanvas *c4 = new TCanvas("c4","c4");
      (rightCTHisto[i])->Draw();
      char fName[256];
      sprintf(fName, "../ExtPulserTiming/RightCrossTalk%dmV.root",i+5);
      char title[256];
      sprintf(title, "Cross talk for right neighbor at %dmV; Time Bin; Counts",i+5);
      (rightCTHisto)[i]->SetTitle(title);  
      c4->SaveAs(fName);
      delete c4;  
    }
    return 0;
}
