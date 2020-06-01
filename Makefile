# Makefile (Leonardo Barbosa; May 16th, 2020)
# Techniken Service PROCEMPA - Cercamento eletrônico

EXEC = TechnikenPROCEMPA
CFLAGS = -c -std=c++17

${EXEC}: main.o Watcher.o Plate.o
	@g++ main.o Watcher.o Plate.o -o ${EXEC}

main.o: main.cpp Watcher.o Plate.o
	@g++ ${CFLAGS} main.cpp

Watcher.o: Watcher.hpp Watcher.cpp
	@g++ ${CFLAGS} Watcher.cpp

Plate.o: Plate.hpp Plate.cpp
	@g++ ${CFLAGS} Plate.cpp

run:	${EXEC}
	@./${EXEC}

clean:
	@rm -f main.o Watcher.o Plate.o ${EXEC}

All: ${EXEC}