/**********************************************************
 * A demo setup of the Karplus-Strong Plucked String      *
 * model of string instruments                            *
 **********************************************************/
void karplus()
{

  TIMER(22050);

  NEWC(kbhit, KeyboardHit1, (KeyboardHit1::_StandardMap, -12));
  Source *note = kbhit->lineout("char");

  bool tiu_ = false;
  Component *tiu = NULL;
  if (tiu_)
  {
    const int N = 4;
    real Y[N] = {-12, -12, 0, 0};
    real X[N] = {-1, 0, 0.03, 1};
    tiu = new PartialLinearDistortion(N, X, Y);
    tiu->attach("input", kbhit);
  }

  NEWC(nfreq, NoteFreqS, ());
  ATT(nfreq, note, note);
  if (tiu_)
    ATT(nfreq, shift, tiu);
  else
    ATT(nfreq, shift, .0);

  NEWC(pluck, KarplusStrongPlucked1, ());
  ATT(pluck, freq, nfreq);
  ATT(pluck, time, kbhit);
  ATT(pluck, gain, 1.0);
  Source *o = pluck->getLineout();

  bool reverb = true;
  if (reverb)
  {
    NEWC(rvb, Reverb0, ());
    ATT(rvb, rvt, 3);
    ATT(rvb, input, o);
    o = rvb->getLineout();
  }

  NEWC(pp, SinPositiveOscillator, ());
  ATT(pp, freq, 10);

  NEWC(so, StereoBalanceSimple, ());
  ATT(so, pos, pp);
  ATT(so, input, o);

  PLAYSTEREO(so->getLineout(0), so->getLineout(1));
}
