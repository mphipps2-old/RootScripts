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
    sprintf(fileName,"/home/mphipps/rcdaqAnalysis/ExtMux4/TreeScanMux4EXTRun275.root");
    TFile *file = TFile::Open(fileName);
    if (!file) return 1;
    TF1 *f1 = new TF1("f1","1",0,26);
    TTree *hitTree = new TTree("Hit","Hit");   
    TTree *eventTree = new TTree("Event","Event");   
    hitTree = (TTree*) file->Get("Hit;1");
    eventTree = (TTree*) file->Get("Event;1");

      char fileName2[256];
    sprintf(fileName2,"/home/mphipps/rcdaqAnalysis/TreesMux4/TreeRun200.root");
   
    TFile *file2 = TFile::Open(fileName2);
    if (!file2) return 1;
    TTree *hitTree2 = new TTree("Hit2","Hit2");  
    //    TTree *eventTree2 = new TTree("Event2","Event2");   
    hitTree2 = (TTree*) file2->Get("Hit;1");
    // eventTree2 = (TTree*) file2->Get("Event;1");

    //    cout << "1" << endl; 
    Int_t nEvents = (Int_t) eventTree->GetEntries();

    TH1F *ical60h1 = new TH1F("ical60h1","ical60h1",27, -0.5, 26.5);
    TH1F *ical60h2 = new TH1F("ical60h2","ical60h2",27, -0.5, 26.5);
    TH1F *ical60h3 = new TH1F("ical60h3","ical60h3",27, -0.5, 26.5);
    TH1F *ical200h4 = new TH1F("ical200h4","ical200h4",27, -0.5, 26.5);
    TH1F *ical200h5 = new TH1F("ical200h5","ical200h5",27, -0.5, 26.5);
    TH1F *ical200h6 = new TH1F("ical200h6","ical200h6",27, -0.5, 26.5);
    vector<TH1F*> icalAll;
    vector<Float_t> *timeBinChargeVec1 = 0;
    hitTree->SetBranchAddress("TimeBinChargeVec",&timeBinChargeVec1);
    vector<Float_t> *timeBinChargeVec2 = 0;
    hitTree2->SetBranchAddress("TimeBinChargeVec",&timeBinChargeVec2);
    int padNo = 0;
    hitTree->SetBranchAddress("PadNo",&padNo);
 count = 0;
    int eventNo = 0;
    int fillCount = 0;
    int tbCount = 0;
    hitTree->SetBranchAddress("EventNo",&eventNo);
    for (int i=0; i < nEvents; ++i) {
      //cout << " new EVENT " << endl;
      eventTree->GetEntry(i);
      eventNo = i;
      
      while (eventNo == i && eventNo <nEvents) {
	hitTree->GetEntry(count);
	hitTree2->GetEntry(count);
	++count;
	//	cout << " event no " << eventNo << " count " << count << " nEvents " << i << endl;
	//	if (j ==0 )cout << "padNo " << padNo << " charge " << (*timeBinChargeVec1)[4]; 
	//if (padNo > 30) continue;
        if (padNo == 19) {
	  Float_t max1=0;
          Float_t max4=0;
            char text[100];
            sprintf (text, "histo%d",i);
	 	    cout <<" filling event " << i << endl;
TH1F *h =  new TH1F(text,text,27, -0.5, 26.5);
//if (i != 0) ++fillCount;
		    icalAll.push_back(h);
 cout << " filled " << endl;
 int maxTB = 0;
	  for (int k=0; k < (int)(*timeBinChargeVec1).size() -1 ; ++k) {
	    if ((*timeBinChargeVec1)[k] > max1) {max1 = (*timeBinChargeVec1)[k];maxTB=k;}
	    if ((*timeBinChargeVec2)[k] > max4) max4 = (*timeBinChargeVec2)[k];
	    // cout << "k " << k << " hit " << (*timeBinChargeVec2)[k] << endl;
	  }
          
	  for (int k=0; k < (int)(*timeBinChargeVec1).size() -1 ; ++k) {
	    //	    if (max1 == 0 ) {cout << " continuing " << endl; continue;}
	    
	    //   (*timeBinChargeVec1)[k] /= max1;
            if (maxTB==7) { 
              ical60h1->Fill(k,(*timeBinChargeVec1)[k]);
              if (k ==0 ) ++tbCount;
            }
	   	    cout << " filling " << i << " time " << k <<  " charge " << (*timeBinChargeVec1)[k] <<endl;
	    cout << " fillcount " << fillCount << endl;
	    icalAll[fillCount]->Fill(k,(*timeBinChargeVec1)[k]);
	    (*timeBinChargeVec2)[k] /= max4;
	    //	    cout << " ical60 " << (*timeBinChargeVec1)[k] << endl;
	    ical200h4->Fill(k,(*timeBinChargeVec2)[k]);
            
	    //	    cout << " k " << k << " adc " << (*timeBinChargeVec2)[k] << endl;
	  }
	  //ical60h1->Divide(f1,count);
++fillCount;
        }
	else if (padNo != 19) continue;
        if (padNo == 13) {
          Float_t max2=0;
          Float_t max5=0;
	  for (int k=0; k < (int)(*timeBinChargeVec1).size() -1; ++k) {
            if ((*timeBinChargeVec1)[k] > max2) max2 = (*timeBinChargeVec1)[k];	   
            if ((*timeBinChargeVec2)[k] > max5) max5 = (*timeBinChargeVec2)[k];            
	  }
	  for (int k=0; k < (int)(*timeBinChargeVec1).size() -1; ++k) {
	    if (max2 == 0 ) continue;

	    (*timeBinChargeVec1)[k] /= max2;
            ical60h2->Fill(k,(*timeBinChargeVec1)[k]);
	    (*timeBinChargeVec2)[k] /= max5;
            ical200h5->Fill(k,(*timeBinChargeVec2)[k]);
	  }        
          //ical60h2->Divide(f1,count);
	}

        if (padNo == 21) {       
          Float_t max3=0;
          Float_t max6=0;
	  for (int k=0; k < (int)(*timeBinChargeVec1).size() -1; ++k) {
            if ((*timeBinChargeVec1)[k] > max3) max3 = (*timeBinChargeVec1)[k];
            if ((*timeBinChargeVec2)[k] > max6) max6= (*timeBinChargeVec2)[k];
	  }
	  for (int k=0; k < (int)(*timeBinChargeVec1).size() -1; ++k) {
	    if (max3 == 0 ) continue;

	    (*timeBinChargeVec1)[k] /= max3;
            ical60h3->Fill(k,(*timeBinChargeVec1)[k]);

	    (*timeBinChargeVec2)[k] /= max6;
            ical200h6->Fill(k,(*timeBinChargeVec2)[k]);
	  }
          //ical60h3->Divide(f1,count);
	}
       
      }
    }
    
        ical60h1->Divide(f1,tbCount);
	/*
    ical60h2->Divide(f1,nEvents);
    ical60h3->Divide(f1,nEvents);
    ical200h4->Divide(f1,nEvents);
    ical200h5->Divide(f1,nEvents);
    ical200h6->Divide(f1,nEvents);
    */


    TH1F *hInt = new TH1F("hInt","hInt",60,-1.5,1.5);
    vector<TH1F*> allRMS;
    for (int i=0; i<10; ++i) {
      char bName[50];
      sprintf(bName,"hRMS%d",i);
      hRMS = new TH1F(bName,bName,1000,0,0.5);
      allRMS.push_back(hRMS);
    }
    /*
    for (int i=0; i<(int)icalAll.size(); ++i) {
      for (int j=0; j < 10; ++j) {
        allRMS[j]->Fill(icalAll[i]->GetBinContent(j+1));
      }
      TF1 *f1 = new TF1("f1","pol1",0.5,3); 
      icalAll[i]->Fit(f1,"R");
      TF1 *f = icalAll[i]->GetFunction("f1");
      float intercept = f->GetParameter(0);
      float slope = f->GetParameter(1);
      float xInt = -intercept/slope;
     
      hInt->Fill(xInt);
      delete f1;
    }
    */
    for (int i = 0; i<10; ++i) {
      cout << "tb " << i << " rms " <<  allRMS[i]->GetRMS() << endl;
    TCanvas *c4 = new TCanvas("c4","c4");
    hInt->Draw();
    char fName[256];
    sprintf(fName, "../ExtMux4/RMSCh464ExtOverlayBin%d.root",i);
    char title[256];
    sprintf(title, "Rising Edge for Ch 464; Time Bin; ADC");
    hInt->SetTitle(title);  
    c4->SaveAs(fName);
    delete c4;   
    }
    TCanvas *c4 = new TCanvas("c4","c4");
    hInt->Draw();
    char fName[256];
    sprintf(fName, "../ExtMux4/RisingEdgeCh464Ext.root");
    char title[256];
    sprintf(title, "Rising Edge for Ch 464; Time Bin; ADC");
    hInt->SetTitle(title);  
    c4->SaveAs(fName);   
    delete c4;

    cout << " DRAWING " << endl;
    
    for (int m=0; m < 10; ++m) {     
	//      cout << " m " << m << endl;
      //if (m == 0) icalAll[0]->Draw();
      //else icalAll[m]->Draw("same");
TCanvas *c = new TCanvas("c","c");
    char fName55[256];
    sprintf(fName55, "../ExtMux4/PulseShape275mVCh19Event%d.root",m);
      char title55[256];
      sprintf(title55, "Pulse %d Ch19 Ext Pulser 275 mV; Time Bin; ADC",m);
      icalAll[m]->SetTitle(title55);  
      icalAll[m]->Draw();
      c->SaveAs(fName55);
      delete c;
    }

    cout << " drawing icalAll " << endl;
