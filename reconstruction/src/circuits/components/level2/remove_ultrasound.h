/*
  Cut ultra-sound ...
  Remove high-frequency components for smoother experience and less annooyance (esp, for sensitive ears)
  Filters out frequencies about 10 KHz
*/
Source *GeneralOutputFilter(Source *x)
{
  NEWC(clip, ClipHardSimpleOutput, (0.9));
  ATT(clip, input, x);

  NEWC(lp, LowPassButterworth, ());
  ATT(lp, freq, 10000); // 20000);
  ATT(lp, input, clip);
  return lp;
}
