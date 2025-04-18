﻿#include <stdio.h>
#include <malloc.h>

typedef struct
{
	int cod;
	char* denumire;
	float pret;
} produs;

typedef struct
{
	produs inf;
	struct nodLS* next;
} nodLS;

typedef struct
{
	struct nodLS** vect;
	int nrElem;
} hashT;

int functieHash1(hashT tabela, int cheie)
{
	return cheie % tabela.nrElem;
}

int functieHash2(hashT tabela, char* denumire)
{
	return denumire[0] % tabela.nrElem;
}

void inserareLS(nodLS** capLS, produs p)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.cod = p.cod;
	nou->inf.denumire = (char*)malloc((strlen(p.denumire) + 1) * sizeof(char));
	strcpy(nou->inf.denumire, p.denumire);
	nou->inf.pret = p.pret;
	nou->next = NULL;

	if (*capLS == NULL)
		*capLS = nou;
	else
	{
		nodLS* temp = *capLS;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
	}
}

void inserareHash(hashT tabela, produs p)
{
	int poz = functieHash1(tabela, p.cod);
	//int poz = functieHash2(tabela, p.denumire);
	inserareLS(&tabela.vect[poz], p);
}

void traversareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp != NULL)
	{
		printf("\nCod = %d, Denumire = %s, Pret = %5.2f",
			temp->inf.cod, temp->inf.denumire, temp->inf.pret);
		temp = temp->next;
	}
}

void traversareHash(hashT tabela)
{
	for(int i=0; i < tabela.nrElem; i++)
		if (tabela.vect[i] != NULL)
		{
			printf("\nPozitia = %d", i);
			traversareLS(tabela.vect[i]);
		}
}

void dezalocareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp != NULL)
	{
		nodLS* aux = temp->next;
		free(temp->inf.denumire);
		free(temp);
		temp = aux;
	}
}

void dezalocareHash(hashT tabela)
{
	for (int i = 0;i < tabela.nrElem;i++)
		if (tabela.vect[i] != NULL)
			dezalocareLS(tabela.vect[i]);
	free(tabela.vect);
}

void stergereDupaCod(hashT tabela, int cod)
{
	int poz = functieHash1(tabela, cod);
	nodLS* current = tabela.vect[poz];
	nodLS* prev = NULL;

	while (current != NULL && current->inf.cod != cod)
	{
		prev = current;
		current = current->next;
	}

	//Pentru char*
	/*while (current != NULL && strcmp(current->inf.denumire, denumire) != 0) {
		prev = current;
		current = current->next;
	}*/

	if (current == NULL)
	{
		printf("\nProdusul cu codul %d nu a fost gasit in tabela", cod);
		return;
	}

	if (prev == NULL)
		tabela.vect[poz] = current->next;
	else
		prev->next = current->next;

	free(current->inf.denumire);
	free(current);

	printf("\nProdusul cu codul %d a fost sters", cod);
}

void main()
{
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vect = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0;i < tabela.nrElem;i++)
		tabela.vect[i] = NULL;
	produs p;
	int nrProd;
	char buffer[20];

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrProd);
	for (int i = 0;i < nrProd;i++)
	{
		fscanf(f, "%d", &p.cod);
		fscanf(f, "%s", buffer);
		p.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.denumire, buffer);
		fscanf(f, "%f", &p.pret);

		inserareHash(tabela, p);
		free(p.denumire);
	}
	fclose(f);

	traversareHash(tabela);

	printf("\n\n------------------Stergere------------------\n");
	stergereDupaCod(tabela, 47);

	traversareHash(tabela);

	dezalocareHash(tabela);
}