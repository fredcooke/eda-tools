BOOM=PATH=/home/moko/svn.openmoko.org/trunk/eda/boom:../boom:$$PATH boom

KITS = 1

DIST = dk
MANU = murata panasonic stackpole yageo

EQU = dist/all.equ
INV = dist/all.inv
DSC = dist/all.dsc
CHR = $(foreach m,$(MANU),manu/$(m)/$(m).chr)

DB = $(EQU) $(INV) $(DSC) $(CHR) \
    $(DIST:%=dist/%/db/*.equ) \
    $(DIST:%=dist/%/db/*.inv) \
    $(DIST:%=dist/%/db/*.dsc)


.PHONY:		all show again spotless tar

all:		show

show:		test.ord $(DSC)
		$(BOOM) prettyord -t $^

again:
		$(MAKE) spotless
		$(MAKE) all

test.ord:	test.par $(INV) $(EQU)
		$(BOOM) part2order $(KITS) $^ >$@ || { rm -f $@; exit 1; }

test.par:	$(EQU) $(INV) $(CHR) test.lst test.sub
		$(BOOM) bom2part $^ >$@ || { rm -f $@; exit 1; }

test.lst:	test.mbq
		perl ./mbq2lst $^ >$@ || { rm -f $@; exit 1; }

spotless:
		rm -f test.lst test.par test.ord

tar:
		tar cfj boom-db.tar.bz2 $(DB)
