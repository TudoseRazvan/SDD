#include <stdio.h>
#include <malloc.h>

typedef struct 
{
	int cod;
	char* titlu;
	float* vectPreturi;
	int nrPreturi;
}carte;

typedef struct
{
	carte info;
	struct nodLD* next;
	struct nodLD* prev;
}nodLD;

nodLD* initializareNod(carte c)
{
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->info.cod = c.cod;
	nou->info.titlu = (char*)malloc((strlen(c.titlu) + 1) * sizeof(char));
	strcpy(nou->info.titlu, c.titlu);
	nou->info.nrPreturi = c.nrPreturi;
	nou->info.vectPreturi = (float*)malloc(nou->info.nrPreturi * sizeof(float));
	for (int i = 0; i < c.nrPreturi;i++)
		nou->info.vectPreturi[i] = c.vectPreturi[i];
	nou->next = nou;
	nou->prev = nou;
	return nou;
}

nodLD* inserareNod(nodLD* capLD, nodLD** coadaLD, carte c)
{
	nodLD* nou = initializareNod(c);
	if (capLD == NULL)
	{
		capLD = nou;
		*coadaLD = nou;
	}
	else 
	{
		(*coadaLD)->next = nou;
		nou->prev = *coadaLD;
		*coadaLD = nou;
		(*coadaLD)->next = capLD;
		capLD->prev = *coadaLD;
	}
	return capLD;
}

void traversareLista(nodLD* capLD)
{
	nodLD* temp = capLD;
	while (temp->next != capLD)
	{
		printf("\nCod = %d, Titlu = %s, Nr. preturi = %d",
			temp->info.cod, temp->info.titlu, temp->info.nrPreturi);
		for (int i = 0; i < temp->info.nrPreturi; i++)
			printf(" Pret = %5.2f", temp->info.vectPreturi[i]);
		temp = temp->next;
	}
	//Pentru coadaLD
	printf("\nCod = %d, Titlu = %s, Nr. preturi = %d",
		temp->info.cod, temp->info.titlu, temp->info.nrPreturi);
	for (int i = 0; i < temp->info.nrPreturi; i++)
		printf(" Pret = %5.2f", temp->info.vectPreturi[i]);
	temp = temp->next;
}

void traversareListaInvers(nodLD* coadaLD)
{
	nodLD* temp = coadaLD;
	while (temp->prev != coadaLD)
	{
		printf("\nCod = %d, Titlu = %s, Nr. preturi = %d",
			temp->info.cod, temp->info.titlu, temp->info.nrPreturi);
		for (int i = 0; i < temp->info.nrPreturi; i++)
			printf(" Pret = %5.2f", temp->info.vectPreturi[i]);
		temp = temp->prev;
	}
	printf("\nCod = %d, Titlu = %s, Nr. preturi = %d",
		temp->info.cod, temp->info.titlu, temp->info.nrPreturi);
	for (int i = 0; i < temp->info.nrPreturi; i++)
		printf(" Pret = %5.2f", temp->info.vectPreturi[i]);
	temp = temp->prev;
}

void dezalocareLista(nodLD* capLD)
{
	nodLD* temp = capLD;
	while (temp->next != capLD)
	{
		nodLD* aux = temp->next;
		free(temp->info.titlu);
		free(temp->info.vectPreturi);
		free(temp);
		temp = aux;
	}
	free(temp->info.titlu);
	free(temp->info.vectPreturi);
	free(temp);
}


void stergeCarteDupaTitlu(nodLD** capLD, nodLD** coadaLD, char* titluDeSters)
{
	//caz 1
	if (strcmp((*capLD)->info.titlu, titluDeSters) == 0)
	{
		nodLD* temp = *capLD;
		*capLD = (*capLD)->next;
		(*capLD)->prev = *coadaLD;
		(*coadaLD)->next = *capLD;
		free(temp->info.titlu);
		free(temp->info.vectPreturi);
		free(temp);
		return;
	}
	else //caz 2
		if (strcmp((*coadaLD)->info.titlu, titluDeSters) == 0)
		{
			nodLD* temp = *coadaLD;
			*coadaLD = (*coadaLD)->prev;
			(*coadaLD)->next = *capLD;
			(*capLD)->prev = *coadaLD;
			free(temp->info.titlu);
			free(temp->info.vectPreturi);
			free(temp);
			return;
		}
		else //caz 3
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


void main()
{
	int nrCarti;
	carte c;
	nodLD* capLD = NULL;
	nodLD* coadaLD = NULL;

	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrCarti);
	for (int i = 0;i < nrCarti;i++)
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

	printf("\n--------------Traversare--------------\n");
	traversareLista(capLD);

	printf("\n\n--------------Stergere--------------\n");
	stergeCarteDupaTitlu(&capLD, &coadaLD, "Baltagul");
	traversareLista(capLD);

	printf("\n\n--------------Traversare inversa--------------\n");
	traversareListaInvers(coadaLD);

}