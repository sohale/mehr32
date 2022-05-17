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
