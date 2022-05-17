/* **************************************************************** *
 *    mehr32
 *    Author: Sohail Siadatnejad
 *    (c) as in March 2001
 *
 * Note: This listing is only a selection of files (used for my BSc report)
 * the following functions were originally in separate include files.
 * The original file contains 14800 lines of code (as in early 2001).
 * *************************************************************** */


#include "reconstruction/src/core/base/base.h"

#include <stdio>

// #include <conio.h>
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

#include "reconstruction/src/easy/keyb.h"  // #define KEY

#include "reconstruction/src/easy/timer.h" // TIMER



/* ****************************************************** *
 *              excerpt of include files                  *
 * ****************************************************** */

#include "./PluckIns.h" // void pluck2(...)

#include "reconstruction/src/circuits/instruments/percussions.h" // rename: rythms.h  // void rythms()
#include "reconstruction/src/circuits/instruments/space_pluck.h" // void space_pluck()
// #include "reconstruction/src/circuits/space_pluck.h"

#include "reconstruction/src/circuits/demo_setups/karplus.h" // void karplus()

#include "reconstruction/src/circuits/components/chorus2.h" // Component* chorus2(...)

#include "reconstruction/src/circuits/components/flang.h" // Source*flang0(...)


#include "reconstruction/src/demo-setups/effect_play_a_file2.h"  // EffectPlayAFile2  (PLAYMONO)

#include "reconstruction/src/demo-setups/effect_play_a_file1.h" // EffectPlayAFile()  (PLAYMONO)

#include "reconstruction/src/circuits/components/chorus_ll.h" // Component* chorusLL(...)

#include "reconstruction/src/demo-setups/plucked_tiny.h" // pluckedTiny() (PLAYMONO)

#include "reconstruction/src/circuits/components/chorus1.h" // Component* chorus

#include "reconstruction/src/demo-setups/plucked_chorus.h" // pluckedChorus()  PLAYMONO


#include "reconstruction/src/demo-setups/plucked_flang.h" // pluckedFlang() (PLAYMONO)

#include "reconstruction/src/circuits/instruments/retro1.h"


#include "reconstruction/src/circuits/components/cache.h" // Component* cache()

#include "reconstruction/src/demo-setups/vosim2.h"
#include "reconstruction/src/demo-setups/vosim1.h"

#include "reconstruction/src/demo-setups/impulse2.h"

#include "reconstruction/src/demo-setups/impulse1.h"
#include "reconstruction/src/demo-setups/plucked3.h"

#include "reconstruction/src/demo-setups/plucked2.h"

#include "reconstruction/src/demo-setups/whistle1.h" // very realistic
#include "reconstruction/src/demo-setups/plucked1.h"

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

#include "reconstruction/src/circuits/components/level2/remove_ultrasound.h" // GeneralOutputFilter()

#include "reconstruction/src/easy/add_item.h"




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
