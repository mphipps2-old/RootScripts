// To run script, first compile with: g++ `root-config --cflags` srsCalib.C `root-config --glibs`. Then execute with ./a.out

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
#include "TLatex.h"
#include "TPad.h"
//#include "TGAxis.h"
using namespace std;


int main() {

 ofstream file[4];
 file[0].open("IntValuesMux2.txt", ofstream::out | ofstream::trunc);
 file[0] << "#Input,     ADC#" << endl;
 file[0] << "######################## " << endl << endl;

 file[1].open("IntValuesMux4.txt", ofstream::out | ofstream::trunc);
 file[1] << "#Input,     ADC#" << endl;
 file[1] << "######################## " << endl << endl;

 file[2].open("ExtValuesMux2.txt", ofstream::out | ofstream::trunc);
 file[2] << "#Input,     ADC#" << endl;
 file[2] << "######################## " << endl << endl;

 file[3].open("ExtValuesMux4.txt", ofstream::out | ofstream::trunc);
 file[3] << "#Input,     ADC#" << endl;
 file[3] << "######################## " << endl << endl;

 int firstFile;
 int lastFile;
 int inc;
 for (int i=0; i < 4; ++i) {
   cout << " i " << i << endl;
   if (i==0 || i == 1) {firstFile=10; lastFile = 255; inc=1;}
   else if (i == 2) {firstFile=100; lastFile=950; inc=50;}
   else {firstFile=100;lastFile=700; inc=50;} 
   for (int j=firstFile; j<lastFile; j+=inc) {
     cout << " file " << j  << endl;
    char fileName[256];
    if (i==0) sprintf(fileName,"Trees/SRSCalibRun%d.root",j);
    else if (i==1) sprintf(fileName,"Trees/SRSCalibMux4Run%d.root",j);
    else if (i==2) sprintf(fileName,"/home/mphipps/rcdaqAnalysisBackup/rcdaqAnalysis/TreesExtMux2/SRSCalibRun%d.root",j);
   else if (i==3) sprintf(fileName,"/home/mphipps/rcdaqAnalysisBackup/rcdaqAnalysis/TreesExtMux4/Trees/SRSCalibMux4Run%d.root",j);
    TFile *file2 = TFile::Open(fileName);
    if (!file2) return 1;

  TTree *hitTree = new TTree("Hit","Hit");   
    hitTree = (TTree*) file2->Get("Hit;1");
    //    cout << "1" << endl; 
    Int_t nEvents = (Int_t) hitTree->GetEntries();

    Int_t stripNo = 0;
    Float_t charge = 0;    
  TH1F *histoArray[480];
    hitTree->SetBranchAddress("PadNo",&stripNo);
    hitTree->SetBranchAddress("MaxCharge",&charge);
    for (int k = 0; k < 480; ++k) {
      char hName[100];
      sprintf(hName,"ch%d",k);
      histoArray[k]= new TH1F(hName,hName,112, 0, 2200);  
    }   
    for (Int_t k = 0; k < nEvents; ++k) {   

        hitTree->GetEntry(k);

       histoArray[stripNo]->Fill(charge);
       //  cout << stripNo << " charge " << charge << endl;
    }
    int z =0;
    for (int k=0; k<480; ++k) {
       if (histoArray[k]->GetEntries() < 500) continue;
       if (k == 13 ){ file[i] << j << "         " << histoArray[k]->GetMean() << endl; }
       ++z;
    }
   for (int k=0; k < 480; ++k) delete histoArray[k];
   delete hitTree;
    file2->Close();
   }
   file[i].close();
 }
  return 1;
 }
