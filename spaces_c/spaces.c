#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
 

// GLOBAL VARIABLES
int spacesLength = -1;
int spacesHeight = -1;
int *spacesPtr = &spacesLength;


/*
SIMBOLOS:
▫ (0): Espacio muerto/vacío
⧇ (1): Espacio joven
⟎ (2): Espacio adulto tipo 1
⟏ (3): Espacio adulto tipo 2
▪ (4): Espacio adulto tipo 0 (infertil)
⧆ (5): Espacio anciano

(0): Espacio libre o muerto, no interactua de ninguna forma con su entorno ni ocupa espacio.
(1): Espacio joven, no puede reproducirse pero ocupa espacio. Tras dos iteraciones crece a un adulto de tipo 1 o 2 (50/50).
(2 y 3): Espacio adulto tipo 1 y 2. Espacios que son capaces de reproducirse. Para ello necesitan un espacio de tipo opuesto a su lado.
(4): Espacio adulto tipo 0. Un tipo 1 o 2 pasará a ser 0 tras reproducirse dos veces, y este es infertil. En dos iteraciones pasa a ser anciano.
(5): Espacio anciano. Infertil. Si tiene tipos 1 o 2 a lado, el espacio se muere para dejar espacio. Si no, muere tras dos iteraciones.
*/

int *defineSpace(int length, int height) {
	int *bitArray = (int*)malloc(sizeof(int) * (length * height));
	for (int i = 0; i < (length * height); i++) {
		bitArray[i] = 0;
	}
	return bitArray; 
}

void createSpace(int *spaces, int index, int type) {
	spaces[index] = type;
}

int randInt(int min, int max) {
        int num = (rand() % (max - min + 1)) + min; 
        return num;
}

int findSpace(int *spaces, int length, int index, int space) { // Esta función habrá encontrado espacio libre si lo que retorna es >0
	int vectors[3] = {(length - 1), length, (length + 1)}; // -9: Arriba dcha, -10 Arriba, -11 Arriba izda, dirección opuesta al cambiar signo

	if (spaces[index + 1] == space) {
		return (index + 1);
		}
	if (spaces[index - 1] == space) {
		return (index - 1);
		}
	for (int i = 0; i < 3; i++) {
		if (spaces[(index + vectors[i])] == space) {
			return (index + vectors[i]);
		}
		if (spaces[(index - vectors[i])] == space) {
			return (index - vectors[i]);
		}
		}

	return -1; // Mejorala para espacios < 10 y para espacios en esquinas
}

void randomSpaces(int *spaces, int length, int height) {
	for (int i = 0; i < (length * height); i++) {
		int chance = randInt(1,10);
		if (chance >= 7) {
			spaces[i] = randInt(1,5);
		}
	}
}

int *indexToCoords(int index) {
	int *coords = (int*)malloc(sizeof(int) * 2);

	int x = index % 10;
	int y = index / 10;

	coords[0] = x;
	coords[1] = y;

	return coords;
}

void iterate(int *spaces, int length, int height) {
	for (int i = 0; i < (length * height); i++) {
		// ESPACIO ANCIANO:
		if (spaces[i] == 51) {
			spaces[i] = 0;
		}

		if (spaces[i] == 5) {
			spaces[i] = 51;
		}
		// TIPO 0:
		if (spaces[i] == 41) {
			spaces[i] = 5;
		}

		if (spaces[i] == 4) {
			spaces[i] = 41;

		}		// ESPACIOS JÓVENES
		if (spaces[i] == 11) {
			int tipo = randInt(1,10);
			if (tipo > 5) {
				spaces[i] = 2;
			}
			else {
				spaces[i] = 3;
			}
		}

		if (spaces[i] == 1) {
			spaces[i] = 11; // Iteración 1
		}


		// ESPACIOS ADULTOS
		if (spaces[i] == 21) {
			if (findSpace(spaces, length, i, 3) >= 0 || findSpace(spaces, length, i, 31) >= 0) {
				if (findSpace(spaces, length, i, 0) >= 0 || findSpace(spaces, length, i, 5) >= 0) {
					createSpace(spaces, findSpace(spaces, length, i, 0), 1);
					spaces[i] = 4;
				}	
			} 
			/* TBD
			else {
				spaces[i] = 4;
			}
			*/
		}
		if (spaces[i] == 2) {
			if (findSpace(spaces, length, i, 3) >= 0 || findSpace(spaces, length, i, 31) >= 0) {
				if (findSpace(spaces, length, i, 0) >= 0 || findSpace(spaces, length, i, 5) >= 0) {
					createSpace(spaces, findSpace(spaces, length, i, 0), 1);
					spaces[i] = 21;
				}	
			} 
			/* TBD
			else {
				spaces[i] = 21;
			}
			*/
		}

		if (spaces[i] == 31) {
			if (findSpace(spaces, length, i, 2) >= 0 || findSpace(spaces, length, i, 21) >= 0) {
				spaces[i] = 4;
	
			}
			/* TBD
			else {
				spaces[i] = 4;
			}
			*/
		}

		if (spaces[i] == 3) {
			if (findSpace(spaces, length, i, 2) >= 0 || findSpace(spaces, length, i, 21) >= 0) {
				spaces[i] = 31;
	
			} 
			/* TBD
			else {
				spaces[i] = 31;
			}
			*/
		}


}}

