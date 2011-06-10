D=rc3_gerber

all:		front.png back.png

drill.pho:
		tr -d '\015' <$(D)/drl00.drl | ./drl2gerber >$@

front.png:	drill.pho
		cd $(D) && ../prettygerbv -d ../drill.pho foo front ../$@
	
back.png:	drill.pho
		cd $(D) && ../prettygerbv -d ../drill.pho foo back ../$@

conn.png:		drill.pho
		cd $(D) && ../prettygerbv -d ../drill.pho foo conn ../$@

clean:
		rm -f drill.pho	front.png back.png conn.png
