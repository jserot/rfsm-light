# Build MacOS M1 / QT6 version

- `./configure -platform macos -qt_spec macx-clang -qt_version 6.3.1 -qmake /Users/jserot/Qt/6.3.1/macos/bin/qmake -qt_config arm64`
- `make macos-dist`

# Build MacOS Intel / QT5 version

- [`opam install rfsm`]
- `./configure -platform macos -qt_spec macx-clang -qt_version 5.8.0 -qmake /Developer/Qt5.8/5.8/clang_64/bin/qmake -qt_config x86_64`
- `make macos-dist`

# Build Windows version

Because `ocaml 5.0` cannot easily installed on W64 (it should be with `opam 2.2`), we cannot rely on
`opam` to install `rfsm`. Instead, it has to compiled from sources:
- `cd ~/Desktop/SF1/Caml/rfsm`
- `git clone https://github.com/jserot/rfsm`
then, from a *Gygwin terminal on the W64 side*:
- `cd /cygdrive/z/Caml/rfsm`
- replace `dune 3.11` by `dune 2.6` in `dune-project`
- edit `src/guests/std/lib/{dune,Makefile}` to de-automatize the generation of `options.ml` (see notes in files)
- `make` 
Then
- `cd ~/Desktop/SF1/Caml; rm -r rfsm-light`
- `git clone https://github.com/jserot/rfsm-light.git`
- and, on the W64 side, *from a MinGW terminal* this time
  - `cd /z/Qt/rfsm-light`
  - `./configure -platform windows`
  - `make win-build`
  - `make win-install`
  - `make win-installer`

# Build under Linux (ex: LinuxMint)

- if required, install `ocaml` and `opam`: `apt install ocaml`
- if required, install `Qt` (`qt5-default` is pre-installed in `LinuxMint 19`)
- check that the `lascar`, `rfsm` and `ppx_deriving_yojson` opam packages are installed and up to date
- clone the source dir; from MacOS:
  - `cd ~/Desktop/SF3`
  - `rm -rf rfsm-light`
  - `git clone ~/Dev/Qt/rfsm/rfsm-light`
- on the Linux side 
  - `cd /media/sf_SF/rfsm-light`
  - `./configure --platform linux --no-doc`
  - `make`
  - `make install`
