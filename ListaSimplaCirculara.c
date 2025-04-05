#include <stdio.h>
#include <malloc.h>

typedef struct
{
	int nrMatricol;
	char* nume;
	float medie;
	int varsta;
} student;

typedef struct
{
	student inf;
	struct nodLS* next;
} nodLS;


nodLS* inserareNod(nodLS* capLS, student s)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.nrMatricol = s.nrMatricol;
	nou->inf.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->inf.nume, s.nume);
	nou->inf.medie = s.medie;
	nou->inf.varsta = s.varsta;
	nou->next = NULL;

	if (capLS == NULL)
	{
		nou->next = nou;
		capLS = nou;
	}
	else
	{
		nodLS* temp = capLS;
		while (temp->next != capLS) //pana la ultimul nod
			temp = temp->next;
		temp->next = nou;
		nou->next = capLS;
	}
	return capLS;
}

void traversareLista(nodLS* capLS)
{
	nodLS* temp = capLS;
	do
	{
		printf("\nNr matricol = %d, Nume = %s, Media = %5.2f, Varsta = %d",
			temp->inf.nrMatricol, temp->inf.nume, temp->inf.medie, temp->inf.varsta);
		temp = temp->next;
	} while (temp != capLS);
}

void dezalocareLista(nodLS* capLS, const char* numeFisier) {
	if (capLS == NULL) 
		return;

	FILE* f = fopen(numeFisier, "w");
	if (!f) {
		printf("Eroare la deschiderea fisierului pentru scriere.\n");
		return;
	}

	nodLS* temp = capLS;
	nodLS* start = capLS;

	do {
		fprintf(f, "Nr matricol = %d, Nume = %s, Media = %5.2f, Varsta = %d\n",
			temp->inf.nrMatricol, temp->inf.nume, temp->inf.medie, temp->inf.varsta);

		nodLS* aux = temp->next;
		free(temp->inf.nume);
		free(temp);
		temp = aux;
	} while (temp != start);

	fclose(f);
}



nodLS* stergereStudentiDupaMedie(nodLS* capLS, float pragMin, float pragMax) {
	if (capLS == NULL)
		return NULL;

	nodLS* temp = capLS;
	nodLS* anterior = NULL;

	do {
		nodLS* urm = temp->next;

		if (temp->inf.medie >= pragMin && temp->inf.medie <= pragMax) {
			if (temp == capLS) { // sterge chiar capul
				// daca lista are un singur element
				if (capLS->next == capLS) {
					free(capLS->inf.nume);
					free(capLS);
					return NULL;
				}
				
				// cauta ultimul nod pentru a-l lega de noul cap
				nodLS* ultim = capLS;
				while (ultim->next != capLS)
					ultim = ultim->next;

				capLS = capLS->next;
				ultim->next = capLS;
				free(temp->inf.nume);
				free(temp);
				temp = capLS;
			}
			else {
				anterior->next = temp->next;
				free(temp->inf.nume);
				free(temp);
				temp = urm;
			}
		}
		else {
			anterior = temp;
			temp = urm;
		}
	} while (temp != capLS);

	return capLS;
}




void main()
{
	nodLS* capLS = NULL;
	student s;

	int nrStudenti;
	char buffer[20];

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrStudenti);
	for (int i = 0; i < nrStudenti; i++)
	{
		fscanf(f, "%d", &s.nrMatricol);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, "%f", &s.medie);
		fscanf(f, "%d", &s.varsta);

		capLS = inserareNod(capLS, s);
		free(s.nume);
	}
	fclose(f);

	traversareLista(capLS);

	printf("\n\nStergere studenti cu media intre 7.5 si 9.0...\n");
	capLS = stergereStudentiDupaMedie(capLS, 7.5, 9.0);
	traversareLista(capLS);

	dezalocareLista(capLS, "dezalocare_fisier.txt");
}