cas: io.c drv.c printer.c eval.c flist.c parser.c
	cc -o caas io.c drv.c printer.c eval.c flist.c parser.c -lm

clean: 
	rm caas
