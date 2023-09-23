# Changes

# 1.3.0 (Sep XX, 2023)

* Builds and run on Mac M1 platforms withMac OS >= 12.6
* Now uses `rfsm` compiler v2.0
* Removed external syntax verifier (thx to new error reporting mechanism in rfsmc-2.0)
* Custom (and hopelfylly more user friendly) dialog for speciying input stimuli

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
