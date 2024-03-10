# Preamble

In any case, first build the `rfsmc` compiler

Do not forget to re-build when changing target platform from the same source directory (ex: for
MacOS Intel and Windows).

# Build MacOS M1 / QT6 version (with Graphviz, without QGV)

- `cd ~/Dev/ml/rfsm`
- `make host; make std_guest` 
- `cd ~/Dev/Qt/rfsm-light`
- `./configure -platform macos -qt_spec macx-clang -qt_version 6.3.1 -qmake /Users/jserot/Qt/6.3.1/macos/bin/qmake -qt_config arm64`
- `make macos-dist`

# Build MacOS M1 / QT6 version (without Graphviz, with QGV)

- `cd ~/Dev/ml/rfsm`
- `make host; make std_guest` 
- `cd ~/Dev/Qt/rfsm-light`
- `./configure -platform macos -qt_spec macx-clang -qt_version 6.3.1 -qmake /Users/jserot/Qt/6.3.1/macos/bin/qmake -qt_config arm64 -qgv /Users/jserot/Dev/Qt/qgv`
- `make macos-dist`

# Build MacOS Intel / QT5 version (with Graphviz, without QGV)

If the `rfsm` source tree is not present or obsolete:
  - `cd ~/Desktop/SF1/Caml`
  - `git clone https://github.com/jserot/rfsm`
Then
- `cd rfsm`
- `make host; make std_guest` 
If the `rfsm-tree` source tree is not present or obsolete:
  - `cd ~/Desktop/SF1/Qt`
  - `git clone https://github.com/jserot/rfsm-light`
Then
- `cd rfsm-light`
- `./configure -platform macos -qt_spec macx-clang -qt_version 5.8.0 -qmake /Developer/Qt5.8/5.8/clang_64/bin/qmake -qt_config x86_64`
- `make macos-dist`

# Build MacOS Intel / QT5 version (without Graphviz, with QGV)

Same as above but configure with
`./configure -platform macos -qt_spec macx-clang -qt_version 5.8.0 -qmake
  /Developer/Qt5.8/5.8/clang_64/bin/qmake -qt_config x86_64 -qgv /Users/jserot/Dev/Qt/qgv`

# Build Windows version (with Graphviz, without QGV)

If the `rfsm` source tree is not present or obsolete, on the MacOS side:
  - `cd ~/Desktop/SF1/Caml`
  - `git clone https://github.com/jserot/rfsm`
Then, from a *Gygwin terminal* on the W64 side:
  - `cd /cygdrive/z/Caml/rfsm`
  - replace `dune 3.11` by `dune 2.6` in `dune-project`
  - edit `src/host/lib/{dune,Makefile}` to de-automatize the generation of `options.ml` (see notes in files)
  - `make host; make std_guest` 
If the `rfsm-light` source tree is not present or obsolete, on the MacOS side:
  - `cd ~/Desktop/SF1/Caml; rm -r rfsm-light`
  - `git clone https://github.com/jserot/rfsm-light.git`
Then, on the W64 side, and *from a MinGW terminal* this time
  - `cd /z/Qt/rfsm-light`
  - `./configure -platform windows` (do not be alarmed if certain fields in the generated `config`
    file are wrong; they will be overriden in the sequel)
  - `make win-build`
  - `make win-install`
  - adjust version in `./dist/windows/RfsmLightSetup.iss` file
  - `make win-installer`

# Build Windows version (without Graphviz, with QGV)

Same as above but
- have first QGV library compiled in `/z/Qt/qgv`
- configure with - `./configure -platform windows -qgv /z/Qt/qgv`

# Build under Linux (ex: LinuxMint)

- if required, install `ocaml` and `opam`: `apt install ocaml`
- if required, install `Qt` (`qt5-default` is pre-installed in `LinuxMint 19`)
- install latest version of `rfsm` package : `opam install rfsm`
- clone the source dir
  - `cd /media/sf_SF2`
  - `rm -rf rfsm-light`
  - `git clone https://github.com/jserot/rfsm-light`
- build 
  - `cd rfsm-light`
  - `./configure --platform linux --no-doc`
  - `make`
  - `make install`
