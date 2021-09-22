include config
include platform

QMAKE=qmake

.PHONY: build install clean doc

all: build

build:
#	cat src/gui/builtin_options.txt src/compiler/options_spec.txt > src/gui/options_spec.txt
ifeq ($(PLATFORM), windows)
	make -f Makefile.windows build
endif
ifeq ($(PLATFORM), macos)
	make -f Makefile.macos
endif
ifeq ($(PLATFORM), linux)
	(cd src; $(QMAKE) main.pro; make)
endif

doc: 
ifeq ($(BUILD_DOC),yes)
	(cd doc; make)
endif

html:
	pandoc -o CHANGES.html CHANGES.md
	pandoc -o README.html README.md

CHANGES.html: CHANGES.md
	pandoc -o CHANGES.html CHANGES.md
README.html: README.md
	pandoc -o README.html README.md


install:
ifeq ($(PLATFORM), macos)
	make -f Makefile.macos install
endif
ifeq ($(PLATFORM), windows)
	make -f Makefile.windows install
endif
ifeq ($(PLATFORM), linux)
	mkdir -p $(INSTALL_LIBDIR)
	cp ./platform $(INSTALL_LIBDIR)
	mkdir -p $(INSTALL_BINDIR)
#	cp $(RFSMC) $(RFSMMAKE) $(INSTALL_BINDIR)
	cp $(RFSMC) $(INSTALL_BINDIR)
#	sed -e 's,__LIBDIR__,$(INSTALL_LIBDIR),' ./etc/rfsmmake > $(INSTALL_BINDIR)/rfsmmake
#	chmod a+x $(INSTALL_BINDIR)/rfsmmake
	cp -r src/rfsm-light $(INSTALL_BINDIR)
	cp ./etc/options_spec.txt $(INSTALL_BINDIR)
	echo "COMPILER=" $(RFSMC) > $(INSTALL_BINDIR)/rfsm-light.ini
	echo "DOTVIEWER=" $(DOTVIEWER) >> $(INSTALL_BINDIR)/rfsm-light.ini
	echo "VCDVIEWER=" $(VCDVIEWER) >> $(INSTALL_BINDIR)/rfsm-light.ini
	echo "TXTVIEWER=" $(TXTVIEWER) >> $(INSTALL_BINDIR)/rfsm-light.ini
endif

###### Building the MacOS distribution

macos-dist:
	@echo "** Cleaning"
	make clobber
#	@echo "** Configuring for MacOS distribution"
#	./configure -platform macos -dot "dot" -dotviewer "open -a Graphviz" -vcdviewer "open -a gtkwave" -txtviewer "open"
	make doc
	make -f Makefile.macos build
	make -f Makefile.macos install
	make -f Makefile.macos installer

###### Building the Windows distribution

WIN_SRC_DIR=~/Desktop/SF1/Qt/rfsm-light
CURRENT_SRC_DIR=`pwd`
RFSMC_SRC_DIR=/Users/jserot/Dev/ml/rfsm/rfsmc

win-pre:
	@echo "** Preparing Windows version.."
	@echo "Building documentation"
	(cd doc; make)
	@echo "** Copying source tree"
	if [ -d $(WIN_SRC_DIR) ]; then rm -rf $(WIN_SRC_DIR); fi
	cp -r $(CURRENT_SRC_DIR) $(WIN_SRC_DIR)
	@echo "** Done"
	@echo "** Now, make win-{build,install,installer} from Windows"

win-build:
	@echo "******************************************************************************"
	@echo "**** WARNING: this make step must be invoked from a [mingw32(MSYS)] shell ****"
	@echo "******************************************************************************"
	make -f Makefile.windows

win-install:
	make -f Makefile.windows install

win-installer:
	make -f Makefile.windows installer

clean:
	(cd src; make clean)
	(cd doc; make clean)

clobber: clean
	(cd src; make clean)
	(cd doc; make clobber)
	\rm -f src/rfsm-light.app/Contents/MacOS/rfsm
	\rm -f *~
	\rm -f README.html CHANGES.html

