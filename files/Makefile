GCC = gcc

EXEC = master slave view

MASTER_O = master.o
SLAVE_O = slave.o
VIEW_O = view.o

MASTER_C = master.c
SLAVE_C = slave.c
VIEW_C = view.c

all: $(EXEC)

master: master.o
	$(GCC) $(MASTER_O) -o Master

slave: slave.o
	$(GCC) $(SLAVE_O) -o Slave

view: view.o
	$(GCC) $(VIEW_O) -o View

master.o: master.c
	$(GCC) -c $(MASTER_C)

slave.o:
	$(GCC) -c $(SLAVE_C)

view.o:
	$(GCC) -c $(VIEW_C)

clean:
	rm -f $(EXEC) $(MASTER_O) $(SLAVE_O) $(VIEW_O)