/* ******************************************** *
 *    mehr32
 *    Author: Sohail Siadatnejad
 *    (c) March 2000
 *
 * Note: This is a selection of files (for my BSc Thesis)
 *   these functions were originally in separate include files.
 * ******************************************** */

#include <conio.h>
#include "myheader.h"
#include "Compont1.h"

#include "Debug.h"

#include "Oscill.h"
#include "Distort.h"
#include "MultiOut.h"
#include "Impulse.h"
#include "Aggregat.h"

//Instantiates parametric Component Patterns
#include "Compont2.h"

#include "PluckIns.h"
#include "Reverb.h"
#include "Keyboard.h"
#include "Output.h"
#include "VOSIM.h"
#include "FreqDom.h"
#include "RawIn.h"
#include "pitch.h"
#include "MusicBox.h"
#include "Localiz.h"
#include "MiscIns1.h"
#include "Flang.h"
#include "easy.h"


/* ********************************************************
 *  Macros for model patterns. Example components.
 * ******************************************************** */

	Source* selector(Source*s0, Source*s1, char*keys, char*name)
	{
		printf("Use [%s] keys to switch %s\n", keys,name);
		KeyboardHit1*kb=new KeyboardHit1(keys,0);
		Source*wch=kb->lineout("char");

		NEWC(sw, Switch2, (false, false));
		ATT(sw, input0, s0);
		ATT(sw, input1, s1);
		ATT(sw, switch, wch);

		return sw;
	}
	Source* selector3(Source*s0, Source*s1, Source*s2, char*keys, char*name)
	{
		printf("Use [%s] keys to switch %s\n", keys,name);
		KeyboardHit1*kb=new KeyboardHit1(keys,0);
		Source*wch=kb->lineout("char");

		NEWC(sw, SwitchN, (3));
		ATT(sw, input0, s0);
		ATT(sw, input1, s1);
		ATT(sw, input2, s2);
		ATT(sw, switch, wch);

		return sw;
	}

	Source*boost(Source*ss, double sc)
	{
		NEWC(b, LinearDistortion, (sc,0));
		ATT(b, input, ss);
		return b;
	}

	#define KEYB(kbhit,nfreq) 	\
		KeyboardHit1*kbhit=new KeyboardHit1(KeyboardHit1::_StandardMap,KeyboardHit1::_StandardOffset);	\
		Component*nfreq=new NoteFreqS();	\
		{Source*note=kbhit->lineout("char");	\
		nfreq->attach("note",note);	\
		nfreq->attachConstant("shift",0);}


	#define TIMER(srate) timer=new Timer();timer->reattachConstant("freq",srate);





/* ****************************************************** *
 *              excerpt of include files                  *
 * ****************************************************** */


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

/******************
 *   Percussions  *
 ******************/
void rythms()
{

	char*playKeys1="zsxdcvgbhnjm,l.;/q2w3e4rt6y7ui9o";//-[]
	char*bassDrumKeys="~`";
	char*HihatKeys="1!";
	char*SnareKeys="\t";
	char*pvsKeys="-=";
	char*threeStateKeys="p[]";

	printf("keys:\n ~ for bass drum\n 1 for Hihat\n <tab> for Snare\n - and = for instrument select (Plucked vs String)\n p[] for tuning flang mix\n * be sure to deactivate Caps-Lock *\n");

	TIMER(22050); //Snare doesn't work in higher rates

	NEWC(bd, BassDrum_K909_1, ());
	ATT(bd, amp, 1);
	NEWC(bdkey, KeyboardHit1, (bassDrumKeys,0));
	ATT(bd, time, bdkey);

	NEWC(hh, Hihat_1, ());
	NEWC(hhkey, KeyboardHit1, (HihatKeys,0));
	ATT(hh, amp, 1);
	ATT(hh, time, hhkey);

	NEWC(sn, Snare_1, ());
	NEWC(snkey, KeyboardHit1, (SnareKeys,0));
	ATT(sn, time, snkey);

	NEWC(string, String1, ());
	ATT(string, amp, 0.25);

	NEWC(skey, KeyboardHit1, (playKeys1,-12*0));
	Source*note=skey->lineout("char");
	NEWC(nfreq,NoteFreqS,());
	ATT(nfreq, note, note);
	ATT(nfreq, shift, 12);
	
	ATT(string, freq, nfreq);
	ATT(string, time, skey);

	NEWC(mix, Mix, (4));
	ATT(mix, input0, bd->getLineout());
	ATT(mix, input1, .0);	//It is attached later
	ATT(mix, input2, sn->getLineout());
	ATT(mix, input3, hh->getLineout());

	Aggregate* pluck = new KarplusStrongPlucked1();
	pluck->attach("freq",nfreq);
	pluck->attach("time",skey);
	pluck->attachConstant("gain", 1.0);
	RATT(mix, input1,
		selector(pluck->getLineout(),string->getLineout(),pvsKeys,"Plucked String vs FM String"));

	NEWC(mc, Cache, ());
	ATT(mc, input, mix);

	//if flang
	NEWC(flng, Flanger1, (Flanger1::TRIANGLE,
		0.8*2 // /1.5
		,
		/*0.060*2*/
		1/(0.8*2)/4/2*2, //octave=1/f/4 (in triangle) octave=1/f/2 (in saw)
		0.01,0));	//default params
	ATT(flng, input, mc);

	NEWC(tk, KeyboardHit1, (threeStateKeys,0));
	NEWC(threestate, LazyLinear, (0.5,0));
	ATT(threestate, input, tk->lineout("char"));

	NEWC(mixorig, MixAmp0_Exclusive, ());
	ATT(mixorig, input, flng);
	ATT(mixorig, original, mc);
	ATT(mixorig, gain, threestate);

	PLAYMONO( mixorig);//mc
}


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

