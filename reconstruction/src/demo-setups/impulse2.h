void impulse2() // using FIRFew0
{
  timer = new Timer();
  timer->reattachConstant("freq", 44100);

  real impulse[] = {0, +0.1, 0.5, +0.52, 0.53};
  real FIRgain[] = {1, -0.5, 0.3, -0.2, 0.1};
  int irn = sizeof(FIRgain) / sizeof(real);
  Component *ec = new FIRFew0(1.0, irn, impulse, FIRgain);
  KeyboardHit1 *kbhit = new KeyboardHit1(KeyboardHit1::_StandardMap, KeyboardHit1::_StandardOffset);
  Component *nfreq = new NoteFreq();
  nfreq->attach("note", kbhit->lineout("char"));
  Aggregate *pluck = new PluckedInstrument0();
  pluck->attach("freq", nfreq);
  pluck->attach("time", kbhit);
  Source *pout = pluck->getLineout();
  ec->attach("input", pout);
  // WP32_src=ec;
  attachGlobalOutput(GeneralOutputFilter(ec));
  // WP32_play();
  Action();
  quit();
}
