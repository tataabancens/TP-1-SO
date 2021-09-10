CC = gcc
CFLAGS= -Wall
GCCLIBS= -lrt -pthread

EXEC = Master Slave View

LIB_O = shared_memory.o
LIB_H = shared_memory.h
LIB_C = shared_memory.c

MASTER_O = master.o
SLAVE_O = slave.o
VIEW_O = view.o

OBJS = $(MASTER_O) $(SLAVE_O) $(VIEW_O) $(LIB_O)


all: $(EXEC)

Master: $(MASTER_O) $(LIB_O) $(LIB_H)
	$(CC) $(CFLAGS) $(MASTER_O) $(LIB_O) -o Master $(GCCLIBS)

Slave: $(SLAVE_O) $(LIB_O) $(LIB_H)
	$(CC) $(CFLAGS) $(SLAVE_O) -o Slave $(GCCLIBS)

View: $(VIEW_O) $(SHARED_MEM_O) $(LIB_O) $(LIB_H)
	$(CC) $(CFLAGS) $(VIEW_O) $(SHARED_MEM_O) -o View $(GCCLIBS)

$(LIB_O): $(LIB_C) $(LIB_H)
	$(CC) $(CFLAGS) -c $(LIB_C) $(GCCLIBS)	

$(MASTER_O): master.c $(LIB_H)
	$(CC) $(CFLAGS) -c master.c $(GCCLIBS)

$(VIEW_O): view.c $(LIB_H)
	$(CC) $(CFLAGS) -c view.c $(GCCLIBS)

$(SLAVE_O): slave.c
	$(CC) $(CFLAGS) -c slave.c $(GCCLIBS)	

clean:
	rm -f $(EXEC) $(OBJS)