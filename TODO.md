* GUI
- after clicking Add in a a dynamic panel, set focus on the first row item (ledit generally)
- add sized ints
- add type_coercions in accepted expressions
- Allow attachement of priorities to transitions
- VHDL and SystemC syntax highlighters
- Allow multi-FSM models (use SCC-based partitionning of the graph)
- Draw state boxes with round corners
- Allow resizing of state boxes
- Export to SCXML ?
- reject multiple state valuations (ex: `o=1 o=2`) ?
- reject actions when the dst state has a valuation for an output modified by the action (ex: `cond / o:=1 -> S[o=2]`) ?
- Note: ... the two latter should probably be handled by an external, _semantic aware_ checker, the
  internal check focusing on _syntactic_ checking

* BUILD

* TOOLS
- rewrite the parsers combination operators `(++)` and `(+++)` in _monadic_ style 
- automatically derive the `rscheck` parsers from those specified in the `rfsm` language ...
  ... OR: rely on a full-fledged checker (with type checking !) integrated within the next `rfsm` distro (2.1)

* DOC
- update figs in Readme.md
- document restrictions compared to the full RFSM language
- make several videos for showing different features (entering a model, generating code, ...)

