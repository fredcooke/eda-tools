MASTER = all.equ all.dsc all.inv
DIST = dk

.PHONY:		all rebuild spotless

all:		$(MASTER)

all.equ:	Makefile
		for n in $(DIST); do echo "include $$n/db/all.equ"; done >$@

all.inv:	Makefile
		for n in $(DIST); do echo "include $$n/db/all.inv"; done >$@

all.dsc:	Makefile
		for n in $(DIST); do echo "include $$n/db/all.dsc"; done >$@

rebuild:
		rm -f $(MASTER)

spotless:	rebuild
