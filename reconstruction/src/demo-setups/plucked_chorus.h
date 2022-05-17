
/******************************************************
 * Plucked String with Chorus effect
 ******************************************************/
void pluckedChorus()
{
  TIMER(44100);

  KEYB(kbhit, nfreq);

  NEWC(pluck, KarplusStrongPlucked1, ());
  ATT(pluck, gain, 1);
  ATT(pluck, freq, nfreq);
  ATT(pluck, time, kbhit);

  Cache *ss = new Cache();
  ss->attach("input", pluck->getLineout() /*ssi*/);

  Source *mix = selector(ss, chorus(2, ss), "`1", "Chorus");

  // adds TANIN
  const M = 5;
  real ptsX[M] = {-1.0, -0.5, 0, 0.5, 0.1};
  real ptsY[M] = {-1.0, -1.0, 0, 1.0, 1.0};
  Distortion *testPLi = new PartialLinearDistortion(M, ptsX, ptsY);
  testPLi->attach("input", mix);

  PLAYMONO(boost(
      // mix
      testPLi, 0.6));
}
