include ../config

build: Makefile-$(QT_VERSION)
	make -f Makefile-$(QT_VERSION) all

clean: Makefile-$(QT_VERSION)
	make -f Makefile-$(QT_VERSION) clean

clobber: Makefile-$(QT_VERSION) clean
	\rm -f Makefile-$(QT_VERSION) clean

Makefile-$(QT_VERSION): qmake

qmake: main.pro
	$(QMAKE) -o Makefile-$(QT_VERSION) $< -spec $(QT_SPEC) CONFIG+=$(QT_CONFIG)
