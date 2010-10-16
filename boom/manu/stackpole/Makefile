BOOM=PATH=/home/moko/svn.openmoko.org/trunk/eda/boom:../boom:$$PATH boom

stackpole.chr:	../../dist/dk/db/digi-key.equ stackpole.gen
	$(BOOM) gen2chr STACKPOLE $^ >$@ || { rm -rf $@; exit 1; }
