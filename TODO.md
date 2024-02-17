* GUI
- merge Model{Inps,Outps,Vars} into ModelIo
- handle event notification in actions
- reject multiple state valuations (ex: `o=1 o=2`) 
- reject actions when the dst state has a valuation for an output modified by the action (ex: `cond / o:=1 -> S[o=2]`)
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

