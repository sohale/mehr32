void VOSIM2()
{
	timer=new Timer();
	timer->reattachConstant("freq",22050);

	KeyboardHit1*kb=new KeyboardHit1(KeyboardHit1::_StandardMap, 24);

	//bassoon
	Component*envelop=new ASR(0.1);
	envelop->attachConstant("rise",0.1);
	envelop->attachConstant("dur",1);
	envelop->attachConstant("rel",1);
	envelop->attach("time",kb);

	Component*noiseenvelop=new LazyLinear(-20,20);
	noiseenvelop->printLinkNames();
	noiseenvelop->attach("input",envelop);

	Component*elasticfreq=new Alpha();
	elasticfreq->attach("input",kb->lineout("char"));
	elasticfreq->attachConstant("alpha",0.01*3);

	Component*freq0=new NoteFreqS();
	freq0->attach("note",elasticfreq);
	Component*rndi=new RANDI();
	freq0->attach("shift",rndi);
	rndi->reattach("amp",noiseenvelop);
	freq0=cache(freq0);

	Component*T1=new Divide();
	T1->attachConstant("input",1);
	T1->attach("by",freq0);
	Component*T3=new Divide();
	T3->attachConstant("input",3);
	T3->attach("by",freq0);

	Aggregate*v2=new BasicVOSIMWaveform();
	v2->attachConstant("N",2);
	v2->attachConstant("b",0.8);
	v2->attach("M",T3);
	v2->attach("T",T1);
	v2->reattach("A",envelop);

	Component*nodc=new NoDC();
	nodc->attach("input",v2->getLineout());

	//WP32_src=nodc;
	attachGlobalOutput(GeneralOutputFilter(nodc));
	//WP32_capture="c:\\out32debug.raw";
	//WP32_play();
	Action();
	quit();
}
