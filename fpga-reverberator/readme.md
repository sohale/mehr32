# A Standalone Realtime Digital Audio Reverberator

Proposal:
* [pdf](../reconstruction/docs/25-fdc-sohail.pdf)
* [original](https://web.archive.org/web/20060714033039/http://ce.sharif.edu/~fdc/proposals/25.pdf)
(archieved 2004)



* Other Links
   * https://web.archive.org/web/20060714032855/http://ce.sharif.edu/~fdc/comments.html
   * https://web.archive.org/web/20060712053344/http://ce.sharif.edu/~fdc/sample.html
   * https://web.archive.org/web/20060712053251/http://ce.sharif.edu/~fdc/time.html
   * https://web.archive.org/web/20060709182237/http://ce.sharif.edu/~fdc/

## Proposal

#### Title:
A Standalone Realtime Digital Audio Reverberator

This is a device used in music and audio broadcasting studios for adding virtual acoustic ambience and reverberation to input audio. Audio input and output is in digital serial format, S/PDIF.
#### FPGA role:
This is a fully digital device and it can be detached from PC after being programmed. "SPDIF protocol" and "calculations" for filters like low pass, band pass, delay, linear interpolation and feedback are implemented with FPGA. It will access its RAM as buffer for delay-line based effects.
#### Other applications:
It can be extended for other audio processing effects. Because of using S/PDIF it can be extended for professional studio and broadcast audio. Can be used for signal processing project for use with high quality (CD quality) sound and music. Appropriate for commercial use.
#### At presentation:
The programmed FPGA attached to (at least) two S/PDIF devices for input and output (can be Creative or Zoltrix soundcards for i/o or any CD drive for input) will add effects like echo and reverberation to input sound and generates studio quality stereo audio. Some parameters will be adjustable with + & - and similar buttons in realtime. It’s ability, applying signal processing filters on input audio (realtime) will be shown. Response time or Latency between input and output will be measured.

#### Needed peripherals:
a FPGA kit with related software and compilers (both mentioned models are applicable), SRAM or DRAM: At least 64KB, 20-100 ns, any available chip can be used,
S/PDIF standard specifications:
• IEC 958 (1989), Digital audio interface, Geneva, Switzerland: International Electrotechnical Commission (1989).
A CD drive and a Creative sound card: just for its test and demonstration.
#### Note:
Software implementation of such device on x86 based PC, was my B.Sc. final project (in Software Engineering). To be practicaly useful in music, it has to be hardware implementated.
If I am not eligible to participate in this contest (due to passing registration deadline), I would like to be supported by computer engineering faculty, with its fascilities (like FPGA board and related compilers).

Sohail Siadat-nejad
Computer Science MSc student,
Math dept, Sharif university
2003

## Solution:

### Diagram of components

SPDIF input

IIR filter

SPDIF output

crystal clock (40 MHz)

SRAM

### Results
Latency: Practically Zero latency ( 70 micro-seconds; 0.07 millisecond): one sample in, one sample out because of the nature of SPDIF, for 44,100 stereo audio CD quality)


Prize: First rank in FDC 2003 (Sharif First FPGA Design Contest). Won 4 "gold coins". (A second team also won, but they didn’t deliver a finished working product despite being a full team, and all those teams were from SUT, the well-known rank 1 technical university: Sharif University of Technology ).

A report was presented: “Standalone Audio Reverberator with SPDIF” (2003).


### Solution specifications:
FPGA (intel/Altera CPLD Max)
VHDL
Synthesis and development on: Max Plus II
Dev kit: Altera development board
SPDIF for interfacing with commercial professional audio devices (demo with connecting it to CD player and a commercial SPDIF commercial soundboard )
Audio quality: CD

Skills: VHDL, FPGA, CPLD, Max Plus II, Altera, SPDIF (Sony/Philips Digital InterFace), Commercial Digital Audio, Digital Filter Design, real-time, low-latency, Computer Architecture, Hardware Design, Hardware Product Design.

https://web.archive.org/web/20050210011446/http://ce.sharif.edu/~fdc/
Closely related to this other project of mine: https://github.com/sohale/mehr32

### Original report
### Original solution
I don't have access to the solution files currently. But I can enquire to provide them if needed.
