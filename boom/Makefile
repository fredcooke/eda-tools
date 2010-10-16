BOOM=PATH=/home/moko/svn.openmoko.org/trunk/eda/boom:../boom:$$PATH boom

KITS = 1

INV = dist/dk/db/digi-key.inv
EQU = dist/dk/db/digi-key.equ
DSC = dist/dk/db/digi-key.dsc
CHR = manu/panasonic/panasonic.chr manu/stackpole/stackpole.chr \
      manu/yageo/yageo.chr

.PHONY:		all show again

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
