/**************************************************
 * Simulated human whistle sound. Very realistic. (Sept 2000)
 **************************************************/
void whistle1(bool elastic, bool reverb)	//17MehrSun1379Noon12:59
{
	Timer *t=new Timer();
	timer=t;
	KeyboardHit1*kbhit=new KeyboardHit1(KeyboardHit1::_StandardMap,0+12+12);
	Component* asr=new ASR(1.0);
	asr->attachConstant("rise",0.03);
	asr->attachConstant("dur",0);
	asr->attachConstant("rel",0.3*2);

	//asr->reattachConstant(	"amp",0.1*10*5);
	asr->reattachConstant(	"amp",0.1*10);
	asr->attach("time",kbhit);
	Component *randi_=new RANDI();
	randi_->reattach("amp",asr);
	randi_->reattachConstant("freq",40000);
	Component *singen=new SinOscillator();
	singen->reattach("amp",randi_);
	singen->attachConstant("freq",20000);
	Component *filt=new Filter1();
	filt->attach("input",singen);
	filt->attachConstant("width", 0.01 *10*2);	//0.01 causes continues sine

	Source*note=kbhit->lineout("char");
	Component*nfreq=new NoteFreq();
	nfreq->attach("note",note);
	if(elastic)
	{
		Component*elasticfreq=new Alpha();
		elasticfreq->attach("input",nfreq);
		elasticfreq->attachConstant("alpha",0.01);
		filt->attach("freq",elasticfreq);
	}
	else
	{
		filt->attach("freq",nfreq);
	}
	Source *sss;
	sss=filt;//la;
	if(reverb)
	{
	//IF HEAVY_REVERB
		if(1)
		{
			Aggregate*r=new Reverb0();
			r->attachConstant("rvt",4);
			r->attach("input",filt);
			sss=r->getLineout();
		}
		if(0)
		{
			//Component*refl=new RetroAS1_Reflection_Sparse();
			Component*refl=new RetroAS1_Reflection_Diffuse();
			refl->attach("input",filt);
			sss=refl;
		}
	}
	attachGlobalOutput(GeneralOutputFilter(sss));
	Action();
	quit();
}
