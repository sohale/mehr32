
#define TIMER(srate)   \
  timer = new Timer(); \
  timer->reattachConstant("freq", srate);
