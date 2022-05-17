
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
