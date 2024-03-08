Known bugs and limitations
--------------------------

* multi-FSM models are not supported (use `rfsmc` command-line compiler if required)

* Multi-events FSM models are not supported by the VHDL backend

* Synchronous interpretation of actions is not (yet) supported by the CTask and SystemC backends

* Types are limited to `event`, `int` and `bool` (sized ints available in the "full" RFSM language
  are not supported in particular)

* The concept of transition priority used in RFSM is not supported; as a result, simulation may fail due to
  non-deterministic situations.

* A portable way of building on Linux distros is still missing

* When running on a Mac M1 under Mac OS 12 - but this may also occur on other versions - when the Rfsm-Light, 
  app is launched by double-clicking, it seems unable to launch the gtkwave application. This does _not_ happen 
  when the app is launched from the terminal (by invoking "/Applications/Rfsm-light.app/Contents/MacOS/rfsm-light"
  from a terminal for example, or event "open /Applications/Rfsm-light.app").  

* When running on a Mac M1 under Mac OS 12 - but this may also occur on other versions - 
  the right-click emulation sequence on a trackpad (two fingers + click) on an item to
  edit it triggers a bug (the GUI becomes essentially non-responsive).
  Here's a sequence to reproduce the bug :
  - start a new design
  - add a new state, validate it with `Done`
  - select the `select` (arrow) tool and right-click on the state
  - hit `Cancel`
  - it is now impossible to change to tool, add an input, etc (keyboard-triggered actions like Save,
    ... are still operational though)
   *Note* : this bug does _not_ show if item editing is performed by using the `Ctl+Click` sequence 
