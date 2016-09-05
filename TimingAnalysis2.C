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

  for (int z=5; z < 6; ++z) {
      char fileName[256];
      sprintf(fileName,"/data/mike/ExtPulserScan/ExtPulserScan/TreeTimingJitterExtPulserRun80230.root",z);
      TFile *file = TFile::Open(fileName);
      if (!file) {cerr << "file not open " << endl; return 1;}
      TTree *hitTree = new TTree("Hit","Hit");   
      TTree *eventTree = new TTree("Event","Event");   
      hitTree = (TTree*) file->Get("Hit;1");
      eventTree = (TTree*) file->Get("Event;1");
  
      Int_t nEvents = (Int_t) eventTree->GetEntries();
      char h1Name[256];
      sprintf(h1Name,"maxTB%d",z);
      TH1F *h1 = new TH1F(h1Name,h1Name,27, -0.5, 26.5);
      maxTBHisto.push_back(h1); 
      char h2Name[256];
      sprintf(h2Name,"meanTime%d",z);
      TH1F *h2 = new TH1F(h2Name,h2Name,115,10 , 18);
      meanTimeHisto.push_back(h2);
     
      vector<Float_t> *timeBinChargeVec1 = 0;
      hitTree->SetBranchAddress("TimeBinChargeVec",&timeBinChargeVec1);
      int padNo = 0;
      hitTree->SetBranchAddress("PadNo",&padNo);
     
     
      int count = 0;
      int eventNo = 0;
      
      int tbCount = 0;
      int maxTB = 0;
      Float_t pedRMS = 0;
      int nTimeBins = 0;
      hitTree->SetBranchAddress("TimeSize",&nTimeBins);
      hitTree->SetBranchAddress("MaxTimeBin",&maxTB);
      hitTree->SetBranchAddress("EventNo",&eventNo);
      hitTree->SetBranchAddress("PedRMS",&pedRMS);
      int plotCount = 0;

      for (int i=0; i < nEvents; ++i) {

        eventTree->GetEntry(i);
        eventNo = i;


        while (eventNo == i && eventNo <nEvents) {

          hitTree->GetEntry(count);

          if (eventNo != i) { continue;}
	  //	    cout << " count " << count << " pad " << padNo << " event " << eventNo << " i " << i << endl;
          char pulseName[100];
          sprintf(pulseName,"pulse%d",i);
          TH1F *pulseHisto = new TH1F(pulseName,pulseName,27,0,26);
	  
	  ++count;
          
          if (padNo == 81) {

            for (int k=0; k < 27 ; ++k) {

              pulseHisto->Fill(k,(*timeBinChargeVec1)[k]);

	    }

	    //	    cout << " rms " << pedRMS << endl;
	    if (z == 5  && (pedRMS > 10/* || nTimeBins < 10 || nTimeBins > 16)*/)) { delete pulseHisto; continue;}
            if (z == 6  && (pedRMS > 10 || nTimeBins < 10 || nTimeBins > 16)) { delete pulseHisto; continue;}
            if (z == 7  && (pedRMS > 10 || nTimeBins < 11|| nTimeBins > 16)) { delete pulseHisto; continue;}
            if (z == 8  && (pedRMS > 10 || nTimeBins < 11|| nTimeBins > 17)) { delete pulseHisto; continue;}
            if (z == 9  && (pedRMS > 10 || nTimeBins < 12 || nTimeBins > 17)) { delete pulseHisto; continue;}
            if (z == 10  && (pedRMS > 10 || nTimeBins < 12|| nTimeBins > 17)) { delete pulseHisto; continue;}



            maxTBHisto[z-5]->Fill(maxTB);

            meanTimeHisto[z-5]->Fill(pulseHisto->GetMean());

            meanTimeHisto[z-5]->Fill(pulseHisto->GetMean());
            if (z == 9 && pulseHisto->GetMean() > 13.8 && pulseHisto->GetMean() < 14.5  && plotCount < 5) {     
      TCanvas *c4 = new TCanvas("c4","c4");
      pulseHisto->Draw();
      char fName[256];
      sprintf(fName, "../ExtPulserTiming/pulseHistoPeak%dmV%d.root",i+5,plotCount);
      c4->SaveAs(fName);
      delete c4;  
          }

            if (z == 9 && pulseHisto->GetMean() > 15.5 && plotCount < 10) {     
      TCanvas *c4 = new TCanvas("c4","c4");
      pulseHisto->Draw();
      char fName[256];
      sprintf(fName, "../ExtPulserTiming/pulseHisto%dmV%d.root",i+5,plotCount);
      c4->SaveAs(fName);
      delete c4;  
      ++plotCount;
	    //	    cout << " mean " << pulseHisto->GetMean() << endl;
	    }
       
          }
          delete pulseHisto; 
	  }
        
      }
      delete eventTree;
      delete hitTree;

      //       delete h1;
      //    delete h2;
      
  }
  cout << " drawing " << endl;
    for (int i=0; i<(int) meanTimeHisto.size(); ++i) {
      cout << " i " << i << endl;
      TCanvas *c4 = new TCanvas("c4","c4");
      (meanTimeHisto[i])->Draw();
      char fName[256];
      sprintf(fName, "../ExtPulserTiming/NewExtPulserMeanTime%d.root",i+5);
      char title[256];
      sprintf(title, "Average time bin for %dmV input; Time Bin; Counts",i+5);
      meanTimeHisto[i]->SetTitle(title);  
      c4->SaveAs(fName);
      delete c4;  
    }

    for (int i=0; i<(int) maxTBHisto.size(); ++i) {
      cout << " i " << endl;
      TCanvas *c4 = new TCanvas("c4","c4");
      (maxTBHisto[i])->Draw();
      char fName[256];
      sprintf(fName, "../ExtPulserTiming/NewExtPulserMaxTB%d.root",i+5);
      char title[256];
      sprintf(title, "Max time bin for %dmV input; Time Bin; Counts",i+5);
      maxTBHisto[i]->SetTitle(title);  
      c4->SaveAs(fName);
      delete c4;  
    }
    return 0;
  }