/**********************************************************
 * A demo setup of the Karplus-Strong Plucked String      *
 * model of string instruments                            *
 **********************************************************/
void karplus()
{

	TIMER(22050);

	NEWC(kbhit, KeyboardHit1, (KeyboardHit1::_StandardMap,-12));
	Source*note=kbhit->lineout("char");

	bool tiu_=false;
	Component*tiu=NULL;
	if(tiu_)
	{
		const int N=4;
		real Y[N] = {-12, -12,    0,   0 };
		real X[N] = {-1,  0,  0.03,  1 };
		tiu=new PartialLinearDistortion(N,X,Y);
		tiu->attach("input",kbhit);
	}

	NEWC(nfreq,NoteFreqS,());
	ATT(nfreq, note, note);
	if(tiu_)
		ATT(nfreq, shift, tiu);
	else
		ATT(nfreq, shift, .0);

	NEWC(pluck, KarplusStrongPlucked1, ());
	ATT(pluck,freq,nfreq);
	ATT(pluck,time,kbhit);
	ATT(pluck,gain, 1.0);
	Source*o=pluck->getLineout();

	bool reverb=true;
	if(reverb)
	{
		NEWC(rvb, Reverb0, ());
		ATT(rvb, rvt, 3);
		ATT(rvb, input, o);
		o=rvb->getLineout();
	}


	NEWC(pp, SinPositiveOscillator, ());
	ATT(pp,freq,10);

	NEWC(so, StereoBalanceSimple, ());
	ATT(so,pos,pp);
	ATT(so,input,o);

	PLAYSTEREO( so->getLineout(0), so->getLineout(1) );
}


/******************************************************
 * "Chorus" 2 sound effect added to the instrument set up
 ******************************************************/
Component* chorus2(int N, Source*ss)
{
	Component*mix=new Mix(N);
	for(int i=0;i<N;i++)
	{
		double frq=1.5*2*pow(2,i);
		Component*chorus1=new SinPositiveOscillator();
		chorus1->reattachConstant("amp",0.003); //this is maximum value
		chorus1->attachConstant("freq",frq);

		Component*delay1=new Delay(1,false);
		delay1->attach("delay",chorus1);
		delay1->attach("input", ss);

		char n[100];sprintf(n,"input%d",i);
		mix->attach(n,delay1);
	}//BUT IT ADDS TOO MUCH NOISE
	//NOISE FIXED: (AA)
	return mix;
}

/**************************************
 * A simple time-variable line delay  *
 **************************************/
Source*flang0(Source*ss)
{
	Component*sinosc=new SinOscillator();
	Component*flang=new LazyLinear(0.001/2,0.001/2);	//(0.5,0.5);  //insectly
	flang->attach("input",sinosc);
	sinosc->attachConstant("freq",3.0*2/* *3 */);
	Component*delay=new Delay(2,true);
	delay->attach("delay",flang);
	delay->attach("input",ss);

	return delay;
}

