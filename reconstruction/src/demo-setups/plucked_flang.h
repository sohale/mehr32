/******************************************************
 * Plucked String with Flanger effect
 * //after ACM
 ******************************************************/
void pluckedFlang() // 10AzarThur1379 03:40 to-have-done-something-for-today
{
  TIMER(44100);

  KEYB(kbhit, nfreq);

  NEWC(pluck, KarplusStrongPlucked1, ());
  ATT(pluck, gain, 1);
  ATT(pluck, freq, nfreq);
  ATT(pluck, time, kbhit);
  Source *ss = pluck->getLineout();

  Source *s = selector(ss, flang0(ss), "`1", "Flanger");
  PLAYMONO(boost(s, 0.6));
}
