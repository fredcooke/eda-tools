DIST = dk
MANU = avx johanson murata panasonic stackpole tdk yageo

EQU = dist/all.equ
INV = dist/all.inv
DSC = dist/all.dsc
CHR = $(foreach m,$(MANU),manu/$(m)/$(m).chr)

DB = $(EQU) $(INV) $(DSC) $(CHR) \
    $(DIST:%=dist/%/db/*.equ) \
    $(DIST:%=dist/%/db/*.inv) \
    $(DIST:%=dist/%/db/*.dsc)


.PHONY:		all spotless tar count

all:

spotless:

tar:
		tar cfj boom-db.tar.bz2 $(DB)

count:
		perl ./count.pl $(CHR)
