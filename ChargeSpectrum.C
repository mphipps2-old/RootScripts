#include <iostream>

using namespace std;

int main() {


  char fileName1[256];
char fileName2[256];
char fileName3[256];
 char fileName4[256];
 sprintf(fileName1,"../Trees/Trees/TreeScanMux1Imux20Run20.root");
 sprintf(fileName2,"../Trees/Trees/TreeScanMux2Imux29Run38.root");
 sprintf(fileName3,"../Trees/Trees/TreeScanMux3Imux56Run44.root");
 sprintf(fileName4,"../Trees/Trees/TreeScanMux4Imux3FRun36.root");
 
  TH1F *ch409Histos[4];
  
  for (int i=1; i< 5; ++i) {
    cout << "File: "<< i << endl;

    TFile *file;
    if (i == 1) file = TFile::Open(fileName1);
    else if (i == 2) file = TFile::Open(fileName2);
    else if (i == 3) file = TFile::Open(fileName3);
    else if (i == 4) file = TFile::Open(fileName4);
    if (!file) return 1;
    char hName[100];
    sprintf(hName,"ch409Mux%d",i);
    ch409Histos[i] =  new TH1F(hName,hName,112, 0, 2200);
   
    TTree *hitTree = new TTree("Hit","Hit");   
    hitTree = (TTree*) file->Get("Hit;1");
    //    cout << "1" << endl; 
    Int_t nEvents = (Int_t) hitTree->GetEntries();

    Int_t stripNo = 0;
    Float_t charge = 0;    
    hitTree->SetBranchAddress("PadNo",&stripNo);
    hitTree->SetBranchAddress("MaxCharge",&charge);
    for (Int_t j = 0; j < nEvents; ++j) {   

        hitTree->GetEntry(j);
	if (stripNo == 408/*&& stripNo < 415*/)	cout << "strip " << stripNo << " charge " << charge << endl;;
        if (stripNo == 408) ch409Histos[i]->Fill(charge);
    }
    TCanvas *c1 = new TCanvas("c1","c1");
    ch409Histos[i]->Draw();
    char fName[256];
    sprintf(fName, "../IntMux1/Channel409Mux%d.root",i);
    char title[256];
    sprintf(title, "Charge Spectrum Channel 409 Mux %d; ADC; Count",i);
    ch409Histos[i]->SetTitle(title);
    char name2[256];
      sprintf(name2,"../IntMux1/chargeSpectrumMux%d.root",i);  
      c1->SaveAs(name2);
    delete c1;
  }

  return 0;
}