void EffectPlayAFile2(char*filename)
{
	TIMER(22050);
	NEWC(wavin, RawWaveIn8uMono,(filename,22050,0,false));
	RATT(wavin,time,new KeyboardHit1("qwertyuiopzxcvbnm,./asdfghjkl;'[]",0));

	NEWC(flng, Flanger1, (Flanger1::TRIANGLE,
		0.8*2 *3,
		/*0.060*2*/
		1/(0.8*2)/4/2*2 /2/2, //octave=1/f/4 (in triangle) octave=1/f/2 (in saw)
		0.01,0));	//default params
	ATT(flng, input, wavin);

	PLAYMONO(selector(wavin,flng->getLineout(),"`1","Triangular Flanger"));
}

void EffectPlayAFile(char*filename)
{
	TIMER(22050);
	NEWC(wavin, RawWaveIn8uMono,(filename,22050,0,false));
	RATT(wavin,time,new KeyboardHit1("qwertyuiopzxcvbnm,./asdfghjkl;'[]",0));
	PLAYMONO(selector3(wavin,chorus2(2,wavin),flang0(wavin),"`12","Normal|Chorus|Flanger"));
}

/******************************************************
 * "Chorus" "LL" sound effect added to the instrument set up
 ******************************************************/

Component* chorusLL(int N, Source*ss)	//with low latency
{
	Component*mix=new Mix(N);
	for(int i=0;i<N;i++)
	{
		double frq=4*3.0*2*pow(2,i)/8*(1.1+rand()/(double)RAND_MAX*0.2);
		Component*sinosc1=new SinOscillator();
		Component*chorus1=new LazyLinear(0.001/2/4,0.001/2/4);
		chorus1->attach("input",sinosc1);
		sinosc1->attachConstant("freq",frq);
		sinosc1->reattachConstant("phase",rand()/(double)RAND_MAX*2*3.14);
		Component*delay1=new Delay(1,false);
		delay1->attach("delay",chorus1);
		delay1->attach("input", ss);

		char n[100];sprintf(n,"input%d",i);
		mix->attach(n,delay1);
	}//BUT IT ADDS TOO MUCH NOISE
	//NOISE FIXED: (AA)
	return mix;
}


/*****************************************************
 * An instrument with COOL NICE tiny voice (Nov 2000)
 *****************************************************/
void pluckedTiny()	//18AzarThur1379 03:59pm-05:57pm
{
	TIMER(44100);

	KEYB(kbhit,nfreq);

		Component* asr=new ASR(9.0);
		asr->attachConstant("rise",0);
		asr->attachConstant("dur", 0);
		asr->attachConstant("rel",0.05);
		asr->reattachConstant("amp",0.10);
		asr->attach("time",kbhit);

		Component *randh=new RANDH(1.0/2.0);
		randh->reattach("amp",asr);
		Component*rs=randh;

		Component*nfd=new Divide();
		nfd->attachConstant("input",1);
		nfd->attach("by",nfreq);

	Component*mix=new Mix(2);
	mix->attach("input0",rs);


	Source*d=nfd;	//delay amount
	Component*fbc0=new Cache();
	Delay*delay=new Delay(1,false);
	fbc0->attach("input",delay);
	delay->attach("delay",d);
	delay->attach("input",mix);

	Component*ff=new LinearDistortion(0.9,0);
	ff->attach("input",fbc0);
	mix->attach("input1", ff);

	PLAYMONO(boost(fbc0/*mix*/,4*4));

}

/******************************************************
 * Standard "Chorus" sound effect
 ******************************************************/

Component* chorus(int N, Source*ss)
{
	Component*mix=new Mix(N);
	for(int i=0;i<N;i++)
	{
		double frq=3.0*2*pow(2,i)/8*(1.1+rand()/(double)RAND_MAX*0.2);
		Component*sinosc1=new SinOscillator();
		Component*chorus1=new LazyLinear(0.001/2,0.001/2);
		chorus1->attach("input",sinosc1);
		sinosc1->attachConstant("freq",frq);
		sinosc1->reattachConstant("phase",rand()/(double)RAND_MAX*2*3.14);
		Component*delay1=new Delay(2,false);
		delay1->attach("delay",chorus1);
		delay1->attach("input", ss);

		char n[100];sprintf(n,"input%d",i);
		mix->attach(n,delay1);
	}//BUT IT ADDS TOO MUCH NOISE
	//NOISE FIXED: (AA)
	return mix;
}

/******************************************************
 * Plucked String with Chorus effect
 ******************************************************/
