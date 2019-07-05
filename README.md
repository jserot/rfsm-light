## RFSM-Light 

![](./doc/snapshot.png)
![](./doc/wave.png)

**RFSM-Light** is a simplified version of the [RFSM](https://github.com/jserot/rfsm) State Diagram
simulator and compiler. 

The main difference is the ability to describe state diagrams graphically using an
interface similar to that used in the [SSDE](https://github.com/jserot/ssde) editor.

On the other hand, and contrary to *RFSM*, **RFSM-Light** only handles systems composed of a *single
FSM* and hence does not support the abstraction facilities offered by the full RFSM language.
 
Apart from these differences, the underlying concepts and capabilities of the tools are similar.  In
particular, starting from a model of a system as a *reactive finite state machine* &mdash; *i.e.* a
set of *states* connected by *transitions* triggered by an *event* and a set boolean *guards* and
triggering a sequence of *actions* &mdash; *RFSM-Light* can

- generate a DOT representation of the model (to be viewed with [Graphviz](http://www.graphviz.org) for example)

- simulate the behavior of the model in response to stimuli attached to inputs (producing `.vcd`
files to be viewed using the [gtkwave](http://gtkwave.sourceforge.net) software

- generate description of the system (including *testbench* code when stimuli are attached to
  inputs) in

    - `CTask` (a C dialect with primitives for describing event-based synchronisation)

    - `SystemC`

    - `VHDL` 

## DOCUMENTATION

There's no user manual for the moment. Meanwhile, a minimalistic "Howto" is available
[here](./doc/using.md).

## INSTALLATION

Prebuilt Windows and MacOS versions can be downloaded [here](https://github.com/jserot/rfsm-light/releases/tag/v0.1a)

See the `INSTALL` file to build from sources.
