all: master slave

master: master.o
	gcc master.o -o Master

slave: slave.o
	gcc slave.o -o Slave

view: view.o
	gcc -view.o -o View

master.o: master.c
	gcc -c master.c

slave.o:
	gcc -c slave.c
	
view.o:
	gcc -c view.c

clean:
	rm -f Master Slave View *.o
