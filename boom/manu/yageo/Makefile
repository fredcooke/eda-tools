BOOM=PATH=/home/moko/svn.openmoko.org/trunk/eda/boom:../boom:$$PATH boom

yageo.chr:	../../dist/dk/db/all.equ yageo.gen
	$(BOOM) gen2chr YAGEO $^ >$@ || { rm -rf $@; exit 1; }
