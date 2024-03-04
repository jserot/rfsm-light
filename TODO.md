* GUI
- encapsulate calls to `rfsmc` compiler in a class
- add sized ints
- add type_coercions in accepted expressions
- Allow attachement of priorities to transitions
- VHDL and SystemC syntax highlighters
- Allow multi-FSM models (use SCC-based partitionning of the graph)
- Allow resizing of state boxes
- Export to SCXML ?

* BUILD

* TOOLS
- rewrite the parsers combination operators `(++)` and `(+++)` in _monadic_ style 
- automatically derive the `rscheck` parsers from those specified in the `rfsm` language ...
  ... OR: rely on a full-fledged checker (with type checking !) integrated within the next `rfsm` distro (2.1)

* DOC
- update figs in Readme.md
- document restrictions compared to the full RFSM language
- make several videos for showing different features (entering a model, generating code, ...)

