void impulse1()
{
	printf("int=%d\n",sizeof(int)*8);
	printf("long=%d\n",sizeof(long)*8);
	timer=new Timer();
	timer->reattachConstant("freq",22050);

	//maximm 460 in 22050sps
	real ir[]={1,0,0, 0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,
		-1,
		0,0,0};
	int irl=sizeof(ir)/sizeof(real);
	printf("h[] length=%d\n",irl);
	Component*ec=new 
		//ImpulseFilterSimple(ir,irl);
		ImpulseFilterFixed(ir,irl);

	KeyboardHit1*kbhit=new KeyboardHit1(KeyboardHit1::_StandardMap, 0);
	Component*nfreq=new NoteFreq();
	nfreq->attach("note",kbhit->lineout("char"));
	Aggregate*pluck=new PluckedInstrument0();
	pluck->attach("freq",nfreq);
	pluck->attach("time",kbhit);
	Source*pout=pluck->getLineout();

	ec->attach("input",pout);

	//WP32_src=
	//	//pout;
	//	ec;
	attachGlobalOutput(GeneralOutputFilter(ec));
	//WP32_play();
	Action();
	quit();
}
