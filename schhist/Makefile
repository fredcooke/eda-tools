SHELL = /bin/bash

CACHE_DIRS = {ppm0,ppm1,ppm2,ps}
DEST = www-data@downloads.qi-hardware.com:werner/schhist
RSYNC = eval rsync -a --progress "--exclude "$(CACHE_DIRS)/

.PHONY:		all update upload-index
.PHONY:		atusb-schhist atusb-schhist-upload
.PHONY:		xue-schhist xue-schhist-upload
.PHONY:		cntr-schhist cntr-schhist-upload
.PHONY:		atusd-schhist atusd-schhist-upload
.PHONY:		sie-ceimtun-schhist sie-ceimtun-schhist-upload
.PHONY:		sie-v2-schhist sie-v2-schhist-upload
.PHONY:		board-qi-avt2 board-qi-avt2-upload
.PHONY:		mmone-jtag-serial-cable

# All the targets are for demo purposes pnly !

all:
	@echo "possible targets:" 2>&1
	@echo "  update upload-index" 2>&1
	@echo "  atusb-schhist atusb-schhist-upload" 2>&1
	@echo "  xue-schhist xue-schhist-upload" 2>&1
	@echo "  cntr-schhist cntr-schhist-upload" 2>&1
	@echo "  atusd-schhist atusd-schhist-upload" 2>&1
	@echo "  sie-ceimtun-schhist sie-ceimtun-schhist-upload" 2>&1
	@echo "  sie-v2-schhist sie-v2-upload" 2>&1
	@exit 1

update:
	cd /home/nn/xue && git pull
	cd /home/nn/sie-ceimtun && git pull
	cd /home/nn/nn-usb-fpga && git pull
	$(MAKE) atusb-schhist atusb-schhist-upload
	$(MAKE) xue-schhist xue-schhist-upload
	$(MAKE) cntr-schhist cntr-schhist-upload
	$(MAKE) atusd-schhist atusd-schhist-upload
	$(MAKE) sie-ceimtun-schhist sie-ceimtun-schhist-upload
	$(MAKE) sie-v2-schhist sie-v2-schhist-upload

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
	SCHHIST_TITLE=ben-wpan/atusd \
	  SCHHIST_HOME_URL=http://projects.qi-hardware.com/index.php/p/ben-wpan/ \
	  SCHHIST_COMMIT_TEMPLATE='http://projects.qi-hardware.com/index.php/p/ben-wpan/source/commit/{}/' \
	  ./schhist2web ../../ben-wpan atusd/atusd.sch _atusd

atusd-schhist-upload:
	$(RSYNC) _atusd/* $(DEST)/atusd/

sie-ceimtun-schhist:
	SCHHIST_TITLE="SIE CEIMTUN" \
	  SCHHIST_HOME_URL=http://projects.qi-hardware.com/index.php/p/sie-ceimtun/ \
	  SCHHIST_COMMIT_TEMPLATE='http://projects.qi-hardware.com/index.php/p/sie-ceimtun/source/commit/{}/' \
	  ./schhist2web -f ../../sie-ceimtun KiCad/SIE_power_module.sch \
	    _sie_ceimtun

sie-ceimtun-schhist-upload:
	$(RSYNC) _sie_ceimtun/* $(DEST)/sie-ceimtun/

sie-v2-schhist:
	SCHHIST_TITLE="SIE V2" \
	  SCHHIST_HOME_URL=http://projects.qi-hardware.com/index.php/p/nn-usb-fpga/ \
	  SCHHIST_COMMIT_TEMPLATE='http://projects.qi-hardware.com/index.php/p/nn-usb-fpga/source/commit/{}/' \
	  ./schhist2web ../../nn-usb-fpga \
	    KICAD_design_files/SAKC_PCB_v2/SAKC.sch _sie_v2

sie-v2-schhist-upload:
	$(RSYNC) _sie_v2/* $(DEST)/sie-v2/

board-qi-avt2:
	SCHHIST_TITLE="AVT2" \
	  SCHHIST_HOME_URL=http://projects.qi-hardware.com/index.php/p/board-qi-avt2/ \
	  SCHHIST_COMMIT_TEMPLATE='http://projects.qi-hardware.com/index.php/p/board-qi-avt2/source/commit/{}/' \
	  SCHHIST_ADDLIBS=device \
	  ./schhist2web -S ../../board-qi-avt2 \
	    main/Qi_AVT2.sch _avt2

board-qi-avt2-upload:
	$(RSYNC) _avt2/* $(DEST)/avt2/

mmone-jtag-serial-cable:
	SCHHIST_TITLE="MM JTAG" \
	  SCHHIST_HOME_URL=http://projects.qi-hardware.com/index.php/p/mmone-jtag-serial-cable/ \
	  SCHHIST_COMMIT_TEMPLATE='http://projects.qi-hardware.com/index.php/p/mmone-jtag-serial-cable/source/commit/{}/' \
	  ./schhist2web -S ../../mmone-jtag-serial-cable \
	    ./usb_jtag.sch _mm_jtag
