#include <stdio.h>
#include <malloc.h>

typedef struct
{
	int nrVagon;
	int nrLocuri;
	int nrPasageri;
	char* numeCompanie;
} vagon;

typedef struct nodLD
{
	vagon inf;
	struct nodLD* next;
	struct nodLD* prev;
} nodLD;

nodLD* inserareNod(nodLD* capLD, nodLD** coadaLD, vagon v)
{
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->inf.nrVagon = v.nrVagon;
	nou->inf.nrLocuri = v.nrLocuri;
	nou->inf.nrPasageri = v.nrPasageri;
	nou->inf.numeCompanie = (char*)malloc((strlen(v.numeCompanie) + 1) * sizeof(char));
	strcpy(nou->inf.numeCompanie, v.numeCompanie);
	nou->next = NULL;
	nou->prev = NULL;

	if (capLD == NULL)
	{
		capLD = nou;
		*coadaLD = nou;
	}
	else
	{
		nodLD* temp = capLD;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou; //Se leaga legatura inainte a ultimului nod de nodul nou
		nou->prev = temp; // se leaga legatura inapoi a noului nod
		*coadaLD = nou; //Noul nod devine coada
	}
	return capLD;
}

void traversareLista(nodLD* capLD)
{
	nodLD* temp = capLD;
	while (temp != NULL)
	{
		printf("\nNr vagon = %d, Nr locuri = %d, Nr pasageri = %d, Nume companie = %s",
			temp->inf.nrVagon, temp->inf.nrLocuri, temp->inf.nrPasageri, temp->inf.numeCompanie);
		temp = temp->next;
	}
}

void dezalocareLista(nodLD* capLD, const char* numeFisier)
{
	if (capLD == NULL)
		return;

	FILE* f = fopen(numeFisier, "w");
	if (!f)
	{
		printf("Eroare la deschiderea fisierului pentru scriere.\n");
		return;
	}

	nodLD* temp = capLD;

	while (temp != NULL)
	{
		fprintf(f, "Nr. vagoane = %d, Nr. locuri = %d, Nr. pasageri = %d, Nume companie = %s\n",
			temp->inf.nrVagon, temp->inf.nrLocuri, temp->inf.nrPasageri, temp->inf.numeCompanie);

		nodLD* aux = temp->next;
		free(temp->inf.numeCompanie);
		free(temp);
		temp = aux;
	} 
	fclose(f);
}

void interschimbarePasageri(nodLD* capLD, int k)
{
	if (capLD == NULL || k <= 1)
	{
		printf("Eroare: Lista este vida sau pozitia k este invalida.\n");
		return;
	}

	nodLD* temp = capLD;
	int pozitie = 1;

	// Parcurgem lista pana la nodul k-1
	while (temp != NULL && pozitie < k - 1)
	{
		temp = temp->next;
		pozitie++;
	}

	// Verificam daca exista cel putin k noduri
	if (temp == NULL || temp->next == NULL)
	{
		printf("Eroare: Nu exista suficienti noduri in lista.\n");
		return;
	}

	// Schimbam numarul de pasageri dintre nodul k-1 si nodul k
	int aux = temp->inf.nrPasageri;
	temp->inf.nrPasageri = temp->next->inf.nrPasageri;
	temp->next->inf.nrPasageri = aux;

	printf("Numarul de pasageri a fost schimbat intre nodurile %d si %d.\n", k - 1, k);
}

void salvareListaInVectorInvers(nodLD* capLD)
{
	if (capLD == NULL)
	{
		printf("Lista este goala.\n");
		return;
	}

	//Gaseste ultimul nod
	nodLD* temp = capLD;
	while (temp->next != NULL)
		temp = temp->next;

	int i = 0;
	nodLD* invers[10];
	while (temp != NULL)
	{
		invers[i++] = temp;
		temp = temp->prev;
	}

	for (int j = 0; j < i; j++)
	{
		printf("\nNr vagon = %d, Nr locuri = %d, Nr pasageri = %d, Nume companie = %s",
			invers[j]->inf.nrVagon, invers[j]->inf.nrLocuri, invers[j]->inf.nrPasageri, invers[j]->inf.numeCompanie);
	}
}


void main()
{
	nodLD* capLD = NULL;
	nodLD* coadaLD = NULL;
	vagon v;
	int nrVagoane;
	char buffer[20];

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrVagoane);
	for (int i = 0;i < nrVagoane;i++)
	{
		fscanf(f, "%d", &v.nrVagon);
		fscanf(f, "%d", &v.nrLocuri);
		fscanf(f, "%d", &v.nrPasageri);
		fscanf(f, "%s", buffer);
		v.numeCompanie = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(v.numeCompanie, buffer);

		capLD = inserareNod(capLD, &coadaLD, v);
		free(v.numeCompanie);
	}
	fclose(f);

	traversareLista(capLD);

	printf("\n\n---------------Interschimbare---------------\n");
	interschimbarePasageri(capLD, 2);
	traversareLista(capLD);

	printf("\n\n---------------Vector---------------\n");
	salvareListaInVectorInvers(capLD);

	dezalocareLista(capLD, "dezalocare_fisier.txt");
}