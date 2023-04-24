./CC = gcc

all : ReconstitueReseau ChaineMain

ReconstitueReseau : ReconstitueReseau.o Reseau.o Chaine.o SVGwriter.o Hachage.o
	$(CC) -Wall -o ReconstitueReseau ReconstitueReseau.o Reseau.o Chaine.o SVGwriter.o Hachage.o -lm

ChaineMain : ChaineMain.o Chaine.o SVGwriter.o
	$(CC) -Wall -o ChaineMain ChaineMain.o Chaine.o SVGwriter.o -lm

ChainMain.o : ChaineMain.c Chaine.h SVGwriter.h	
	$(CC) -Wall -c -o ChaineMain.o ChaineMain.c
	
Chaine.o : Chaine.c Chaine.h 
	$(CC) -Wall -c -o Chaine.o Chaine.c 
	
SVGwriter.o : SVGwriter.c SVGwriter.h
	$(CC) -Wall -c -o SVGwriter.o SVGwriter.c
	
ReconstitueReseau.o : ReconstitueReseau.c Chaine.h Reseau.h SVGwriter.h Hachage.h
	$(CC) -Wall -c -o ReconstitueReseau.o ReconstitueReseau.c

Reseau.o : Reseau.c Reseau.h Chaine.h SVGwriter.h
	$(CC) -Wall -c -o Reseau.o Reseau.c
	
Hachage.o : Hachage.c Hachage.h Chaine.h Reseau.h
	$(CC) -Wall -c -o Hachage.o Hachage.c
	
clean : 
	rm -f *.o ReconstitueReseau ChainMain

	
