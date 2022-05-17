
void plucked3
(
	int sampleRate,
	bool modulation,
	bool elastic,
	bool detune,
	bool reverb,
	int transpose
)
{
	//COOL! present it level by level!
	//adding modulation, detune, outatune, random phase-shift
	timer=new Timer();
	timer->reattachConstant("freq",sampleRate);///*11025*/22050);

	KeyboardHit1*kbhit=new KeyboardHit1(KeyboardHit1::_StandardMap, transpose+0);

	//mfreq:20,mrise=0 kind of a new strange ins.
	real MFREQ=2.0 ;	//2.0Hz
	real MRISE=3 ;	//mod. rise time
	real MDEPTH=0.5;	//modulation depth

	Component*msat=new ASR(1);
	msat->attachConstant("rise",MRISE);
	msat->attachConstant("dur",100);
	msat->attachConstant("rel",1);
	msat->reattachConstant("amp",MDEPTH);
	msat->attach("time",kbhit);

	Component*modulator=new SinOscillator();
	modulator->reattach("amp",msat);
	modulator->attachConstant("freq",MFREQ);	

	//Component*randphase=new RANDI(/*0.001*/1);
	//randphase->reattachConstant("amp",PI/8);
	//modulation->reattach("phase",randphase);

	Source*note=kbhit->lineout("char");
	Component*nfreq=new NoteFreqS();
	nfreq->attach("note",note);
	if(modulation)
		nfreq->attach("shift",modulator);
	else
		nfreq->attachConstant("shift",0);

	Component*elasticfreq=new Alpha();
	elasticfreq->attach("input",nfreq);
	elasticfreq->attachConstant("alpha",0.01);

	Aggregate*pluck=new PluckedInstrument0();
	pluck->attach("time",kbhit);
	if(elastic)
		pluck->attach("freq",elasticfreq);
	else
		pluck->attach("freq",nfreq);

	Source*out=pluck->getLineout();

	if(detune)
	{
		Aggregate*dtn=new Detune0();
		dtn->attach("input",out);
		//dtn->attachConstant("delay",0.01);
		Component*sind=new SinOscillator();
		sind->attachConstant("freq",0.3*8*2);
		//sind->reattachConstant("amp",0.01);
		Component*sinda=new LazyLinear(0.0001,0.0005);
		sinda->attach("input",sind);
		dtn->attach("delay",sinda);
		out=dtn->getLineout();
	}
	
	if(reverb)
	{
		Aggregate*r=new Reverb0();
		r->attach("input",out);
		r->attachConstant("rvt",3);
		out=r->getLineout();
	}

	attachGlobalOutput(GeneralOutputFilter(out));
	Action();
}
