* GUI
- factorize out code for StateValuations, TransitionActions and TransitionGuards panels
- show state\_valuations and transition\_{actions,guards} as pop-up panels instead of sub-panels in the properties area
- fix bug occuting sometimes (not always) when the src or dst state of a transition is modified in the transition edit panel
- add sized ints
- add type_coercions in accepted expressions
- Allow attachement of priorities to transitions
- VHDL and SystemC syntax highlighters
- Allow multi-FSM models (use SCC-based partitionning of the graph)
- Draw state boxes with round corners
- Allow resizing of state boxes
- Export to SCXML ?

* BUILD

* TOOLS
- automatically derive the `rscheck` parsers from those specified in the `rfsm` language ...
- ... OR: rely on a full-fledged checker (with type checking !) integrated within the next `rfsm` distro (2.1)

* DOC
- document restrictions compared to the full RFSM language
- make several videos for showing different features (entering a model, generating code, ...)

