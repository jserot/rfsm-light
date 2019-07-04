include VERSION
include config

APPNAME=rfsm-light

BIN_INSTALLED = src/compiler/rfsmc.opt

QMAKE_MACOS = /Developer/Qt5.8/5.8/clang_64/bin/qmake 
QMAKE_WIN = C:/Qt/Qt5.8.0/5.8/mingw53_32/bin/qmake.exe
MAKE_WIN = C:/Qt/Qt5.8.0/Tools/mingw530_32/bin/mingw32-make
QMAKE_UNIX=qmake

.PHONY: lib compiler gui clean install dist html

all: lib compiler gui

lib:
			(cd src/lib; make native)

compiler:
			(cd src/compiler; make native)
			mv src/compiler/main.native src/compiler/rfsmc.opt

gui:
ifeq ($(PLATFORM), win32)
	(cd src/gui; $(QMAKE_WIN) -spec win32-g++ main.pro; $(MAKE_WIN))
endif
ifeq ($(PLATFORM), macos)
	(cd src/gui; $(QMAKE_MACOS) -spec macx-clang CONFIG+=x86_64 main.pro; make)
endif
ifeq ($(PLATFORM), unix)
	(cd src/gui; $(QMAKE_UNIX) main.pro; make)
endif

txt:
	pandoc -o CHANGELOG.txt CHANGELOG.md
	pandoc -o CHANGELOG.html CHANGELOG.md
	pandoc -o README.txt README.md
	pandoc -o README.html README.md

html:
	pandoc -o README.html README.md
	open -a Safari README.html

clean:
	(cd src/lib; make clean)
	(cd src/compiler; make clean)
	(cd src/gui; make clean)
	(cd examples; make clean)

clobber: clean
	(cd src/lib; make clobber)
	(cd src/compiler; make clobber)
	(cd examples; make clobber)
	\rm -f src/gui/rfsm-light.app/Contents/MacOS/rfsm
	\rm -f *~

install:
	mkdir -p $(INSTALL_BINDIR)
	cp src/compiler/rfsmc.opt $(INSTALL_BINDIR)/rfsmc
ifeq ($(PLATFORM), macos)
	cp -r src/gui/rfsm-light.app $(INSTALL_BINDIR)/RfsmLight
else
	cp src/gui/rfsm-light $(INSTALL_BINDIR)/rfsm-light
endif

SRCTMPDIR=/tmp
SRCDISTNAME=rfsml-source
SRCDISTDIR=$(SRCTMPDIR)/$(SRCDISTNAME)
EXCLUDES=--exclude .git --exclude .gitignore --exclude .DS_Store
SRCTARBALL=$(SRCDISTNAME).tar

MACOS_DIST=/tmp/rfsm-light

macos-dist:
	@echo "** Cleaning"
	make clobber
	@echo "** Configuring for MacOS distribution"
	./configure -platform macos -dot "dot" -dotviewer "open -a Graphviz" -vcdviewer "open -a gtkwave" -txtviewer "open"
	@echo "** Building"
	(cd src/compiler; make)
	(cd src/gui; make)
	make macos-install
	make macos-installer

macos-install:
	@echo "** Installing in $(MACOS_DIST)"
	rm -rf $(MACOS_DIST)
	mkdir $(MACOS_DIST)
	cp -r src/gui/rfsm-light.app $(MACOS_DIST)/RfsmLight.app
	cp src/compiler/main.native $(MACOS_DIST)/RfsmLight.app/Contents/MacOS/rfsmc
	cp ./dist/macos/rfsm-light.ini $(MACOS_DIST)/RfsmLight.app/Contents/MacOS
	cp ./src/gui/etc/options_spec.txt $(MACOS_DIST)/RfsmLight.app/Contents/MacOS
	cp ./dist/macos/INSTALL $(MACOS_DIST)/INSTALL
	mkdir $(MACOS_DIST)/doc
