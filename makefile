all: zip
CFLAGS = -Wall -Werror

sfcompress.o: sfcompress.c 
	$(CC) $(CFLAGS) -c sfcompress.c 
zip: sfcompress.o main.c 
	$(CC) $(CFLAGS)  sfcompress.o main.c -o zip
PHONY: memcheck
run: 
	./zip sfcompress -c -o file.sfc file.txt
	./zip sfcompress -d -o file1.txt file.sfc
memcheck:
	valgrind --leak-check=full --show-leak-kinds=all ./a.out sfcompress -c -o file.sfc file.txt
	valgrind --leak-check=full --show-leak-kinds=all ./a.out sfcompress -d -o file1.txt file.sfc
clean: 
	rm *.o
	rm zip
