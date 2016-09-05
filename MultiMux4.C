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

  TMultiGraph *mg = new TMultiGraph();
  // TMultiGraph *mg2 = new TMultiGraph();

// TFile *file = TFile::Open("ExtPulserMux2/Mux2Calibration.root");
  // if (!file) return 1;
  // TFile *file2 = TFile::Open("ExtPulserMux4/Mux4Calibration.root");
  // if (!file2) return 1;
/*
 ofstream file1b;
 file1b.open("../IntValuesMux2.txt", ofstream::out | ofstream::trunc);
 file1b << "#Input,     ADC#" << endl;
 file1b << "######################## " << endl << endl;

ofstream file2b;
 file2b.open("IntValuesMux4.txt", ofstream::out | ofstream::trunc);
 file2b << "#Input,     ADC#" << endl;
 file2b << "######################## " << endl << endl;
*/
 
 Int_t N2; 
 Int_t N4;
 Double_t *X2;
 Double_t *Y2;
 
 Double_t *X4;
 Double_t *Y4;


 Int_t N6; 
 Int_t N8;
 Double_t *X6;
 Double_t *Y6;
 
 Double_t *X8;
 Double_t *Y8;

   TFile *file = TFile::Open("/data/IntPulserMux4/Mux4Imux22Calibration.root");
   if (!file) return 1;
    TFile *file2 = TFile::Open("/home/mphipps/rcdaqAnalysis/IntMux4/Mux4Imux15Calibration.root");
   if (!file2) return 1;

   TFile *file3 = TFile::Open("/home/mphipps/rcdaqAnalysis/IntMux1/Mux1Imux20Calibration.root");
   if (!file3) return 1;
    TFile *file4 = TFile::Open("/home/mphipps/rcdaqAnalysis/IntMux5/Mux5Imux46Calibration.root");
   if (!file4) return 1;



   TCanvas *c1 = (TCanvas*) file->Get("c2;1");
  TCanvas *c2 = (TCanvas*) file2->Get("c2;1");
  // TCanvas *c3 = (TCanvas*) file->Get("c2;1");
  // TCanvas *c4 = (TCanvas*) file2->Get("c2;1");

   TCanvas *c3 = (TCanvas*) file3->Get("c2;1");
  TCanvas *c4 = (TCanvas*) file4->Get("c2;1");
  // TCanvas *c3 = (TCanvas*) file->Get("c2;1");
  // TCanvas *c4 = (TCanvas*) file2->Get("c2;1");

  TGraphErrors *g3 = (TGraphErrors*) c3->GetPrimitive("Graph");
  TGraphErrors *g4 = (TGraphErrors*) c4->GetPrimitive("Graph");
  X6 = g3->GetX();
  Y6 = g3->GetY();
  N6= g3->GetN();
  X8 = g4->GetX();
  Y8 = g4->GetY();
  N8= g4->GetN();
  TGraphErrors *g1 = (TGraphErrors*) c1->GetPrimitive("Graph");
  TGraphErrors *g2 = (TGraphErrors*) c2->GetPrimitive("Graph");
  X2 = g1->GetX();
  Y2 = g1->GetY();
  N2= g1->GetN();
  X4 = g2->GetX();
  Y4 = g2->GetY();
  N4= g2->GetN();
  /*
  for (int i = 0; i < N4; ++i) {
    X4[i] = (0.0153159 * X4[i]) + 52.9296;
    X2[i] = (0.0153159 * X2[i]) + 52.9296;
  }
  */
  /*
  for (int i=0; i<N2; ++i) {
    file1b << X2[i] << "        " << Y2[i] << endl;
  }

  for (int i=0; i<N4; ++i) {
    file2b << X2[i] << "        " << Y4[i] << endl;
  }
  */
    g1->SetMarkerStyle(20);
  g1->SetMarkerColor(4);
  g1->SetLineColor(4);
 g2->SetMarkerStyle(21);
  g2->SetMarkerColor(2);
 g2->SetLineColor(2);
  g3->SetMarkerStyle(22);
  g3->SetMarkerColor(8);
  g3->SetLineColor(8);
 g4->SetMarkerStyle(23);
  g4->SetMarkerColor(6);
 g4->SetLineColor(6);
  // TGraphErrors *g3 = (TGraphErrors*) c4->GetPrimitive("Graph");
  // TGraphErrors *g4 = (TGraphErrors*) c4->GetPrimitive("Graph");
  TCanvas *c = new TCanvas("c","c");
  /*
   TPad *pad1 = new TPad("pad1","",0,0,1,1);
   TPad *pad2 = new TPad("pad2","",0,0,1,1);
   pad2->SetFillStyle(4000); //will be transparent
   pad1->Draw();
   pad1->cd();
cout << " 2 " << endl;
  */
  mg->Add(g1);
  mg->Add(g2);
  mg->Add(g3);
  mg->Add(g4);
  
  mg->Draw("apl");
  //mg->SetTitle("SRS Calibration: Internal Pulser; Corrected ADC; Avg Pedestal-Subtracted ADC");

  mg->SetTitle("SRS Calibration: Internal Pulser; N_{electrons}; Avg ADC");

  //mg->SetTitle("SRS Calibration: Internal Pulser; Input Pulse (mV); Avg ADC");
 c->Update();
  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();
  TLegend *leg = new TLegend(0.46,0.7,0.7,0.9);
  leg->SetTextFont(72);
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
  leg->AddEntry(g1,"Muxgain 4; Imuxin 0x22","ple");
  leg->AddEntry(g2,"Muxgain 4; Imuxin 0x15: (New)","ple");
  leg->AddEntry(g3,"Muxgain 1; Imuxin 0x20","ple");
  leg->AddEntry(g4,"Muxgain 5; Imuxin 0x46","ple");
  leg->Draw();
 c->cd();
 /*
   //compute the pad range with suitable margins
   Double_t ymin = 0;
   Double_t ymax = 2000;
   Double_t dy = (ymax-ymin)/0.8; //10 per cent margins top and bottom
   Double_t xmin = -3;
   Double_t xmax = 3;
   Double_t dx = (xmax-xmin)/0.8; //10 per cent margins left and right
   pad2->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
   pad2->Draw();
   pad2->cd();
  //mg2->SetTitle("SRS Calibration; N_{electrons}; Avg ADC");
  mg2->Add(g3);
  mg2->Add(g4);
  mg2->Draw("][same");
 mg->SetTitle("SRS Calibration; N_{electrons}; Avg ADC");
 c->Update();

  Double_t low = 50;
  Double_t high = 950;
TGaxis *axis = new TGaxis(gPad->GetUxmin(),gPad->GetUymax(),gPad->GetUxmax(),gPad->GetUymax(),low,high,510,"+");
axis->Draw();

  TLegend *leg2 = new TLegend(0.12,0.45,0.7,0.9);
  leg2->SetTextFont(72);
  leg2->SetTextSize(0.04);
  leg2->SetFillColor(0);
  leg2->AddEntry(g3,"Mux 2","ple");
  leg2->AddEntry(g4,"Mux 4","ple");
  leg2->Draw();
*/
  char mgName[256];
  sprintf(mgName,"../IntMux1/calibrationMux1Mux4Mux5.root");
  c->SaveAs(mgName); 

  //  file1b.close();
  //file2b.close();
  return 0;
}
