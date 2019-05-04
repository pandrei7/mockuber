CC = g++
FLAGS = -Wall -Wextra -Werror -std=c++11


build: main.o solver.o uber_app.o uber_database.o uber_driver.o\
	uber_map.o uber_node.o
	g++ --std=c++11 -Wall -Wextra $^ -o tema2

run:
	./tema2

main.o: main.cpp solver.h
	$(CC) -c $< $(FLAGS)

solver.o: solver.cpp solver.h uber_app.h
	$(CC) -c $< $(FLAGS)

uber_app.o: uber_app.cpp uber_app.h uber_database.h uber_driver.h\
	uber_map.h algorithms.h
	$(CC) -c $< $(FLAGS)

uber_database.o: uber_database.cpp uber_database.h hashtable.h\
	uber_driver.h algorithms.h
	$(CC) -c $< $(FLAGS)

uber_driver.o: uber_driver.cpp uber_driver.h
	$(CC) -c $< $(FLAGS)

uber_map.o: uber_map.cpp uber_map.h graph.h hashtable.h uber_node.h
	$(CC) -c $< $(FLAGS)

uber_node.o: uber_node.cpp uber_node.h
	$(CC) -c $< $(FLAGS)


.PHONY: clean
clean:
	rm out/*/*
	rm tema2
	rm time.out
	rm -f *.o
