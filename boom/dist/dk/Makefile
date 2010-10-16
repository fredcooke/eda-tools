CACHE = query.data
EQUS = panasonic-erj stackpole-rmcf yageo-rc

.PHONY:			all

all:			db/digi-key.dsc db/digi-key.inv

$(CACHE):		db/digi-key.equ
			awk '/^#END/ { exit } /^DIGI-KEY / { print $$2 }' $< | \
			  perl ./dk-db.pl query \
			  `[ -r $(CACHE) ] && echo '' -i $(CACHE)` >_$@ || \
			  { rm -f $@ _$@; exit 1; }
			mv _$@ $@

db/digi-key.equ:	$(EQUS:%=db/%.equ)
			cat $^ >$@ || { rm -rf $@; exit 1; }

db/digi-key.dsc:	$(CACHE)
			perl ./dk-db.pl dsc $< >$@ || \
			  { rm -f $@; exit 1; }

db/digi-key.inv:	$(CACHE)
			perl ./dk-db.pl inv $< >$@ || \
			  { rm -f $@; exit 1; }

db/%.equ:		%.catq
			perl ./dk-catq.pl $< >$@ || { rm -rf $@; exit 1; }
