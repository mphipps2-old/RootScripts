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
   gROOT->ProcessLine(".L loader.C+");
      char fileName[256];
    sprintf(fileName,"/home/mphipps/rcdaqAnalysis/Trees/Trees/TreeScanMux4Run36.root");
    TFile *file = TFile::Open(fileName);
    if (!file) return 1;
    TF1 *f1 = new TF1("f1","1",0,26);
    TTree *hitTree = new TTree("Hit","Hit");   
    TTree *eventTree = new TTree("Event","Event");   
    hitTree = (TTree*) file->Get("Hit;1");
    eventTree = (TTree*) file->Get("Event;1");

   Int_t nEvents = (Int_t) eventTree->GetEntries();

    TH1F *h1 = new TH1F("h1","h1",27, -0.5, 26.5);
    TH1F *h2 = new TH1F("h2","h2",27, -0.5, 26.5);
    TH1F *h3 = new TH1F("h3","h3",27, -0.5, 26.5);

    vector<TH1F*> All;
    vector<Float_t> *timeBinChargeVec1 = 0;
    hitTree->SetBranchAddress("TimeBinChargeVec",&timeBinChargeVec1);
    
    int padNo = 0;
    hitTree->SetBranchAddress("PadNo",&padNo);
 int count = 0;
    int eventNo = 0;
    int fillCount = 0;
    float maxCharge = 0;
    hitTree->SetBranchAddress("EventNo",&eventNo);
    hitTree->SetBranchAddress("MaxCharge",&maxCharge);
    for (int i=0; i < nEvents; ++i) {
      //cout << " new EVENT " << endl;
      eventTree->GetEntry(i);
      eventNo = i;  
      while (eventNo == i && eventNo <nEvents) {
	hitTree->GetEntry(count);
	
	++count;
        if (padNo == 464 && maxCharge > 500 && maxCharge < 700) {
	  Float_t max1=0;
          
            char text[100];
            sprintf (text, "histo%d",i);
	    //	    cout <<" filling event " << i << endl;

		    // cout << " filled " << endl;
	    int firstTB =0;	    int maxTB=0;
	  for (int k=0; k < (int)(*timeBinChargeVec1).size() -1 ; ++k) {
	    if ((*timeBinChargeVec1)[k] > max1) {
              max1 = (*timeBinChargeVec1)[k];
	      maxTB = k;
	      if (firstTB==0) firstTB = k;
            }
	  }
          if (maxTB != 9 ) continue;
          //else if (firstTB != 7) continue;
	  cout << " firstTB " << firstTB << endl;
            TH1F *h =  new TH1F(text,text,27, -0.5, 26.5);
	   All.push_back(h);
	  for (int k=0; k < (int)(*timeBinChargeVec1).size() -1 ; ++k) {
	  

	    (*timeBinChargeVec1)[k] /= max1;
	    if ((*timeBinChargeVec1)[k] > 1) cout << " greater than 1 " << (*timeBinChargeVec1)[k] << endl;
            h1->Fill(k,(*timeBinChargeVec1)[k]);
	  
	    //	    cout << " fillcount " << fillCount << endl;
	    All[fillCount]->Fill(k,(*timeBinChargeVec1)[k]);
	  }
 ++fillCount;
        }
	else if (padNo != 464) continue;
        if (padNo == 13) {
          Float_t max2=0;
          
	  for (int k=0; k < (int)(*timeBinChargeVec1).size() -1; ++k) {
            if ((*timeBinChargeVec1)[k] > max2) max2 = (*timeBinChargeVec1)[k];	   
          
	  }
	  for (int k=0; k < (int)(*timeBinChargeVec1).size() -1; ++k) {
	    if (max2 == 0 ) continue;

	    (*timeBinChargeVec1)[k] /= max2;
            h2->Fill(k,(*timeBinChargeVec1)[k]);
          
	  }        
	}

        if (padNo == 21) {       
          Float_t max3=0;
	  for (int k=0; k < (int)(*timeBinChargeVec1).size() -1; ++k) {
            if ((*timeBinChargeVec1)[k] > max3) max3 = (*timeBinChargeVec1)[k];

	  }
	  for (int k=0; k < (int)(*timeBinChargeVec1).size() -1; ++k) {
	    if (max3 == 0 ) continue;

	    (*timeBinChargeVec1)[k] /= max3;
            h3->Fill(k,(*timeBinChargeVec1)[k]);
	  }
	}
       
      }
    }


    /*
    h1>Divide(f1,nEvents);
    h2->Divide(f1,nEvents);
    h3->Divide(f1,nEvents);
    
    */

    for (int i=0; i< 26; ++i) {
      if (h1->GetBinContent(i+1)!=0)  cout << " bin " << i+1 << " h1 " << h1->GetBinError(i+1) / h1->GetBinContent(i+1)<< endl;
    }
     
    TH1F *hInt = new TH1F("hInt","hInt",80,1.5,9.5);
    for (int i=0; i<(int)All.size(); ++i) {
      TF1 *f1 = new TF1("f1","pol1",5,9); 
      All[i]->Fit(f1,"R");
      TF1 *f = All[i]->GetFunction("f1");
      float intercept = f->GetParameter(0);
      float slope = f->GetParameter(1);
      float xInt = -intercept/slope;
     
      hInt->Fill(xInt);
      delete f1;
    }
    TCanvas *c4 = new TCanvas("c4","c4");
    hInt->Draw();
    char fName[256];
    sprintf(fName, "../IntMux4/RisingEdgeCh464Fe55.root");
    char title[256];
    sprintf(title, "Rising Edge for Ch 464; Time Bin; ADC");
    hInt->SetTitle(title);  
    c4->SaveAs(fName);   


    cout << " DRAWING " << endl;
    TCanvas *c = new TCanvas("c","c");
    char fName55[256];
      sprintf(fName55, "../IntMux4/PulseShapeCh464Fe55AllEvents.root");
      char title55[256];
      sprintf(title55, "All Fe55 Pulses for Ch 464; Time Bin; ADC");
      All[0]->SetTitle(title55);  
      for (int m=0; m < All.size(); ++m) {     
	//      cout << " m " << m << endl;
      if (m == 0) All[0]->Draw();
       else All[m]->Draw("same");
    }
          c->SaveAs(fName55);
    
      TCanvas *c1 = new TCanvas("c1","c1");
    h1->Draw();
    char fName[256];
    sprintf(fName, "../IntMux4/PulseShapeCh464Fe55.root");
    char title[256];
    sprintf(title, "Avg Fe55 Pulse Shape for Ch 464; Time Bin; ADC");
    h1->SetTitle(title);  
    c1->SaveAs(fName);
    /*
    TCanvas *c2 = new TCanvas("c2","c2");
    h2->Draw();
    char fName2[256];
    sprintf(fName2, "../IntMux4/PulseShapeCh2.root");
    char title2[256];
    sprintf(title2, "Avg Pulse Shape for Ch2 at ; Time Bin; ADC");
    h2->SetTitle(title2);  
    c2->SaveAs(fName2);

    TCanvas *c3 = new TCanvas("c3","c3");
    h3->Draw();
    char fName3[256];
    sprintf(fName3, "../IntMux4/PulseShapeCh3.root");
    char title3[256];
    sprintf(title3, "Avg Pulse Shape for Ch3 at ; Time Bin; ADC");
    h3->SetTitle(title3);  
    c3->SaveAs(fName3);
    */
    
    
    
    
    
    


    /*
    TCanvas *c5 = new TCanvas("c5","c5");
    200h5->Draw();
    char fName5[256];
    sprintf(fName5, "../IntMux4/PulseShape200Ch2.root");
    char title5[256];
    sprintf(title5, "Avg Pulse Shape for Ch2 at 200; Time Bin; ADC");
    200h5->SetTitle(title5);  
    c5->SaveAs(fName5);

    TCanvas *c6 = new TCanvas("c6","c6");
    200h6->Draw();
    char fName6[256];
    sprintf(fName6, "../IntMux4/PulseShape200Ch3.root");
    char title6[256];
    sprintf(title6, "Avg Pulse Shape for Ch3 at 200; Time Bin; ADC");
    200h6->SetTitle(title6);  
    c6->SaveAs(fName6);
    */
    delete c1; delete c4; 
    //delete c2; delete c3; delete c4; delete c5; delete c6;
    return 0;
}
