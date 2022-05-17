# orgaanisational ideas
Ideas to bring order and organisation to file structure, and even design (classes, namespaces, packages, etc)

###
Where do these belong?
If not in the same categoory, are there of different levels? Where is the hierarchy of those levels?
```C++
attachGlobalOutput(.)
Action();
quit();
```

### Avoid global variables even for master output or input
Is it driven by output? Then remove the global variable.
`RawRecorder`

// driven by soemthing else? or is a driver?

But we will need some kind of preparation of sound-cards, etc.
Will use a correct C++17-style resrouce-management paattern.
(If we had Python's `with`, it wouold have been good.)

### A goood pattern already used
But incomplete.
Give some structure constraint to impose this
```c++
  //timer->reset();
  quit();
  // aftter quit, everythhing is cleared up and we can start over by calling reset
  timer->reset();
  // quit();
```

### Examine the possibility of compile-time attriutes?
### x-time: extended
"run-" time, "stream-" time (1,2,3), etc
"simulation-" time
"GUI-" time.

### Is this event?
```C++
  void reset(); // event?
```
It is. But no queue neccessary.
Rutime mixed with "simulation-" time.
Somehow not good.
I don't like to put events all over.]

Besides, they should  be much less than samples!
"Events-"time.

### io
the components that are vanialla C++ are very different to the ones that have depenrencies such as direct-sound, etc.
They require certain rituals to prepare too.

Move all non-vanilla ones to `io` folder:
`reconstruction/src/circuits/components/io`.

But it should be sommething outside of `circuits` or `component` fodler. (in fact, they are probably not even "component"s. Also not `Source`s).

In other words, they have a hardware dependency. (Or external software).
(OOr even external libraries: boost, etc? Shoould this be a criterion)
## framework ones
Also related to io components:
core/framework.h