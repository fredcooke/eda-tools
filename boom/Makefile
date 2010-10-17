DIST = dk
MANU = avx johanson murata panasonic stackpole tdk yageo

EQU = $(shell ./boom-config equ)
INV = $(shell ./boom-config inv)
DSC = $(shell ./boom-config dsc)
CHR = $(shell ./boom-config chr)

DB = $(EQU) $(INV) $(DSC) $(CHR)


.PHONY:		all spotless tar count

all:		boom-config

boom-config:    boom-config.in Makefile
		sed 's/##DIST##/$(DIST)/;s/##MANU##/$(MANU)/' boom-config.in \
		  >$@ || { rm -f $@; exit 1; }
		chmod 755 boom-config

spotless:
		rm -f boom-config

tar:
		tar cfj boom-db.tar.bz2 boom-config $(DB)

count:
		perl ./count.pl $(CHR)
