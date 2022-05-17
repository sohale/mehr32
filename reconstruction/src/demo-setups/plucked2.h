void plucked2()	//18MehrMon1379 21:11
{
	timer=new Timer();

	KeyboardHit1*kbhit=new KeyboardHit1(KeyboardHit1::_StandardMap,KeyboardHit1::_StandardOffset);
	Source*note=kbhit->lineout("char");
	Component*nfreq=new NoteFreqS();
	nfreq->attach("note",note);
	nfreq->attachConstant("shift",0);	//necassary

	Aggregate*pluck=new PluckedInstrument0();
	pluck->attach("freq",nfreq);
	pluck->attach("time",kbhit);

	attachGlobalOutput(GeneralOutputFilter(pluck->getLineout()));
	Action();
}
