* GUI
- add sized ints
- add type_coercions in accepted expressions
- Allow attachement of priorities to transitions
- VHDL and SystemC syntax highlighters
- Allow multi-FSM models (use SCC-based partitionning of the graph)
- Allow resizing of state boxes
- Export to SCXML ?

* BUILD

* TOOLS

* DOC
- update figs in Readme.md
- document restrictions compared to the full RFSM language
- make several videos for showing different features (entering a model, generating code, ...)

* PLATFORM-SPECIFIC ISSUES
- on MacOS, the right-click emulation sequence on a trackpad (two fingers + click) on an item to
  edit it triggers a bug (the GUI becomes essentially non-responsive).
  Here's a sequence to reproduce the bug :
  - start a new design
  - add a new state, validate it with `Done`
  - select the `select` (arrow) tool and right-click on the state
  - hit `Cancel`
  - it is now impossible to change to tool, add an input, etc (keyboard-triggered actions like Save,
    ... are still operational though)
   *Note* : this bug does _not_ show if item editing is performed by using the `Ctl+Click` sequence 