#	cp -r doc/lib $(MACOS_DIST)/doc
#	cp  doc/um/rfsm.pdf $(MACOS_DIST)/doc/rfsm-manual.pdf
	mkdir $(MACOS_DIST)/examples
	cp -r examples/* $(MACOS_DIST)/examples
	make txt
	cp {CHANGELOG.txt,KNOWN-BUGS,LICENSE,README.txt} $(MACOS_DIST)

RFSM_VOLUME=RfsmLight-$(VERSION)

macos-installer:
	@echo "** Creating disk image"
	rm -f /tmp/RfsmLight.dmg
	hdiutil create -size 16m -fs HFS+ -volname "$(RFSM_VOLUME)" /tmp/RfsmLight.dmg
	hdiutil attach /tmp/RfsmLight.dmg
	cp -r $(MACOS_DIST)/RfsmLight.app /Volumes/$(RFSM_VOLUME)
	ln -s /Applications /Volumes/$(RFSM_VOLUME)/Applications
	cp -r $(MACOS_DIST)/examples /Volumes/$(RFSM_VOLUME)/Examples
	cp -r $(MACOS_DIST)/doc /Volumes/$(RFSM_VOLUME)/Documentation
	cp $(MACOS_DIST)/{CHANGELOG.txt,KNOWN-BUGS,LICENSE,README.txt,INSTALL} /Volumes/$(RFSM_VOLUME)
	hdiutil detach /Volumes/$(RFSM_VOLUME)
	hdiutil convert /tmp/RfsmLight.dmg -format UDZO -o /tmp/RfsmLight_ro.dmg
	@echo "** Copying disk image into ./binaries"
	mv /tmp/RfsmLight_ro.dmg ./binaries/RfsmLight-$(VERSION).dmg

WIN_SRC_DIR=~/Desktop/SF1/Caml

win32-pre:
	@echo "** Preparing Windows version.."
	@echo "** Cleaning source directory.."
	make clobber
	@echo "** Copying source tree"
	if [ -d $(WIN_SRC_DIR)/rfsm-light ]; then rm -rf $(WIN_SRC_DIR)/rfsm-light.bak; mv $(WIN_SRC_DIR)/rfsm-light $(WIN_SRC_DIR)/rfsm-light.bak; fi
	(cd ..; cp -r light $(WIN_SRC_DIR)/rfsm-light)
	@echo "** Done"
	@echo "** Now, make win32-{compiler,gui,install,installer} from Windows"
	@echo "** And then, back to MacOS and make win32-post"

win32-post:
	@echo "** Copying win32 installer exe in ./binaries"
	cp $(WIN_SRC_DIR)/rfsm-light/dist/windows/RfsmLight_setup.exe ./binaries

win32-compiler:
	@echo "***********************************************************************"
	@echo "**** WARNING: this make step must be invoked from a [Cygwin] shell ****"
	@echo "***********************************************************************"
	@echo "** Building compiler"
	make compiler
	@echo "** Done"

win32-gui:
	@echo "******************************************************************************"
	@echo "**** WARNING: this make step must be invoked from a [mingw32(MSYS)] shell ****"
	@echo "******************************************************************************"
	./configure -platform win32 -dot "/C/Program Files/Graphviz/bin/dot.exe" -dotviewer "/C/Program Files/Graphviz/bin/dotty.exe" -vcdviewer "/C/Program Files/gtkwave/bin/gtkwave.exe"
	@echo "** Building GUI"
	make gui
	@echo "** Done"

WIN_INSTALL_DIR=./build

win32-install:
	@echo "** Installing in $(WIN_INSTALL_DIR)"
	rm -rf $(WIN_INSTALL_DIR)
	mkdir $(WIN_INSTALL_DIR)
	cp ./src/gui/release/rfsm-light.exe $(WIN_INSTALL_DIR)
	cp ./src/gui/etc/options_spec.txt $(WIN_INSTALL_DIR)
	mkdir $(WIN_INSTALL_DIR)/bin
	cp ./src/compiler/_build/main.native $(WIN_INSTALL_DIR)/bin/rfsmc.exe
	cp ../caph/dlls/{Qt5Core,Qt5Gui,Qt5Widgets,libgcc_s_dw2-1,libstdc++-6,libwinpthread-1}.dll $(WIN_INSTALL_DIR)
	mkdir $(WIN_INSTALL_DIR)/platforms
	cp ../caph/dlls/qwindows.dll $(WIN_INSTALL_DIR)/platforms
	cp {CHANGELOG.txt,KNOWN-BUGS,LICENSE,README.txt} $(WIN_INSTALL_DIR)
	cp ./dist/windows/icons/*.{bmp,ico} $(WIN_INSTALL_DIR)
#	cp ./dist/windows/rfsm-light.ini $(WIN_INSTALL_DIR)
	mkdir $(WIN_INSTALL_DIR)/doc
	mkdir $(WIN_INSTALL_DIR)/examples
	cp -r examples/* $(WIN_INSTALL_DIR)/examples
	@echo "Done"

win32-installer:
	@echo "** Building self-installer"
	/C/Program\ Files/Inno\ Setup\ 5/iscc ./dist/windows/RfsmLightSetup.iss
