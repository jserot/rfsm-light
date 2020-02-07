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

There's no user manual for the moment.

A minimalistic "Howto" is available
[here](https://github.com/jserot/rfsm-light/blob/master/doc/using.md)

This short [video](https://youtu.be/SLB0Hw2DQ88) can also be used as a tutorial.

## INSTALLATION

### Using binary versions

Prebuilt Windows and MacOS versions can be downloaded
[here](https://github.com/jserot/rfsm-light/releases) (check the `Assets` tab).

For Windows, a quick Install Guide is available
[here](https://github.com/jserot/rfsm-light/blob/master/dist/windows/windows-install-guide.pdf)

### Building from source 

#### Pre-requisites

* [ocaml](http://ocaml.org) (version>=4.08) with latest version of the following [opam](http://opam.ocaml.org) packages installed:
  - [lascar](http://opam.ocaml.org/packages/lascar)
  - [rfsm](http://opam.ocaml.org/packages/rfsm)
* [Qt](http://www.qt.io) (version>=5.8)

#### How to build

* Get the source code: `git clone https://github.com/jserot/rfsm-light`
* `cd rfsm-light`
* `./configure [options]` (`./configure --help` for the list of options)
* `make`
* `make install` 

If you can't or don't want to build the documentation from source, pass the `--no-doc` option to
`configure`. A pre-built version is available
[here](https://github.com/jserot/rfsm-light/blob/master/doc/using.md).

Building on Windows requires [Cygwin](https://cygwin.com) or [MinGW](http://www.mingw.org) which
`gcc`, `ocaml` and `opam` installed.
