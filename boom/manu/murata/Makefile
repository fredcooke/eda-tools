BOOM=PATH=/home/moko/svn.openmoko.org/trunk/eda/boom:../boom:$$PATH boom

murata.chr:	../../dist/all.equ murata.gen
		$(BOOM) gen2chr MURATA $^ >$@ || { rm -rf $@; exit 1; }
