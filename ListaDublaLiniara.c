#include <stdio.h>
#include <malloc.h>

typedef struct {
	int cod;
	char* titlu;
	float* vectPreturi;
	int nrPreturi;
}carte;

typedef struct {
	carte info;
	struct nodLD* next;
	struct nodLD* prev; //2 pointeri de legatura 4B x 2 = 8B
}nodLD;

nodLD* initializareNod(carte c)
{
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->info.cod = c.cod;
	nou->info.titlu = (char*)malloc((strlen(c.titlu) + 1) * sizeof(char));
	strcpy(nou->info.titlu, c.titlu);
	nou->info.nrPreturi = c.nrPreturi;
	nou->info.vectPreturi = (float*)malloc(nou->info.nrPreturi * sizeof(float));
	for (int i = 0; i < nou->info.nrPreturi; i++)
		nou->info.vectPreturi[i] = c.vectPreturi[i];
	nou->next = NULL;
	nou->prev = NULL;
	return nou;
}

nodLD* inserareNod(nodLD* capLD, nodLD** coadaLD, carte c)
{
	nodLD* nou = initializareNod(c);
	//Lista goala
	if (capLD == NULL)
	{
		capLD = nou;
		*coadaLD = nou;
	}
	else //Lista nu este goala
	{
		nodLD* temp = capLD;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou; //Se leaga legatura inainte a ultimului nod de nodul nou
		nou->prev = temp; //Se leaga legatura inapoi a noului nod 
		*coadaLD = nou; //Noul nod devine coada
	}
	return capLD;
}

void traversareLista(nodLD* capLD)
{
	nodLD* temp = capLD;
	while (temp != NULL)
	{
		printf("\nCod = %d, Titlu = %s, Nr. preturi = %d",
			temp->info.cod, temp->info.titlu, temp->info.nrPreturi);
		for (int i = 0;i < temp->info.nrPreturi;i++)
			printf(" Pret = %5.2f,", temp->info.vectPreturi[i]);
		temp = temp->next;
	}
}

//La fel ca la traversarea obisnuita, doar ca pui coadaLD si temp->prev
void traversareListaInvers(nodLD* coadaLD)
{
	nodLD* temp = coadaLD;
	while (temp != NULL)
	{
		printf("\nCod = %d, Titlu = %s, Nr. preturi = %d",
			temp->info.cod, temp->info.titlu, temp->info.nrPreturi);
		for (int i = 0;i < temp->info.nrPreturi;i++)
			printf(" Pret = %5.2f,", temp->info.vectPreturi[i]);
		temp = temp->prev;
	}
}

void dezalocareLista(nodLD* capLD)
{
	nodLD* temp = capLD;
	while (temp != NULL)
	{
		nodLD* aux = temp->next;
		free(temp->info.titlu);
		free(temp->info.vectPreturi);
		free(temp);
		temp = aux;
	}
}

void stergereCarteDupaTitlu(nodLD** capLD, nodLD** coadaLD, char* titluDeSters) 
{
	//caz 1 Stergere CAP
	if (strcmp((*capLD)->info.titlu, titluDeSters) == 0)
	{
		nodLD* temp = *capLD;
		*capLD = (*capLD)->next;
		if (*capLD != NULL)
			(*capLD)->prev = NULL;
		free(temp->info.titlu);
		free(temp->info.vectPreturi);
		free(temp);
		return;
	}
	else // caz2 //Stergere coada
	{
		if (strcmp((*coadaLD)->info.titlu, titluDeSters) == 0)
		{
			nodLD* temp = *coadaLD;
			*coadaLD = (*coadaLD)->prev;
			if (*coadaLD != NULL)
				(*coadaLD)->next = NULL;
			free(temp->info.titlu);
			free(temp->info.vectPreturi);
			free(temp);
			return;
		}
		else // caz 3
		{
			nodLD* temp = *capLD;
			while (temp != NULL)
			{
				if (strcmp(temp->info.titlu, titluDeSters) == 0)
				{
					nodLD* anterior = temp->prev;
					nodLD* urmator = temp->next;
					anterior->next = urmator;
					urmator->prev = anterior;
					free(temp->info.titlu);
					free(temp->info.vectPreturi);
					free(temp);
					return;
				}
				else
					temp = temp->next;
			}
		}
	}
}

void main()
{
	int nrCarti;
	carte c;
	nodLD* capLD = NULL;
	nodLD* coadaLD = NULL;
	
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
		capLD = inserareNod(capLD, &coadaLD, c);
		free(c.titlu);
		free(c.vectPreturi);
	}
	fclose(f);

	traversareLista(capLD);
	printf("\n\n--------------Traversare inversa--------------\n");
	traversareListaInvers(coadaLD);

	printf("\n\n-----------------Stergere-----------------\n");
	//Stergerea se face prin adresa -> ** la apel
	stergereCarteDupaTitlu(&capLD, &coadaLD, "Baltagul");
	traversareLista(capLD);

	dezalocareLista(capLD);
}