* GUI
- unify handling of QGraphicsView-based dot viewer and custom ImageViewer in mainwindow.cpp
- When reading a `.fsd` file, do _not_ clear the current model before the new one is  successfully
  read (this will need two fsm objects : the current one and the new one; swap only if read succeeds)
- add sized ints
- Allow attachement of priorities to transitions
- Change cursor shape according to currently selected tool
- Draw state boxes with round corners
- VHDL and SystemC syntax highlighters
- Automatic adjustement of version number in About window
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

