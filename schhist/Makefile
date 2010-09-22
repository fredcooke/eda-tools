SHELL = /bin/bash

CACHE_DIRS = {ppm0,ppm1,ppm2,ps}
DEST = www-data@downloads.qi-hardware.com:werner/schhist
RSYNC = eval rsync -a --progress "--exclude "$(CACHE_DIRS)/

.PHONY:		all update upload-index
.PHONY:		atusb-schhist atusb-schhist-upload
.PHONY:		xue-schhist xue-schhist-upload
.PHONY:		cntr-schhist cntr-schhist-upload
.PHONY:		atusd-schhist atusd-schhist-upload

# All the targets are for demo purposes pnly !

all:
		@echo "possible targets:" 2>&1
		@echo "  atusb-schhist atusb-schhist-upload" 2>&1
		@echo "  xue-schhist xue-schhist-upload" 2>&1
		@echo "  cntr-schhist cntr-schhist-upload" 2>&1
		@echo "  atusd-schhist atusd-schhist-upload" 2>&1
		@exit 1

update:
	cd /home/nn/xue && git pull
	$(MAKE) atusb-schhist atusb-schhist-upload
	$(MAKE) xue-schhist xue-schhist-upload
	$(MAKE) cntr-schhist cntr-schhist-upload
	$(MAKE) atusd-schhist atusd-schhist-upload

upload-index:
	scp demo-index.html $(DEST)/index.html

atusb-schhist:
	SCHHIST_TITLE=ben-wpan/atrf \
	  SCHHIST_HOME_URL=http://projects.qi-hardware.com/index.php/p/ben-wpan/ \
	  SCHHIST_COMMIT_TEMPLATE='http://projects.qi-hardware.com/index.php/p/ben-wpan/source/commit/{}/' \
	  SCHHIST_ORDER=wpan-atrf:USB:RF \
	  ./schhist2web ../../ben-wpan atrf/wpan-atrf.sch _atusb

atusb-schhist-upload:
	 $(RSYNC) _atusb/* $(DEST)/atusb/

xue-schhist:
	SCHHIST_TITLE=Xue \
	  SCHHIST_HOME_URL=http://projects.qi-hardware.com/index.php/p/xue/ \
	  SCHHIST_COMMIT_TEMPLATE='http://projects.qi-hardware.com/index.php/p/xue/source/commit/{}/' \
	  ./schhist2web -S ../../xue kicad/xue-rnc/xue-rnc.sch _xue

xue-schhist-upload:
	 $(RSYNC) _xue/* $(DEST)/xue/

cntr-schhist:
	SCHHIST_TITLE=ben-wpan/cntr \
	  SCHHIST_HOME_URL=http://projects.qi-hardware.com/index.php/p/ben-wpan/ \
	  SCHHIST_COMMIT_TEMPLATE='http://projects.qi-hardware.com/index.php/p/ben-wpan/source/commit/{}/' \
	  ./schhist2web ../../ben-wpan cntr/cntr.sch _cntr

cntr-schhist-upload:
	$(RSYNC) _cntr/* $(DEST)/cntr/

atusd-schhist:
	SCHHIST_TITLE=ben-wpan/atusd 
	  SCHHIST_HOME_URL=http://projects.qi-hardware.com/index.php/p/ben-wpan/ \
	  SCHHIST_COMMIT_TEMPLATE='http://projects.qi-hardware.com/index.php/p/ben-wpan/source/commit/{}/' \
	  ./schhist2web ../../ben-wpan atusd/atusd.sch _atusd

atusd-schhist-upload:
	$(RSYNC) _atusd/* $(DEST)/atusd/
