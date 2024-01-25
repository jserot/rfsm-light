* GUI
- fix bug occuting sometimes (not always) when the src or dst state of a transition is modified in the transition edit panel
- add sized ints
- Allow attachement of priorities to transitions
- VHDL and SystemC syntax highlighters
- Allow multi-FSM models (use SCC-based partitionning of the graph)
- Draw state boxes with round corners
- Allow resizing of state boxes
- Export to SCXML ?

* BUILD

* TOOLS
- pre-check guard and action syntax before sending to compiler (the location reported by
the compiler is too vague in case of syntax error in this case). I.o.w. re-vive `rfsmlint`, but in 
a simplified form (limited to expressions and assignations)

* DOC
- document restrictions compared to the full RFSM language
- make several videos for showing different features (entering a model, generating code, ...)

