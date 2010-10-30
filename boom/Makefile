DIST = dk
MANU = avx johanson murata panasonic stackpole tdk yageo

EQU = $(shell ./boom-config equ)
INV = $(shell ./boom-config inv)
DSC = $(shell ./boom-config dsc)
CHR = $(shell ./boom-config chr)

DIRS = $(DIST:%=dist/%) $(MANU:%=manu/%)

DB = $(EQU) $(INV) $(DSC) $(CHR)


.PHONY:		all db spotless tar count $(DIRS)

all:		boom-config

db:		boom-config $(DIRS)

$(DIRS):
		$(MAKE) -C $@

boom-config:    boom-config.in Makefile
		sed -e 's/##WARN##/*** MACHINE-GENERATED. DO NOT EDIT ! ***/' \
		  -e 's/##DIST##/$(DIST)/;s/##MANU##/$(MANU)/' boom-config.in \
		  >$@ || { rm -f $@; exit 1; }
		chmod 755 boom-config

spotless:
		rm -f boom-config

tar:
		tar cfj boom-db.tar.bz2 boom-config $(DB)

count:
		perl ./count.pl $(CHR)
