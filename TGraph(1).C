#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

int main() {


TString name[8] = {"Ba133", "Cd109", "Co57", "Co60", "Cs137", "Mn54", "Na22", "Unknown"};
TString bName[8] = {"Ba133Bg", "Cd109Bg", "Co57Bg", "Co60Bg", "Cs137Bg", "Mn54Bg", "Na22Bg", "UnknownBg"};
TString ext1 = ".root";  
TString ext2 = ".png";
TString ext3 = ".txt";



for (int n = 0; n < 8; ++n) {
  TCanvas *c = new TCanvas(name[n],name[n], 1300,1300);
/*
  gStyle->SetStatH(0.4);
  gStyle->SetStatW(0.15);
  gStyle->SetOptFit(111);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(000000);

*/


  vector<double> e, bE;
  vector<double> count, bCount, adjustedCount;
  vector<double> yErrors, sigma;
  double pE, pCount;
  TString fileName = name[n] + ext3;
  fstream fin(fileName,ios::in);
  if(!fin){cout<<"file 1 not read "<< fileName.Data() << endl; return 0;}
  Int_t it = 0;
  while(fin>>pE>>pCount) { 
   e.push_back(pE); 
   count.push_back(pCount);
++it;
  }
  fin.close();
  TString fileName2 = bName[n] + ext3;
  fstream fin2(fileName2,ios::in);
  if(!fin2){cout<<"file 2 not read "<< fileName2.Data() << endl; return 0;}
  it = 0;
  while(fin2>>pE>>pCount) { 

   bE.push_back(pE); 
   bCount.push_back(pCount);
++it;
  }
  fin2.close();
  for (int i = 0; i <1024; ++i) {
    Double_t errorB = TMath::Sqrt(bCount[i])/60;
    Double_t errorC = TMath::Sqrt(count[i])/60;
    Double_t countB = bCount[i] / 60;
    Double_t countC = count[i] / 60;
    if (errorB != 0 || errorC != 0) sigma.push_back(TMath::Sqrt( errorC*errorC + errorB*errorB));
    else sigma.push_back(0);
    adjustedCount.push_back(countC - countB);
  }
  TGraphErrors *myGraph = new TGraphErrors(1024, &e[0], &adjustedCount[0], NULL, &sigma[0] );
  char title[50];
  sprintf(title,"%s Spectrum from Multi-Channel Analyzer", name[n].Data());
  myGraph->SetTitle(title);
  myGraph->GetXaxis()->SetTitle("Energy (keV)");
  myGraph->GetYaxis()->SetTitle("Frequency (Hz)");
  myGraph->GetXaxis()->CenterTitle();
  myGraph->GetYaxis()->CenterTitle();
  myGraph->SetMarkerStyle(6);
  myGraph->SetMarkerSize(1);
  myGraph->Draw("AP");

  TPaveText *t;
  TF1 *f[6];
  if (n == 0) { // Ba133 
    t = new TPaveText(.80,.3,.98,.95,"brNDC");
    t->SetFillColor(0);
    t->SetShadowColor(0);
    f[0] = new TF1("g1","gaus",23,38);
    f[1] = new TF1("g2","gaus",80,90);
    f[2] = new TF1("g3","gaus",115,124);
    f[3] = new TF1("g4","gaus",290,316);
    f[4] = new TF1("g5","gaus", 340,380);
    f[5] = new TF1("g6","gaus", 425,450);
    myGraph->Fit(f[0], "R");
    char text0[50];
    char text1[50];
    char text2[50];
    char text3[50];
    t->AddText("Peak 1");  
    sprintf(text1, "#chi^{2}  %.5g",  f[0]->GetChisquare() / f[0]->GetNDF());
    t->AddText(text1);
    sprintf(text0, "Constant %.5g #pm %.5g", f[0]->GetParameter(0),f[0]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean  %.5g #pm %.5g", f[0]->GetParameter(1),f[0]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma  %.5g #pm %.5g", f[0]->GetParameter(2),f[0]->GetParError(2)); 
    t->AddText(text3);
    myGraph->Fit(f[1], "R+");
    t->AddText("");
    t->AddText("Peak 2");  
    sprintf(text1, "#chi^{2}  %.5g", f[1]->GetChisquare() / f[1]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant %.5g #pm %.5g", f[1]->GetParameter(0),f[1]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean  %.5g #pm %.5g", f[1]->GetParameter(1),f[1]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma  %.5g #pm %.5g", f[1]->GetParameter(2),f[1]->GetParError(2)); 
    t->AddText(text3);
    myGraph->Fit(f[2], "R+");
    t->AddText("");
    t->AddText("Peak 3");  
    sprintf(text1, "#chi^{2}  %.5g", f[2]->GetChisquare() / f[2]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant %.5g #pm %.5g", f[2]->GetParameter(0),f[2]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean  %.5g #pm %.5g", f[2]->GetParameter(1),f[2]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma  %.5g #pm %.5g", f[2]->GetParameter(2),f[2]->GetParError(2)); 
    t->AddText(text3);
    myGraph->Fit(f[3], "R+");
    t->AddText("");
    t->AddText("Peak 4");  
    sprintf(text1, "#chi^{2}  %.5g", f[3]->GetChisquare() / f[3]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant %.5g #pm %.5g", f[3]->GetParameter(0),f[3]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean  %.5g #pm %.5g", f[3]->GetParameter(1),f[3]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma  %.5g #pm %.5g", f[3]->GetParameter(2),f[3]->GetParError(2)); 
    t->AddText(text3);
    myGraph->Fit(f[4], "R+");
    t->AddText("");
    t->AddText("Peak 5");  
    sprintf(text1, "#chi^{2}  %.5g", f[4]->GetChisquare() / f[4]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant %.5g #pm %.5g", f[4]->GetParameter(0),f[4]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean  %.5g #pm %.5g", f[4]->GetParameter(1),f[4]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma  %.5g #pm %.5g", f[4]->GetParameter(2),f[4]->GetParError(2)); 
    t->AddText(text3);
    myGraph->Fit(f[5], "R+");
    t->AddText("");
    t->AddText("Peak 6");  
    sprintf(text1, "#chi^{2}  %.5g", f[5]->GetChisquare() / f[5]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant %.5g #pm %.5g", f[5]->GetParameter(0),f[5]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean  %.5g #pm %.5g", f[5]->GetParameter(1),f[5]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma  %.5g #pm %.5g", f[5]->GetParameter(2),f[5]->GetParError(2)); 
    t->AddText(text3);
    t->Draw();
  }
  else if (n == 1) { // Cd109
    t = new TPaveText(.80,.7,.98,.95,"brNDC");
    t->SetFillColor(0);
    t->SetShadowColor(0);
    f[0] = new TF1("g1","gaus",19,26);
    f[1] = new TF1("g2","gaus",80,95);
    myGraph->Fit(f[0], "R");
    char text0[50];
    char text1[50];
    char text2[50];
    char text3[50];

    t->AddText("Peak 1"); 
    sprintf(text1, "#chi^{2}/ndf     %.5g", f[0]->GetChisquare() / f[0]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant     %.5g #pm %.5g", f[0]->GetParameter(0),f[0]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean     %.5g #pm %.5g", f[0]->GetParameter(1),f[0]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma     %.5g #pm %.5g", f[0]->GetParameter(2),f[0]->GetParError(2)); 
    t->AddText(text3);
    t->AddText("");
    myGraph->Fit(f[1], "R+");
    t->AddText("Peak 2");  
    sprintf(text1, "#chi^{2}/ndf    %.5g", f[1]->GetChisquare() / f[1]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant    %.5g #pm %.5g", f[1]->GetParameter(0),f[1]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean    %.5g #pm %.5g", f[1]->GetParameter(1),f[1]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma    %.5g #pm %.5g", f[1]->GetParameter(2),f[1]->GetParError(2)); 
    t->AddText(text3);
    t->Draw();
  }
  else if (n == 2) { // Co57
    t = new TPaveText(.80,.8,.98,.95,"brNDC");
    t->SetFillColor(0);
    t->SetShadowColor(0);
    f[0] = new TF1("g1","gaus",132,141);
    myGraph->Fit(f[0], "R");
    char text0[50];
    char text1[50];
    char text2[50];
    char text3[50];
    t->AddText("Peak 1");  
    sprintf(text1, "#chi^{2}/ndf    %.5g", f[0]->GetChisquare() / f[0]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant     %.5g #pm %.5g", f[0]->GetParameter(0),f[0]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean      %.5g #pm %.5g", f[0]->GetParameter(1),f[0]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma     %.5g #pm %.5g", f[0]->GetParameter(2),f[0]->GetParError(2)); 
    t->AddText(text3);
    t->Draw();

  }
  else if (n == 3) { // Co60
    t = new TPaveText(.80,.7,1,.98,"brNDC");
    t->SetFillColor(0);
    t->SetShadowColor(0);
    f[0] = new TF1("g1","gaus",1120,1210);
    f[1] = new TF1("g2","gaus",1280,1370);
    myGraph->Fit(f[0], "R");
    char text0[50];
    char text1[50];
    char text2[50];
    char text3[50];
    t->AddText("Peak 1");  
    sprintf(text1, "#chi^{2}/ndf     %.5g", f[0]->GetChisquare() / f[0]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant     %.5g #pm %.5g", f[0]->GetParameter(0),f[0]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean     %.5g #pm %.5g", f[0]->GetParameter(1),f[0]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma     %.5g #pm %.5g", f[0]->GetParameter(2),f[0]->GetParError(2)); 
    t->AddText(text3);
    myGraph->Fit(f[1], "R+");
    t->AddText("");
    t->AddText("Peak 2"); 
    sprintf(text1, "#chi^{2}/ndf     %.5g", f[1]->GetChisquare() / f[1]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant     %.5g #pm %.5g", f[1]->GetParameter(0),f[1]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean     %.5g #pm %.5g", f[1]->GetParameter(1),f[1]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma     %.5g #pm %.5g", f[1]->GetParameter(2),f[1]->GetParError(2)); 
    t->AddText(text3);
    t->Draw();
  }
  else if (n== 4) { // Cs137
    t = new TPaveText(.80,.8,1,.98,"brNDC");
    t->SetFillColor(0);
    t->SetShadowColor(0);
    f[0] = new TF1("g1","gaus",620,700);
    myGraph->Fit(f[0], "R");
    char text0[50];
    char text1[50];
    char text2[50];
    char text3[50];
    t->AddText("Peak 1");  
    sprintf(text1, "#chi^{2}/ndf     %.5g", f[0]->GetChisquare() / f[0]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant     %.5g #pm %.5g", f[0]->GetParameter(0),f[0]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean     %.5g #pm %.5g", f[0]->GetParameter(1),f[0]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma     %.5g #pm %.5g", f[0]->GetParameter(2),f[0]->GetParError(2)); 
    t->AddText(text3);
    t->Draw();
  }
  else if (n == 5) { // Mn54
    t = new TPaveText(.80,.8,1,.98,"brNDC");
    t->SetFillColor(0);
    t->SetShadowColor(0);
    f[0] = new TF1("g1","gaus",800,860);
    myGraph->Fit(f[0], "R");
    char text0[50];
    char text1[50];
    char text2[50];
    char text3[50];
    t->AddText("Peak 1");  
    sprintf(text1, "#chi^{2}/ndf     %.5g", f[0]->GetChisquare() / f[0]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant     %.5g #pm %.5g", f[0]->GetParameter(0),f[0]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean     %.5g #pm %.5g", f[0]->GetParameter(1),f[0]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma     %.5g #pm %.5g", f[0]->GetParameter(2),f[0]->GetParError(2)); 
    t->AddText(text3);
    t->Draw();
  }
  else if (n == 6) { // Na22
    t = new TPaveText(.80,.7,.98,.95,"brNDC");
    t->SetFillColor(0);
    t->SetShadowColor(0);
    f[0] = new TF1("g1","gaus",460,550);
    f[1] = new TF1("g2","gaus",1170,1230);
    myGraph->Fit(f[0], "R");
    char text0[50];
    char text1[50];
    char text2[50];
    char text3[50];    
    t->AddText("Peak 1");  
    sprintf(text1, "#chi^{2}/ndf     %.5g", f[0]->GetChisquare() / f[0]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant     %.5g #pm %.5g", f[0]->GetParameter(0),f[0]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean     %.5g #pm %.5g", f[0]->GetParameter(1),f[0]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma     %.5g #pm %.5g", f[0]->GetParameter(2),f[0]->GetParError(2)); 
    t->AddText(text3);
    myGraph->Fit(f[1], "R+");
    t->AddText("");
    t->AddText("Peak 2");  
    sprintf(text1, "#chi^{2}/ndf     %.5g", f[1]->GetChisquare() / f[1]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant     %.5g #pm %.5g", f[1]->GetParameter(0),f[1]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean     %.5g #pm %.5g", f[1]->GetParameter(1),f[1]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma     %.5g #pm %.5g", f[1]->GetParameter(2),f[1]->GetParError(2)); 
    t->AddText(text3);
    t->Draw();
  }
  else if (n == 7) { // Unknown
    t = new TPaveText(.80,.7,1,.98,"brNDC");
    t->SetFillColor(0);
    t->SetShadowColor(0);
    f[0] = new TF1("g1","gaus",22,38);
    f[1] = new TF1("g2","gaus",620,690);
    myGraph->Fit(f[0], "R");
    char text0[50];
    char text1[50];
    char text2[50];
    char text3[50];
    t->AddText("Peak 1");  
    t->SetTextSize(2);
    sprintf(text1, "#chi^{2}/ndf     %.5g", f[0]->GetChisquare() / f[0]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant     %.5g #pm %.5g", f[0]->GetParameter(0),f[0]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean      %.5g #pm %.5g", f[0]->GetParameter(1),f[0]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma      %.5g #pm %.5g", f[0]->GetParameter(2),f[0]->GetParError(2)); 
    t->AddText(text3);
    myGraph->Fit(f[1], "R+");
    t->AddText("");
    t->AddText("Peak 2"); 
    sprintf(text1, "#chi^{2}/ndf     %.5g", f[1]->GetChisquare() / f[1]->GetNDF());  
    t->AddText(text1);
    sprintf(text0, "Constant     %.5g #pm %.5g", f[1]->GetParameter(0),f[1]->GetParError(0)); 
    t->AddText(text0);
    sprintf(text2, "Mean     %.5g #pm %.5g", f[1]->GetParameter(1),f[1]->GetParError(1)); 
    t->AddText(text2);
    sprintf(text3, "#sigma     %.5g #pm %.5g", f[1]->GetParameter(2),f[1]->GetParError(2)); 
    t->AddText(text3);
    t->Draw();
  }

  //c->Update();
 // c->Modified();

  char file[50];
  sprintf(file, "%s%s",name[n].Data(), ext1.Data());
  c->SaveAs(file);
  delete t; delete f[0]; delete f[1]; delete f[2]; delete f[3]; delete f[4]; delete f[5];
  delete c;

}

return 1;
}
