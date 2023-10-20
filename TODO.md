* GUI
- When reading a .fsd file, do _not_ clear the current model before the new one is  successfully
  read (this will need two fsm objects : the current one and the new one; swap only if read succeeds)
- add sized ints
- Allow attachement of priorities to transitions
- Change cursor shape according to currently selected tool
- Draw state boxes with round corners
- VHDL and SystemC syntax highlighters
- Automatic adjustement of version number in About window
- Use Dot widget instead of translating to gif via external command (links in etc/doc)
- Export to SCXML ?
- Allow resizing of state boxes

* BUILD
- add qmake target to src top Makefile

* TOOLS

* DOC
- document restrictions compared to the full RFSM language

