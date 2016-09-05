float CorrectionFunction(TProfile *prof, float cent, float bound) { 

  if (bound <= 0 || bound > 1) return centroid;
  Float_t binSize = prof->GetXaxis()->GetBinWidth(prof->GetXaxis()->FindBin(cent));
  Float_t nBins = 1 + 2 * (bound/binSize);
  Float_t avgCorrection = 0;
  for (int i=0; i < nBins; ++i) {
    avgCorrection += prof->GetBinContent(prof->GetXaxis()->FindBin((cent - bound) + (i*binSize)));
  }
  avgCorrection /= nBins;
  return avgCorrection;
}
