BOOM=PATH=/home/moko/svn.openmoko.org/trunk/eda/boom:../boom:$$PATH boom

panasonic.chr:	../../dist/dk/db/digi-key.equ panasonic.gen
	$(BOOM) gen2chr PANASONIC $^ >$@ || { rm -rf $@; exit 1; }
