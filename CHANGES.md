# Changes

# 1.3.1 (May 10, 2024)

* State and transition properties are now entered and edited using modal pop-up dialogs; the GUI left
  hand panel is now static and displays the model name and IOs
* State valuations and transition guards/actions are now checked using the `-check_fragment` of the
  `rfsmc` compiler; this allows a much more complete checking (including type checking for example)
  and more informative error messages, which, in turn, reduces the possibility of writing ill-formed models
* Fixed a bug in the graphical item selection mechanism; selecting transitions, in particular, now
  works in a much more reliable way
* The cursor in the graphical panel now changes depending on the selected tool (select item, insert state, ...)
* States are now drawn with rounded corners
* Do not erase the current model and view when reading a `.fsd` file before the read is complete
* Provide versions with in-app (not requiring `Graphviz` external tools) rendering (using the `QGV` library)
* New multi-platform (macm1,macx86,windows) build system
  
  
# 1.3.0 (Dec 23, 2023)

* Builds and run on Mac M1 platforms withMac OS >= 12.6 and Qt 6.3
* Now uses `rfsm` compiler v2.0
* Removed `rfsmlint` external syntax verifier (thx to new error reporting mechanism in rfsmc-2.0)
* Custom (and hopefully more user friendly) dialog for specifying input stimuli

# 1.2.0 (Sep 20, 2021)

* Added external syntax verifier (`rfsmlint`) 

# 1.1.0 (Mar 30, 2021)

* Allow output valuations to be attached to states 

# 1.0.0 (Sep 25, 2019)
* The application is now only a GUI to the `rfsmc` compiler (which is distributed as a separate
  `opam` package). For convenience, the, distributed MacOS and Windows
  installers include pre-built binaries of the compiler)
* Building from sources under Linux is now supported (tested under LinuxMint19)

# 0.1 (Jul 5, 2019)
* First public version
