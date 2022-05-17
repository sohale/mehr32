/********************************************
 * An instrumnt based on KS Plucked String  *
 ********************************************/

void space_pluck(char* prce, real alpha, bool distort)
{
	TIMER(22050);

	KeyboardHit1*kbhit=new KeyboardHit1(KeyboardHit1::_StandardMap,KeyboardHit1::_StandardOffset);
	Source*note=kbhit->lineout("char");

	Component*nfreq=new NoteFreqS();
	nfreq->attach("note",note);
	nfreq->attachConstant("shift",0);

	bool elastic=true;
	if(elastic)
	{
		Component*elasticfreq=new Alpha();
		elasticfreq->attach("input",nfreq);
		elasticfreq->attachConstant("alpha",alpha);
		nfreq=elasticfreq;
	}

	Source*o;

	Component*perrycook=NULL;
	if(prce!=NULL)
	{
		char*FILENAME=prce;
		perrycook=new RawWaveIn16sMono(FILENAME,44100,0,false);
		perrycook->reattach("time",kbhit);
	}

	Aggregate* pluck = new KarplusStrongPlucked1();	//my version
	pluck->attach("freq",nfreq);
	pluck->attach("time",kbhit);
	/*********************************************/
	if(prce!=NULL)
		pluck->reattach("excitation", perrycook );
	/*********************************************/
	pluck->attachConstant("gain", 1.0);
	o=pluck->getLineout();

	if(distort)
	{
		NEWC(dist,PAFuzzDistortion,());
		ATT(dist,input,o);
		o=dist;
	};

	bool echo=true;
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

	Component *pp=new SinPositiveOscillator();
	pp->attachConstant("freq",1);
	AggregateMultiOutput*so=new StereoBalanceSimple();
	so->attach("pos",pp);
	so->attach("input",o);
	Source*left=so->getLineout(0);
	Source*right=so->getLineout(1);

	bool space=true;		//use different reverbs for L&R
	bool longdelay=false;

	//(0.2, 29.7, 37.1, 41.1, 43.7, 96.83, 5, 32.92, 1.7)
	Aggregate* rvbL = new Reverb1(0.2, 28.7, 37.1, 45.1, 54.7, (longdelay?200:0)+101.83, 4, 33.92, 0.91123);
						if(!space) rvbL=new Reverb0();
	rvbL->attachConstant("rvt",1);
	rvbL->attach("input",left);

	Aggregate* rvbR = new Reverb1(0.25, 29.7, 37.1, 41.1, 43.7, (longdelay?150:0)+96.83, 5, 32.92, 1.7);
						if(!space) rvbR=new Reverb0();
	rvbR->attachConstant("rvt",1);
	rvbR->attach("input",right);

	PLAYSTEREO(rvbL->getLineout(),rvbR->getLineout());

}
