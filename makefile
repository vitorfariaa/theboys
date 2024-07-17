parametrosCompilacao = -Wall -lm
nomePrograma=theboys

all: $(nomePrograma)

$(nomePrograma): theboys.o lef.o fila.o conjuntos.o
	gcc -o $(nomePrograma) theboys.o lef.o fila.o conjuntos.o $(parametrosCompilacao)

theboys.o: theboys.h theboys.c
	gcc -c theboys.c $(parametrosCompilacao)

lef.o: lef.h lef.c
	gcc -c lef.c $(parametrosCompilacao)

fila.o: fila.h fila.c
	gcc -c fila.c $(parametrosCompilacao)

conjuntos.o: conjuntos.h conjuntos.c
	gcc -c conjuntos.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)
