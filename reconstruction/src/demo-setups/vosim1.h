/*
	Its frequency form is similar to its waveform!
*/
void VOSIM1()
{
	timer=new Timer();
	timer->reattachConstant("freq",44100);

	Component*v=new BasicVOSIMWaveform0();
	//bassoon
	v->attachConstant("freq",800);
	v->reattachConstant("b",1);
	v->attachConstant("N",2);
	v->attachConstant("subfreq",2.9);	//M=e

	Component*rndi=new RANDI();
	rndi->reattachConstant("amp",5);

	//note freqline holes
	KeyboardHit1*kbhit=new KeyboardHit1(KeyboardHit1::_StandardMap, 24);
	Component*freq0=new NoteFreqS();
	freq0->attach("note",kbhit->lineout("char"));
	freq0->attach("shift",rndi);
	{Component*c=new Cache();c->printLinkNames();c->attach("input",freq0);freq0=c;}
	Component*TT=new Divide();
	TT->attachConstant("input",1);
	TT->attach("by",freq0);
	Component*T3=new Divide();
	T3->attachConstant("input",3);
	T3->attach("by",freq0);

	Component*envM=new ASR(2);
	envM->printLinkNames();
	envM->attachConstant("rise",0.1);
	envM->attachConstant("dur",0.3*3);
	envM->attachConstant("rel",1*3);
	envM->reattach("amp",/*3/freq0*/T3);

	envM->attach("time",kbhit);

	Component*envV=new ASR(2);
	envV->attachConstant("rise",0.1);
	envV->attachConstant("dur",10);
	envV->attachConstant("rel",1);
	envV->attach("time",kbhit);

	Aggregate*v2=new BasicVOSIMWaveform();
	v2->attachConstant("N",2);
	v2->attachConstant("b",0.8);
	v2->attach("M",envM);
	v2->attach("T",/*1/freq0*/TT);
	v2->reattach("A",envV);

	Component*nodc=new NoDC();
	nodc->attach("input",v2->getLineout());

	attachGlobalOutput(GeneralOutputFilter(nodc));
	Action();
	quit();
}