void printSpaces(int *spaces, int length, int height) {
	for (int i = 0; i < (length * height); i++) {
		
		if (spaces[i] == 1 || spaces[i] == 11) {
			printf("⧇");}
		else if (spaces[i] == 2 || spaces[i] == 21) {
			printf("⟎");}
		else if (spaces[i] == 3 || spaces[i] == 31) {
			printf("⟏");}
		else if (spaces[i] == 4 || spaces[i] == 41) {
			printf("▪");}
		else if (spaces[i] == 5 || spaces[i] == 51) {
			printf("⧆");}
		else {
			printf("▫");}
			
			
		if ((i+1) % length == 0) { // Salto de línea
			printf("\n");
		}
	}
}

void run(int *spaces, int length, int height, int times, int rate) {
	for (int i = 0; i < times; i++) {
		printSpaces(spaces, length, height);
		iterate(spaces, length, height);
		printf("\n");
		sleep(rate);		
	}
}

void createSpaceUI() {
	printf("Create space\n");
	printf("\n");

	printf("0) Empty space\n1) Newborn space\n2) Type 1 adult space\n3) Type 2 adult space\n4) Type 0 adult space\n5) Elder space\n");

	int spaceTypeOption;
	int x;
	int y;

	printf("Space type:\n");
	scanf("%d\n", &spaceTypeOption);

	printf("X coordinate: ");
	scanf("%d\n", &x);

	printf("Y coordinate: ");
	scanf("%d\n", &y);
	
	int index = x + (spacesLength * y);
	createSpace(spacesPtr, index, spaceTypeOption);
	manualGenerationUI();

}


void manualGenerationUI() {
	printf("Manual generation\n");
	printf("\n");
	int whatever;
	int manualGenerationOption;
	printf("1) Create space\n");
	printf("2) Preview spaces\n");
	printf("0) Done\n");
	scanf("%d\n", &manualGenerationOption);

	if (manualGenerationOption == 1) {
		createSpaceUI();
	}	
	else if (manualGenerationOption == 2) {
		printSpaces(spacesPtr, spacesLength, spacesHeight);
		printf("\n0) Return\n");
		scanf("%d\n", &whatever);
		manualGenerationUI();
	} 
	else {
		iterationMethodUI();
	}
}


void spacesGenerationUI() {
	printf("Space generation menu\n");
	printf("\n");

	int generationOption;
	printf("Select generation method:\n");
	printf("1) Manual\n");
	printf("2) Random\n");
	printf("0) Exit\n");
	scanf("%d\n", &generationOption);

	if (generationOption == 1) {
		manualGenerationUI();
	}
	
	else if (generationOption = 2) {
		randomGenerationUI();
	}
	else {
		mainMenuUI();
	}
}

void createSpacesUI() {

	printf("Space creation menu\n");
	printf("\n");
	
	printf("Enter length: ");
	scanf("%d\n", &spacesLength);

	int height;
	printf("Enter height: ");
	scanf("%d\n", &spacesHeight);

	spacesPtr = defineSpace(spacesLength, spacesHeight);
	spacesGenerationUI();
}

void mainMenuUI() {
	printf("spaces - v.1\n");
	printf("\n");
	
	int menuOption = -1;
	printf("1) Create Spaces\n2) Information and rules\n0) Exit\n");
	scanf("%d\n", &menuOption);

	if (menuOption == 1) {
		createSpacesUI();
}
}
int main() {
	srand(time(NULL));
	mainMenuUI();	
	return 0;
}
