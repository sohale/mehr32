
//Plucked string, excitation signal study
void pluck2(char* exc, real factor, real falloff, bool distort, real tune)
{
	//Viol* : factor=0,falloff=1

	TIMER(22050);

	KeyboardHit1*kbhit=new KeyboardHit1(KeyboardHit1::_StandardMap,tune);
	Source*note=kbhit->lineout("char");

	Source*rndshift=new Constant(0);

	Component*nfreq=new NoteFreqS();
	nfreq->attach("note",note);
	nfreq->attach("shift",rndshift);


	Component*rawin=new RawWaveIn16sMono(exc,44100,0,false);
	rawin->reattach("time",kbhit);
	NEWC(rawinamp, LazyLinear, (0.1,0));
	ATT(rawinamp,input,rawin);
	Component*excs=rawinamp;

	Aggregate* pluck = new KarplusStrongPlucked2(factor);
	pluck->attach("freq",nfreq);
	pluck->attach("time",kbhit);
	pluck->attach("excitation", excs);
	pluck->attachConstant("gain", falloff);
	Source*o;
	o=pluck->getLineout();


	if(distort)
	{
		//SoftSaturationWaveShaper		//ATT(dist,a,0.1);
		//OverDriveWaveShaperJW		ATT(dist,overdrive,0.8);
		NEWC(dist,PAFuzzDistortion,());
		ATT(dist,input,o);
		o=dist;
	};

	bool echo=false;
	if(echo)
	{
		Component*echo=new Comb(2,false);
		echo->attach("input",o);
		echo->attachConstant("delay",0.5 );
		echo->attachConstant("reverbtime",7);

		Component*mix=new Mix(2);
		mix->attach("input0",echo);
		mix->attach("input1",o);
		o=mix;
	}
	//o=excs;

	bool reverb=false;
	bool space=false;		//use different reverbs for L&R
	bool longdelay=false;

	Aggregate* rvb = new Reverb1(0.2, 28.7, 37.1, 45.1, 54.7, (longdelay?200:0)+101.83, 4, 33.92, 0.91123);
	rvb->attachConstant("rvt",1);
	rvb->attach("input",o);
	//o=rvb->getLineout();


	NEWC(mc, Cache, ());
	ATT(mc, input, o);


	//Pitch shift module

	real FRQ=4;
	real RATIO=1-0.5;
	real frq=15*2;
	real ratio=2;
	NEWC(shft, PitchShiftMySoft1, (0.5,ratio/frq, frq));//RATIO,FRQ));
	ATT(shft, input, mc);

	NEWC(mixorig, MixAmp1_Exclusive, (.4));
	ATT(mixorig, input, shft);
	ATT(mixorig, original, mc);

	o=mixorig;

	PLAYMONO(boost(o,8));
}
