void EffectPlayAFile2(char*filename)
{
	TIMER(22050);
	NEWC(wavin, RawWaveIn8uMono,(filename,22050,0,false));
	RATT(wavin,time,new KeyboardHit1("qwertyuiopzxcvbnm,./asdfghjkl;'[]",0));

	NEWC(flng, Flanger1, (Flanger1::TRIANGLE,
		0.8*2 *3,
		/*0.060*2*/
		1/(0.8*2)/4/2*2 /2/2, //octave=1/f/4 (in triangle) octave=1/f/2 (in saw)
		0.01,0));	//default params
	ATT(flng, input, wavin);

	PLAYMONO(selector(wavin,flng->getLineout(),"`1","Triangular Flanger"));
}