void pluckedChorus()
{
	TIMER(44100);

	KEYB(kbhit,nfreq);

	NEWC(pluck, KarplusStrongPlucked1, ());
	ATT(pluck, gain, 1);
	ATT(pluck, freq, nfreq);
	ATT(pluck, time, kbhit);

	Cache*ss=new Cache();
	ss->attach("input",pluck->getLineout()/*ssi*/);

	Source*mix=selector(ss,chorus(2,ss),"`1", "Chorus");

	//adds TANIN
	const M=5;
	real ptsX[M] = { -1.0, -0.5, 0, 0.5, 0.1 };
	real ptsY[M] = { -1.0, -1.0, 0, 1.0, 1.0 };
	Distortion*testPLi = new PartialLinearDistortion(M,ptsX, ptsY);
	testPLi->attach("input",mix);

	PLAYMONO(boost(
		//mix
		testPLi
	,0.6));
}



/******************************************************
 * Plucked String with Flanger effect
 * //after ACM
 ******************************************************/
void pluckedFlang()	//10AzarThur1379 03:40 to-have-done-something-for-today
{
	TIMER(44100);

	KEYB(kbhit,nfreq);

	NEWC(pluck, KarplusStrongPlucked1, ());
	ATT(pluck, gain, 1);
	ATT(pluck, freq, nfreq);
	ATT(pluck, time, kbhit);
	Source*ss=pluck->getLineout();

	Source*s=selector(ss,flang0(ss), "`1", "Flanger");
	PLAYMONO(boost(s,0.6));
}
/******************************************************
 * Retro instrument
 ******************************************************/
void retro1()
{
	timer=new Timer();
	timer->reattachConstant("freq",44100);

	KeyboardHit1*kbhit=new KeyboardHit1(KeyboardHit1::_StandardMap,KeyboardHit1::_StandardOffset);
	Source*note=kbhit->lineout("char");
	Component*nfreq=new NoteFreqS();
	nfreq->attach("note",note);
	nfreq->attachConstant("shift",0);	//necassary

	Aggregate*pluck=new PluckedInstrument0();
	pluck->attach("freq",nfreq);
	pluck->attach("time",kbhit);
	Source*po=pluck->getLineout();

	Component*refl=new RetroAS1_Reflection_Sparse();
	refl->attach("input",po);

	Component*lp=new Filter1();
	lp->attach("input",refl);
	lp->attachConstant("freq",2000/4/3);
	lp->attachConstant("width",2000/3);


	PLAYMONO(lp);
};

/******************************************************
 * Cache is a standard component for defining feedback loops
 *  This prevents multiple evaluations.
 ******************************************************/

Component*cache(Source*s)
{
	Component*c=new Cache();
	c->attach("input",s);
	return c;
}

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

void impulse2()	//using FIRFew0
{
	timer=new Timer();
	timer->reattachConstant("freq",44100);

	real impulse[]={ 0, +0.1, 0.5, +0.52, 0.53};
	real FIRgain[]={ 1, -0.5, 0.3, -0.2, 0.1};
	int irn=sizeof(FIRgain)/sizeof(real);
	Component*ec=new FIRFew0(1.0,irn,impulse,FIRgain);
	KeyboardHit1*kbhit=new KeyboardHit1(KeyboardHit1::_StandardMap, KeyboardHit1::_StandardOffset);
	Component*nfreq=new NoteFreq();
	nfreq->attach("note",kbhit->lineout("char"));
	Aggregate*pluck=new PluckedInstrument0();
	pluck->attach("freq",nfreq);
	pluck->attach("time",kbhit);
	Source*pout=pluck->getLineout();
	ec->attach("input",pout);
	//WP32_src=ec;
	attachGlobalOutput(GeneralOutputFilter(ec));
	//WP32_play();
	Action();
	quit();
}

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

/******************************************************
 * A selection of 10 preset circuits / setups 
 * to play music, percussion, live audio effects 
 * on micriphone, etc
 ******************************************************/

void select10()
{
	int select=-1;
	printf(" 1=plucked\n 2=whistle\n 3=plucked2\n 4=plucked3(complex)\n 5=plucked4\n 6=impulse1\n 7=impulse2\n"
		" 8=VOSIM(1)\n 9=VOSIM(2)\n 10=retro\n");
	printf("Select Synthesiser Model [1..10]:");
	scanf("%d",&select);

	setRealtimePriority();

	switch(select)
	{
	case 1: plucked1();break;
	case 2: whistle1(true,true);break;
	case 3: plucked2();break;
	case 4: plucked3(
				44100,//sample rate
				true,//modulation
				true,//elastic
				true,//detune
				true,//reverb
				0//tranpose
				);	//cool complete pluck
			break;
	case 5: plucked3( 22050,true,true,true,true,0);	//all features
			break;
	case 6: impulse1();break;//heavy
	case 7: impulse2();break;
	case 8: VOSIM1();break;
	case 9: VOSIM2();break;
	case 10: retro1();break;
	default:
		quit();
	}
	quit();
}

