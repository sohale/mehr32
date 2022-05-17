
/******************************************************
 * Another Plucked String instrument
 ******************************************************/

void plucked1()	//16MehrSat1379(12XM)
{
	Timer *t=new Timer();
	timer=t; //globaltimer

	KeyboardHit1*kbhit=new KeyboardHit1("zsxdcvgbhnjm,l.;/q2w3e4rt6y7ui9o0p-[]",0);

	Component* asr=new ASR(1.0);
	asr->attachConstant("rise",0);
	asr->attachConstant("dur",0.01);
	asr->attachConstant("rel",0.0);
	asr->attach("time",kbhit);

	Component *randh=new RANDH(1.0/2.0);
	randh->reattach("amp",asr);

	Component*mixer=randh;

	Source*note=kbhit->lineout("char");//new Constant(0);
	Component*freq=new NoteFreq();
	freq->attach("note",note);
	Component*nfd=new Divide();
	nfd->attachConstant("input",1);
	nfd->attach("by",freq);

	Component*comb=new Comb(3,true);	//Anti Aliasing
	comb->attach("delay",/*0.05234*/nfd);
	comb->attachConstant("reverbtime",2*4);
	comb->attach("input",mixer);

	Component *amp = new Amp();
	amp->attach("input", comb);
	amp->attachConstant("amp",0.25);

	attachGlobalOutput(GeneralOutputFilter(amp));
	Action();
	quit();

	timer->reset();

	RawRecorder *rec = new RawRecorder();
	printf("start");
	rec->attach("input", amp);
	rec->record(5);
	printf("\ndone\n");
	quit();	//340K random
}
