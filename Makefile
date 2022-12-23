cas: io.c drv.c printer.c eval.c flist.c parser.c
	gcc -o caas io.c drv.c printer.c eval.c flist.c parser.c -lm

clean: 
	rm caas