//Cut ultra-sound ...
Source*GeneralOutputFilter(Source*x)
{
	NEWC(clip, ClipHardSimpleOutput, (0.9));
	ATT(clip, input, x);

	NEWC(lp, LowPassButterworth, ());
	ATT(lp, freq,10000);//20000);
	ATT(lp, input, clip);
	return lp;
}

#define ADDITEM(nm, name) {strcat(names, "  " ); strcat(names, #nm ); strcat(names, " = " ); strcat(names, #name ); strcat(names, "\n" );if(maxi<nm)maxi=nm;}






/* ****************************************************** *
 *                         main file                      *
 * ****************************************************** */


#include<string.h>

void __cdecl main()
{
	int select=-1;
	int maxi=1;
	char names[80*128]="";


	ADDITEM(1, pluckedChorus);
	ADDITEM(2, pluckedFlang);
	ADDITEM(3, pluckedTiny);
	ADDITEM(4, EffectOnFile(WishU.pcm));
	ADDITEM(5, EffectOnFile2(WishU.pcm));
	ADDITEM(6, rythms);
	ADDITEM(7, pluck2(g2.pcm, .3,1, false, 0));
	ADDITEM(8, pluck2(g3.pcm, .5,1, false, 0));
	ADDITEM(9, pluck2(g1.pcm, factor=.3,falloff=0.99, distort=false, tune key=0));
	ADDITEM(10, pluck2(b3.pcm, .8,0.999, false, 0));
	ADDITEM(11, karplus);
	ADDITEM(12, space_pluck(NULL,0.01,false));
	ADDITEM(13, space_pluck(prcpluc4.raw,0.01 ,true));
	ADDITEM(14, whistle1(true,true));

	ADDITEM(15, plucked1);
	ADDITEM(16, plucked2);
	ADDITEM(17, plucked3(rate=44K,modulation,elastic,detune,reverb,key=0)	);
	ADDITEM(18, plucked3( 22K,true,true,true,true,0));
	ADDITEM(19, impulse1);
	ADDITEM(20, impulse2);
	ADDITEM(21, VOSIM1);
	ADDITEM(22, VOSIM2);
	ADDITEM(23, retro1);


	printf(names);
	printf("Select Synthesiser Model [1..%d]:",maxi);
	scanf("%d",&select);
	setRealtimePriority();
	switch(select)
	{

		case 1: pluckedChorus();break;
		case 2: pluckedFlang();break;
		case 3: pluckedTiny();break;
		case 4: EffectPlayAFile("rawin\\WishU.pcm");break;
		case 5: EffectPlayAFile2("rawin\\WishU.pcm");break;
		case 6: rythms();break;
		case 7: pluck2(	"rawin\\g2.pcm", .3,1, false, -12*3*0+0.1);break;//great piano/guitar
		case 8: pluck2(	"rawin\\g3.pcm", .5,1, false, 0.1);break;
		case 9: pluck2(	"rawin\\g1.pcm", .3,0.99, false, 0.1);break;
		case 10: pluck2("rawin\\b3.pcm", 0.8,0.999,	false,-12*1*0+0.1);break;
			 //excitation signal, factor,falloff, distort, tune key
		case 11: karplus();break;
		case 12: space_pluck(NULL,0.01,false);break;	//COOLEST
		case 13: space_pluck("rawin\\prcpluc4.raw",0.01 /* *2*5*5 */,true);break;

		case 14: whistle1(true,true);break;

		case 15: plucked1();break;
		case 16: plucked2();break;
		case 17: plucked3(
					44100,//sample rate
					true,//modulation
					true,//elastic
					true,//detune
					true,//reverb
					0//tranpose
					);	//cool complete pluck
				break;
		case 18: plucked3( 22050,true,true,true,true,0);	//all features
				break;
		case 19: impulse1();break;//heavy
		case 20: impulse2();break;

		case 21: VOSIM1();break;
		case 22: VOSIM2();break;
		case 23: retro1();break;

	}
	quit();
}
