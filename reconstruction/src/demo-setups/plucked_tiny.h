
/*****************************************************
 * An instrument with COOL NICE tiny voice (Nov 2000)
 *****************************************************/
void pluckedTiny() // 18AzarThur1379 03:59pm-05:57pm
{
  TIMER(44100);

  KEYB(kbhit, nfreq);

  Component *asr = new ASR(9.0);
  asr->attachConstant("rise", 0);
  asr->attachConstant("dur", 0);
  asr->attachConstant("rel", 0.05);
  asr->reattachConstant("amp", 0.10);
  asr->attach("time", kbhit);

  Component *randh = new RANDH(1.0 / 2.0);
  randh->reattach("amp", asr);
  Component *rs = randh;

  Component *nfd = new Divide();
  nfd->attachConstant("input", 1);
  nfd->attach("by", nfreq);

  Component *mix = new Mix(2);
  mix->attach("input0", rs);

  Source *d = nfd; // delay amount
  Component *fbc0 = new Cache();
  Delay *delay = new Delay(1, false);
  fbc0->attach("input", delay);
  delay->attach("delay", d);
  delay->attach("input", mix);

  Component *ff = new LinearDistortion(0.9, 0);
  ff->attach("input", fbc0);
  mix->attach("input1", ff);

  PLAYMONO(boost(fbc0 /*mix*/, 4 * 4));
}
