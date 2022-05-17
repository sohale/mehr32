void EffectPlayAFile(char *filename)
{
  TIMER(22050);
  NEWC(wavin, RawWaveIn8uMono, (filename, 22050, 0, false));
  RATT(wavin, time, new KeyboardHit1("qwertyuiopzxcvbnm,./asdfghjkl;'[]", 0));
  PLAYMONO(selector3(wavin, chorus2(2, wavin), flang0(wavin), "`12", "Normal|Chorus|Flanger"));
}
