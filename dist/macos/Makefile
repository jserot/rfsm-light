include ../../config

CWD=`pwd`

ifeq ($(VERSION),)
	VOLUME_NAME=$(APPNAME)
else
	VOLUME_NAME=$(APPNAME)-$(VERSION)
endif

installer: $(APPNAME).app
	@echo "** Creating disk image"
	rm -f /tmp/$(VOLUME_NAME).dmg
	hdiutil create -size 16m -fs HFS+ -volname "$(VOLUME_NAME)" /tmp/$(VOLUME_NAME).dmg
	hdiutil attach /tmp/$(VOLUME_NAME).dmg
	cp -r ./$(APPNAME).app /Volumes/$(VOLUME_NAME)
	ln -s /Applications /Volumes/$(VOLUME_NAME)/Applications
	cp INSTALL /Volumes/$(VOLUME_NAME)
	cp ../../{LICENSE,CHANGES.md,KNOWN-BUGS.md,README.md} /Volumes/$(VOLUME_NAME)
	cp -r ../../examples/ /Volumes/$(VOLUME_NAME)/Examples
	rm -f /Volumes/$(VOLUME_NAME)/Examples/Makefile
	mkdir /Volumes/$(VOLUME_NAME)/Documentation
	cp ../../doc/Using.{html,pdf} /Volumes/$(VOLUME_NAME)/Documentation
	cp -r ../../doc/imgs /Volumes/$(VOLUME_NAME)/Documentation

	cp -r ../../examples/[a-z]* /Volumes/$(VOLUME_NAME)/Examples
	hdiutil detach /Volumes/$(VOLUME_NAME)
	hdiutil convert /tmp/$(VOLUME_NAME).dmg -format UDZO -o /tmp/$(VOLUME_NAME)_ro.dmg
	@echo "** Copying disk image into $(CWD)"
	mv /tmp/$(VOLUME_NAME)_ro.dmg ./$(VOLUME_NAME).dmg
