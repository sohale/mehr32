/******************************************************
 * Retro instrument
 ******************************************************/
void retro1()
{
  timer = new Timer();
  timer->reattachConstant("freq", 44100);

  KeyboardHit1 *kbhit = new KeyboardHit1(KeyboardHit1::_StandardMap, KeyboardHit1::_StandardOffset);
  Source *note = kbhit->lineout("char");
  Component *nfreq = new NoteFreqS();
  nfreq->attach("note", note);
  nfreq->attachConstant("shift", 0); // necassary

  Aggregate *pluck = new PluckedInstrument0();
  pluck->attach("freq", nfreq);
  pluck->attach("time", kbhit);
  Source *po = pluck->getLineout();

  Component *refl = new RetroAS1_Reflection_Sparse();
  refl->attach("input", po);

  Component *lp = new Filter1();
  lp->attach("input", refl);
  lp->attachConstant("freq", 2000 / 4 / 3);
  lp->attachConstant("width", 2000 / 3);

  PLAYMONO(lp);
};
