### Saving and loading

* FSM diagrams can be saved to (resp. read from) files with the ![](./../src/gui/images/save.png)
  (resp. ![](./../src/gui/images/open.png)) button of the toolbar (or the by invoking the `Save`, `Save As` or
  `Open` actions in the `File` menu)

* The ![](./../src/gui/images/new.png) button (`New` action in the `File` menu) clears the current diagram

### Editing 

* To **add a state**, select the ![](./../src/gui/images/state.png) button in the toolbar and click on the
  canvas

* To **add a transition**, select the ![](./../src/gui/images/transition.png) button, click on
  the start state and, keeping the mouse button pressed, go the end state and release mouse button.

* To **add a self transition** (from a state to itself) , select the ![](./../src/gui/images/loop.png) button
  and click on start state (the location of the click will decide on that of the
  transition).

* To **add an initial transition**, select the ![](./../src/gui/images/initstate.png) button
  and click on initial state 

* To **delete a state or a transition**, select the ![](./../src/gui/images/delete.png) button
  and click on the state or transition (deleting a state will also delete all incoming and
  outcoming transitions)

* To **move a state**, select the ![](./../src/gui/images/select.png) button and drag the state.

* To **edit a state or a transition**, select the ![](./../src/gui/images/select.png) button, click on
  the corresponding item and update the property panel on the right.

### Compiling

* To **generate a DOT representation of the  diagram**, click the
  ![](./../src/gui/images/compileDOT.png) button (or invoke the corresponding action in the
  `Build` menu)
  
* To **generate `CTask` code **, click the ![](./../src/gui/images/compileCTask.png) button

* To **generate `SystemC` code **, click the ![](./../src/gui/images/compileSystemc.png) button

* To **generate `VHDL` code **, click the ![](./../src/gui/images/compileVHDL.png) button

The generated graphs and code will appear as separate tabs in the right part of the window.
  
### Simulating

To **simulate the diagram** (provided that stimuli have been attached to inputs using the `IOs and
  variables`  in the left part of the window), click the ![](./../src/gui/images/runSimulation.png) button (or invoke the
  corresponding action in the `Build` menu)
  
If a valid VCD viewer (such as `gtkwave`) has been specified, simulation results will be displayed
in a separate window.

