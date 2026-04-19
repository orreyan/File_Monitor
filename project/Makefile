all: monitor

monitor: monitor.o backup.o
	gcc -o monitor monitor.o backup.o

monitor.o: monitor.c backup.h
	gcc -c monitor.c

backup.o: backup.c backup.h
	gcc -c backup.c

clean:
	rm -f *.o monitor
	rm -rf backup