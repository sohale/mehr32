mehr32
======
Written in 2000. Sohail Siadat


"Mehr32" (1999 –2000): was my BSc degree project:

**“Real-time audio processing toolbox in C++ for computer music”**

Keywords: C++, Real-time, Tcl/Tk, DirectSound, Steinberg VST.


Mehr32 is a full digital Audio Engineering studio and toolbox written in C++/C.
It was my final degree project developed back in 1999-2000. The full version will be available soon)
Note that currently only the main file is provided. The library (.h) files will be added later.
Note  that this was not written in C++ ISO standards. It is using macros which are now discouraged in current C++ standards.

#### IO implemented:
* Steinberg VST
* DirectSound
* normal buffered Win32 sound output `WP32_play()`
* SPDIF (FPGA-version only)
#### Devices:
* Microphone live (as part of above)
* Audio-in live (as part of above)
* Keyobard playing (live)
* Progamatic sequence (melody)

### History
* 1999: (v1) Based on an earlier project previous year 1999 [sohale/jMusic](https://github.com/sohale/jMusic)
* 2000: (v2) Presented as BSc final project at UI (Thesis report available) <span style="color:red">this version</span>
* 2001: (v3) Further extentions: (VST plugin, Live Microphone input, granular synthesis, pitch)
* 2004?: Lost all files due to loss of a hard-disk 🥺 in pre-github era.
* 2002: FPGA implementation (2002?): SPDIF input and output fully implemented. See [^The-FPGA-version]


### Todo
* Rewrite and refactor using C++ templates.
* Revive by reconstructing from the main.cpp from the BSc Thesis file.



### Related projects:
 1.	The “Mehr32” (1999 – 2000) implemented as my BSc degree project titled “Real-time audio processing toolbox in C++ for computer music”. [C++, Real-time, Tcl/Tk, DirectSound, Steinberg VST]. Mehr32 was a C++ library implementing audio processing filters and computer music models of musical instruments for real-time performance on PCs. Example designs included: Real-time reverberator (acoustic environmental effects), string and wind instrument models, and an electric guitar amp. The software could achieve the input/output latency of down to 300ms on a Celeron 366.

 2.	The “FPGA Audio reverberator” (2002): Implemented on Altera CPLD/FPGA. Winner of the First Sharif FPGA Design Contest (FDC); I implemented a working hardware and I implemented (from scratch) both Input and Output modules for the Sony/Phillips Serial Digital audio Interface (SPDIF) protocol. The implemented device applied an IIR filter on the streaming stereo digital audio from a commercial CD player, with less than 3 samples total latency (VHDL/FLEX10K50 CPLD).


### The FPGA version
FPGA implementation: SPDIF input and output fully implemented.

Presented and won joint first rank award (2x gold coins 🟡🟡 prize) (Provided live IO fro audio players but dues too lack oof time, only the Simple FIR & IIR filters were ported to hardware / VHDL ). Here is my [proposal](http://ce.sharif.edu/~fdc/proposals/25.pdf) before winning!

**A Standalone Realtime Digital Audio Reverberator**

> This is a device used in music and audio broadcasting studios for adding virtual acoustic ... interpolation and feedback are implemented with FPGA.
From [http://ce.sharif.edu/~fdc/proposals/25.pdf](http://ce.sharif.edu/~fdc/proposals/25.pdf)

### Implemented classes:

????????? ? ???? ????? ??????
Oscillator
Phase
BasicVOSIMWaveform
BasicVOSIMWaveform0
ImpulseOneSampleOscillator
PulseABOscillator
PulseOscillator
PulseWOscillator
RetroGlottalOscillator
Sawtooth01Oscillator
Sawtooth10Oscillator
SawtoothOscillator
SinOscillator
SinPositiveOscillator
SinSquareOscillator
StomperNoiseyFreqShifter0
TriangularOscillator
PeriodicChange
PeriodicChangeFixed
RANDH
RANDI

ExponEnvelopeComponent1
ExponEnvelopeComponent2
ExponEnvelopeSolid


???? ??? convolution
RetroAS1_Reflection_Sparse
RetroAS1_Reflection_Diffuse
RetroAS1_Reflection_Random
FIRFew_Random0

????? ??????? ????? ???? ???? Wave shaper
Distortion
AbsoluteDistortion
ArcSinDistortion
BbJWaveShaper
ClipHardSimpleOutput
DeadZone0
FractionDistortion
LimitBetweenDistortion
LinearDistortion
LogarithmDistortion
MyClip
MyEnvelopeDistorter
MyOvrDrv
MyThreshold
NoiseGate1
OverDriveWaveShaperJW
PAFuzzDistortion
PartialLinearCODistortion
PartialLinearDistortion
PolynomialDistortion
PowerDistortion01div
PullDownDistortion
PullUpDistortion
SimpleExponentialDistortion
SimpleGainDistortion
SimpleLimiter
SimpleOpAmp
SinousDistortion
SoftSaturationWaveShaper
SquareRootDistortion
StomperEnvelopeDist0


??????? ????? ?? Subtractive Synthesis :
Alpha
BandPassButterworth
BandRejectButterworth
Filter1
Filter2
HighPassButterworth
JeffSmith1
LowPassAverage
LowPassButterworth
LowPassWeighted
NoDC
RMS
SimplePeakFollowerPB


??????? ????? ?? ????? ?????
DebugEven
DebugPrint
PrintAll

??????? ????? ?? ????? ????? ???? ? ????????
Cache
CacheSlow

??????? ?????
KeyboardHit
KeyboardHit1
MusicBox3
NoteFreq
NoteFreqS
RawRecorder
RawWaveIn16sMono
VariableSource
???? Wave guide
Waveguid1
??????? ????????????? ????????? ???
BassDrum_K909_1
Snare_1
String1
KarplusStrongPlucked0
KarplusStrongPlucked1
KarplusStrongPlucked2
PitchShiftMySoft1
PluckedInstrument0
PluckedInstrument1
Detune0

???? ????? ????? ?? ?????????????? ?????????? ???
BassDrum_
claps
claps2
impulse1
impulse2
karplus()
MyBlackSmith
pluck2
plucked1
plucked2
plucked3
pluckedChorus
pluckedFlang
pluckedNonlinear
pluckedTiny
Retro1
rythms
Snare1
space_pluck
VOSIM1
VOSIM2
whistle1

Chorus
ChorusLL
ChorusOnPhysical



