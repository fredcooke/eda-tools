DIRS = $(shell ../boom-config manu)

.PHONY:	all test sane spotless $(DIRS)

all:	$(DIRS)

$(DIRS):
	$(MAKE) -C $@

test sane spotless:
	for n in $(DIRS); do $(MAKE) -C $$n $@ || exit 1; done
