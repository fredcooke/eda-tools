MASTER = db/all.equ db/all.dsc db/all.inv
PARTS = murata-grm panasonic-erj stackpole-rmcf tdk-c yageo-cc yageo-rc

.SECONDARY:		# prevent *.cache from being automatically deleted

.PHONY:			all clean rebuild update spotless

all:			$(MASTER) \
			  $(PARTS:%=db/%.equ) \
			  $(PARTS:%=db/%.inv) \
			  $(PARTS:%=db/%.dsc) 

db/%.cache:		db/%.equ
			awk '/^#END/ { exit } /^DIGI-KEY / { print $$2 }' $< | \
			  perl ./dk-db.pl query \
			  `[ -r $@ ] && echo '' -i $@` >$(@)_ || \
			  { rm -f $@ $(@)_; exit 1; }
			mv $(@)_ $@

db:
			mkdir -p db

#
# We need "db" to create the directory. However, this also means that any
# change in the db/ directory triggers a rebuild of all.equ. This is cheap, so
# this is nothing but a small cosmetic issue.
#

db/all.equ:		db Makefile
			for n in $(PARTS); do echo "include $$n.equ"; done >$@

db/all.inv:		Makefile
			for n in $(PARTS); do echo "include $$n.inv"; done >$@

db/all.dsc:		Makefile
			for n in $(PARTS); do echo "include $$n.dsc"; done >$@

db/%.equ:		%.catq
			perl ./dk-catq.pl $< >$@ || { rm -rf $@; exit 1; }

db/%.inv:		db/%.cache
			perl ./dk-db.pl inv $< >$@ || { rm -f $@; exit 1; }

db/%.dsc:		db/%.cache
			perl ./dk-db.pl dsc $< >$@ || { rm -f $@; exit 1; }


# --- Cleanup -----------------------------------------------------------------

#
# We have various levels of cleanup:
#
# - clean: remove left-over temporary files
# - rebuild: remove everything that can be rebuilt from cached data
# - update: remove the short-lived inventory and pricing cache too
# - again: remove also the long-lived equivalences cache
#
# "clean" does not disturb use of the database. The run-time cost of recovering
# from "rebuild" is small. What takes a lot of time is "update". The additional
# cost of "spotless" is small again.
#

clean:
			rm -f $(PARTS:%=db/%.cache_)

rebuild:		clean
			rm -f $(MASTER)
			rm -f $(PARTS:%=db/%.inv)
			rm -f $(PARTS:%=db/%.dsc)

update:			rebuild
			rm -f $(PARTS:%=db/%.cache)

spotless:		update
			rm -f $(PARTS:%=db/%.equ)
