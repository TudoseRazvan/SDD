#include <stdio.h>
#include <malloc.h>

typedef struct {
	int cod; //4B
	char* titlu; //4B
	int nrPreturi; //4B
	float* vectPreturi; //4B
}carte;

typedef struct
{
	carte info; //Informatie utila 12B
	struct nodLS* next; //pointer legatura 4B
}nodLS;

nodLS* initializareNod(carte c)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->info.cod = c.cod;
	nou->info.titlu = (char*)malloc((strlen(c.titlu) + 1) * sizeof(char));
	strcpy(nou->info.titlu, c.titlu);
	nou->info.nrPreturi = c.nrPreturi;
	nou->info.vectPreturi = (float*)malloc(nou->info.nrPreturi * sizeof(float));
	for (int i = 0; i < c.nrPreturi; i++)
		nou->info.vectPreturi[i] = c.vectPreturi[i];
	nou->next = NULL;
	return nou;
}

nodLS* inserareNod(nodLS* capLS, carte c)
{
	nodLS* nou = initializareNod(c);
	if (capLS == NULL)
		capLS = nou;
	else 
	{
		nodLS* temp = capLS;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
	}
	return capLS;
}

void traversareLista(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp != NULL)
	{
		printf("\nCod = %d, Titlu = %s, Nr. preturi = %d",
			temp->info.cod, temp->info.titlu, temp->info.nrPreturi);
		for (int i = 0; i < temp->info.nrPreturi;i++)
			printf(" Pret = %5.2f", temp->info.vectPreturi[i]);
		temp = temp->next;
	}
}

void dezalocareLista(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp != NULL)
	{
		nodLS* aux = temp->next;
		free(temp->info.titlu);
		free(temp->info.vectPreturi);
		free(temp);
		temp = aux;
	}
}



void main()
{
	int nrCarti;
	carte c;
	nodLS* capLS = NULL;
	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrCarti);
	for (int i = 0; i < nrCarti; i++)
	{
		fscanf(f, "%d", &c.cod);
		fscanf(f, "%s", buffer);
		c.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(c.titlu, buffer);
		fscanf(f, "%d", &c.nrPreturi);
		c.vectPreturi = (float*)malloc(c.nrPreturi * sizeof(float));
		for (int i = 0; i < c.nrPreturi; i++)
			fscanf(f, "%f", &c.vectPreturi[i]);
		capLS = inserareNod(capLS, c);
		free(c.titlu);
		free(c.vectPreturi);
	}
	fclose(f);

	traversareLista(capLS);

	dezalocareLista(capLS);
}
