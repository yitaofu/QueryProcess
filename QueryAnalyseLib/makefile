CC = cc
CFLAGS = -lm -lcurl -lstdc++

objects = queryAnalyse.o ic_url.o cJSON.o main.o

main: $(objects)
	$(CC) $(objects) $(CFLAGS) -o main

main.o: main.cpp
	$(CC) -c main.cpp

queryAnalyse.o: queryAnalyse.cpp
	$(CC) -c queryAnalyse.cpp

ic_url.o: ic_url.c
	$(CC) -c ic_url.c

cJSON.o: cJSON.c
	$(CC) -c cJSON.c

clean:
	rm *.o
