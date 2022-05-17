
/******************
 *   Percussions  *
 ******************/
void rythms()
{

  char *playKeys1 = "zsxdcvgbhnjm,l.;/q2w3e4rt6y7ui9o"; //-[]
  char *bassDrumKeys = "~`";
  char *HihatKeys = "1!";
  char *SnareKeys = "\t";
  char *pvsKeys = "-=";
  char *threeStateKeys = "p[]";

  printf("keys:\n ~ for bass drum\n 1 for Hihat\n <tab> for Snare\n - and = for instrument select (Plucked vs String)\n p[] for tuning flang mix\n * be sure to deactivate Caps-Lock *\n");

  TIMER(22050); // Snare doesn't work in higher rates

  NEWC(bd, BassDrum_K909_1, ());
  ATT(bd, amp, 1);
  NEWC(bdkey, KeyboardHit1, (bassDrumKeys, 0));
  ATT(bd, time, bdkey);

  NEWC(hh, Hihat_1, ());
  NEWC(hhkey, KeyboardHit1, (HihatKeys, 0));
  ATT(hh, amp, 1);
  ATT(hh, time, hhkey);

  NEWC(sn, Snare_1, ());
  NEWC(snkey, KeyboardHit1, (SnareKeys, 0));
  ATT(sn, time, snkey);

  NEWC(string, String1, ());
  ATT(string, amp, 0.25);

  NEWC(skey, KeyboardHit1, (playKeys1, -12 * 0));
  Source *note = skey->lineout("char");
  NEWC(nfreq, NoteFreqS, ());
  ATT(nfreq, note, note);
  ATT(nfreq, shift, 12);

  ATT(string, freq, nfreq);
  ATT(string, time, skey);

  NEWC(mix, Mix, (4));
  ATT(mix, input0, bd->getLineout());
  ATT(mix, input1, .0); // It is attached later
  ATT(mix, input2, sn->getLineout());
  ATT(mix, input3, hh->getLineout());

  Aggregate *pluck = new KarplusStrongPlucked1();
  pluck->attach("freq", nfreq);
  pluck->attach("time", skey);
  pluck->attachConstant("gain", 1.0);
  RATT(mix, input1,
       selector(pluck->getLineout(), string->getLineout(), pvsKeys, "Plucked String vs FM String"));

  NEWC(mc, Cache, ());
  ATT(mc, input, mix);

  // if flang
  NEWC(flng, Flanger1, (Flanger1::TRIANGLE,
                        0.8 * 2 // /1.5
                        ,
                        /*0.060*2*/
                        1 / (0.8 * 2) / 4 / 2 * 2, // octave=1/f/4 (in triangle) octave=1/f/2 (in saw)
                        0.01, 0));                 // default params
  ATT(flng, input, mc);

  NEWC(tk, KeyboardHit1, (threeStateKeys, 0));
  NEWC(threestate, LazyLinear, (0.5, 0));
  ATT(threestate, input, tk->lineout("char"));

  NEWC(mixorig, MixAmp0_Exclusive, ());
  ATT(mixorig, input, flng);
  ATT(mixorig, original, mc);
  ATT(mixorig, gain, threestate);

  PLAYMONO(mixorig); // mc
}
