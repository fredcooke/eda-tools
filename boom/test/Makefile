BOOM=PATH=/home/moko/svn.openmoko.org/trunk/eda/boom:../boom:$$PATH boom

KITS = 1

EQU = $(shell ../boom-config equ)
INV = $(shell ../boom-config inv)
DSC = $(shell ../boom-config dsc)
CHR = $(shell ../boom-config chr)


.PHONY:		all show again spotless

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
		perl ../mbq2lst $^ >$@ || { rm -f $@; exit 1; }

spotless:
		rm -f test.lst test.par test.ord
