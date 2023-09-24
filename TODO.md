* GUI
- add a horizontal spacer to the properties panel
- use `setInputMask` for IO names to fordid syntax errors on IO names (check rsfm syntax)
- rename `fsm` to `model` (this is the _underlying_ model)
- When reading a .fsd file, do _not_ clear the current model before the new one is  successfully
  read (this will need two fsm objects : the current one and the new one; swap only if read succeeds)
- ... more generally rewrite the all thing using a proper MVC approach
- Allow resizing of the main canvas 
- Fix toolbars alignment (left, center, right)
- Allow attachement of priorities to transitions
- Change cursor shape according to currently selected tool
- Draw state boxes with round corners
- VHDL and SystemC syntax highlighters
- Automatic adjustement of version number in About window
- Use Dot widget instead of translating to gif via external command (links in etc/doc)
- Export to SCXML ?
- Re-implement IO handling with a List (or Table) ModelView ?
- When reading from file, do not erase existing model before file reading completed successfully
- Allow resizing of state boxes

TOOLS
- rfsmlint : remove