TCanvas *cAll = new TCanvas("cAll","cAll");
    for (int m=0; m < icalAll.size(); ++m) {     
    cout << " m " << m << endl;
    if (m == 0) {
      icalAll[0]->Draw();
      char title55[256];
      sprintf(title55, "All Pulses: Ext Pulser Ch 19 at 275 mV; Time Bin; ADC",m);
      icalAll[0]->SetTitle(title55);  
    }
    else icalAll[m]->Draw("same");
    }
   

    char fName55[256];
    sprintf(fName55, "../ExtMux4/AllPulses275mVCh19.root",m);
      
      //      icalAll[m]->Draw();
      cAll->SaveAs(fName55);
      delete cAll;
   




    
      TCanvas *c10 = new TCanvas("c10","c10");
    ical60h1->Draw();
    char fName[256];
    sprintf(fName, "../ExtMux4/PulseShape275mVCh19MaxTB7.root");
    char title[256];
    sprintf(title, "Avg Pulse Shape External Pulser for Ch19: Max time bin 7 ; Time Bin; ADC");
    ical60h1->SetTitle(title);  
    c10->SaveAs(fName);
    /*
    TCanvas *c2 = new TCanvas("c2","c2");
    ical60h2->Draw();
    char fName2[256];
    sprintf(fName2, "../ExtMux4/PulseShape275mVCh2.root");
    char title2[256];
    sprintf(title2, "Avg Pulse Shape for Ch2 at ICal60; Time Bin; ADC");
    ical60h2->SetTitle(title2);  
    c2->SaveAs(fName2);

    TCanvas *c3 = new TCanvas("c3","c3");
    ical60h3->Draw();
    char fName3[256];
    sprintf(fName3, "../ExtMux4/PulseShape275mVCh3.root");
    char title3[256];
    sprintf(title3, "Avg Pulse Shape for Ch3 at ICal60; Time Bin; ADC");
    ical60h3->SetTitle(title3);  
    c3->SaveAs(fName3);
    */
    TCanvas *c4 = new TCanvas("c4","c4");
    ical200h4->Draw();
    char fName4[256];
    sprintf(fName4, "../ExtMux4/PulseShapeIcal200Ch1.root");
    char title4[256];
    sprintf(title4, "Avg Pulse Shape for Ch1 at ICal200; Time Bin; ADC");
    ical200h4->SetTitle(title4);  
    c4->SaveAs(fName4);
    /*
    TCanvas *c5 = new TCanvas("c5","c5");
    ical200h5->Draw();
    char fName5[256];
    sprintf(fName5, "../ExtMux4/PulseShapeIcal200Ch2.root");
    char title5[256];
    sprintf(title5, "Avg Pulse Shape for Ch2 at ICal200; Time Bin; ADC");
    ical200h5->SetTitle(title5);  
    c5->SaveAs(fName5);

    TCanvas *c6 = new TCanvas("c6","c6");
    ical200h6->Draw();
    char fName6[256];
    sprintf(fName6, "../ExtMux4/PulseShapeIcal200Ch3.root");
    char title6[256];
    sprintf(title6, "Avg Pulse Shape for Ch3 at ICal200; Time Bin; ADC");
    ical200h6->SetTitle(title6);  
    c6->SaveAs(fName6);
    */
    delete c4; 
    //delete c2; delete c3; delete c4; delete c5; delete c6;
    return 0;
}
