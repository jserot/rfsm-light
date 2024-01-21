* GUI
- make switch to QGV a config/compile time option, not a GUI option
- Split IO panel in 3 : Inputs, Outputs, Vars; attach stimuli only to the first
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
- document -dot_qgv option (?)
- document restrictions compared to the full RFSM language
- make several videos for showing different features (entering a model, generating code, ...)

