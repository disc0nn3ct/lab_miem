c=gcc
all:
	$(c) -o main.o master.c
	./main.o
	@echo "================================================================="
	$(c) -o slave1.o slave1.c 
	./slave1.o
	@echo "================================================================="
	$(c) -o slave2.o slave2.c 
	./slave2.o
server:
	$(c) -o main.o master.c
	./main.o

slave1:
	$(c) -o slave1.o slave1.c 
	./slave1.o
slave2:
	$(c) -o slave2.o slave2.c 
	./slave2.o
clean: 
	rm -rf *.o