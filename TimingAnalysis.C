#include <iostream>

using namespace std;


int main() {

  //  for (int i = 5; i < 11; ++i) { 
    char file[256];
    sprintf(file,"/data/mike/ExtPulserScan/ExtPulserScan/TreeTimingJitterFe55Run80105.root");    
    cout << " file " << file << endl;
    /*if (i == 0 )*/TFile f(file);
    // else if (i == 5 ) TFile f2(file);
    //else if (i == 10) TFile f3(file);
    //else TFile f4(file);
   
    TCanvas *c1 = new TCanvas("c1","c1");
    Hit->Draw("MaxCharge","PadNo ==354");
    char output[256];
    sprintf(output,"../ExtPulserTiming/Pad354Fe55Spectrum.root");
       c1->SaveAs(output);
    delete c1;
    TCanvas *c2 = new TCanvas("c2","c2");
    Hit->Draw("MaxCharge","PadNo == 352");
    char output[256];
    sprintf(output,"../ExtPulserTiming/Pad352Fe55Spectrum.root");
    c2->SaveAs(output);
    delete c2;
    


    //}
  return 0;
}
