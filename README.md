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

A minimalistic "Howto" is available
[here](https://github.com/jserot/rfsm-light/blob/master/doc/using.md)

This sequence of short videos can be used as tutorials
1. [This video](https://github.com/jserot/rfsm-light/releases/download/1.3.1/0-Chrono.mov) is an
   introduction, describing a simple model of a stopwatch, to be used in the sequel
2. [This video](https://github.com/jserot/rfsm-light/releases/download/1.3.1/1-SaisieModele.mov) and
   [this video](https://github.com/jserot/rfsm-light/releases/download/1.3.1/2-SaisieModele-suite.mov)
   briefly describe the GUI and show of to enter the model
3. [This video](https://github.com/jserot/rfsm-light/releases/download/1.3.1/4-Visualisation.mov)
   shows how to redisplay the entered model in a nice way using [Graphviz](http://www.graphviz.org)
4. [This video](https://github.com/jserot/rfsm-light/releases/download/1.3.1/5-Simulation.mov) shows
   how to simulate the model and view simulation results using [gtkwave](http://gtkwave.sourceforge.net)
5. [This video](https://github.com/jserot/rfsm-light/releases/download/1.3.1/6-GeneratioCode.mov)
   shows how to translate the model into C, SystemC or VHDL code 

## INSTALLATION

### Using binary versions

Prebuilt Windows and MacOS (Intel and M1) versions can be downloaded
[here](https://github.com/jserot/rfsm-light/releases) (check the `Assets` tab).

For Windows, a quick Install Guide is available
[here](https://github.com/jserot/rfsm-light/blob/master/dist/windows/windows-install-guide.pdf)

**Note** the [Graphviz](http://www.graphviz.org) package and the [gtkwave](http://gtkwave.sourceforge.net)
application (used, respectively, to display diagrams and viewing simulation results) are _not_
included in the distribution and must be installed separately. If you cannot (or don't want) install
the `Graphviz` package, versions of **Rfsm-Light** using the [QGV](https://github.com/nbergont/qgv)
library instead are provided (the corresponding files have the `_qgv` suffix in the [corresponding
directory](https://github.com/jserot/rfsm-light/releases). These versions are provided only as
workarounds because the rendering is poorer than when using the external `Graphviz` package. 

### Building from source 

#### Pre-requisites

* [ocaml](http://ocaml.org) (version>=5.0) with latest version of the following [opam](http://opam.ocaml.org) packages installed:
  - [dune](http://opam.ocaml.org/packages/dune) (version>=3.11)
  - [rfsm](http://opam.ocaml.org/packages/rfsm) (version>=2.1)
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
