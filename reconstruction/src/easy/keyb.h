	#define KEYB(kbhit,nfreq) 	\
		KeyboardHit1*kbhit=new KeyboardHit1(KeyboardHit1::_StandardMap,KeyboardHit1::_StandardOffset);	\
		Component*nfreq=new NoteFreqS();	\
		{Source*note=kbhit->lineout("char");	\
		nfreq->attach("note",note);	\
		nfreq->attachConstant("shift",0);}
