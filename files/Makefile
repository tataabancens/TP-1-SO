CC = gcc

EXEC = Master Slave View

MASTER_O = master.o
SLAVE_O = slave.o
VIEW_O = view.o

OBJS = $(MASTER_O) $(SLAVE_O) $(VIEW_O)


all: $(EXEC)

Master: $(MASTER_O)
	$(CC) $(MASTER_O) -o Master

Slave: $(SLAVE_O)
	$(CC) $(SLAVE_O) -o Slave

View: $(VIEW_O)
	$(CC) $(VIEW_O) -o View

%.o: %.c
	$(CC) -c $?

clean:
	rm -f $(EXEC) $(OBJS)