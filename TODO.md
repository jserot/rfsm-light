* GUI
- fix bug when changing src/dst state when editing a transition
- when deleting an initial state, the phantom init state is not deleted
- add sized ints
- Allow attachement of priorities to transitions
- Draw state boxes with round corners
- VHDL and SystemC syntax highlighters
- Allow multi-FSM models (use SCC-based partitionning of the graph)
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

