/**************************************
 * A simple time-variable line delay  *
 **************************************/
Source *flang0(Source *ss)
{
  Component *sinosc = new SinOscillator();
  Component *flang = new LazyLinear(0.001 / 2, 0.001 / 2); //(0.5,0.5);  //insectly
  flang->attach("input", sinosc);
  sinosc->attachConstant("freq", 3.0 * 2 /* *3 */);
  Component *delay = new Delay(2, true);
  delay->attach("delay", flang);
  delay->attach("input", ss);

  return delay;
}
