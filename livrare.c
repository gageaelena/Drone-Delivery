#include <stdio.h>
#include <stdlib.h>

#define n 50
#define INF 99999

typedef struct drum
{
	int start;
	int finish;
}drum;

void afiseazaDist( int dist[][n], int N)
{

	int i, j;
	
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
        {
            if(dist[i][j] == INF)
                printf("%s  ", "INF");
            else
                printf("%d  ", dist[i][j]);
        }
        printf("\n");
	}

	printf("\n\n\n");
}

void afiseazaDrum(FILE *g, int nod, int path[], int dist[], int track[n], int *k)
{
	if(path[nod] == -1)
	{
		return;
	}
	
	afiseazaDrum(g, path[nod], path, dist, track, k);
	
	track[(*k)++] = nod;
}

void floyd(int graph[][n], int N, int dist[n][n], int path[n][n])
{
	int i, j, k;

	for(i = 0; i < N; i++)
		for(j = 0; j < N; j++)
			if(graph[i][j] != 0  &&  graph[i][j]!=INF)
				path[i][j] = i;
			else
				path[i][j] = -1;
 
	for(i = 0; i < N; i++)
        for(j = 0; j < N; j++)
            dist[i][j] = graph[i][j];

    for(k = 0; k < N; k++)
    { 
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < N; j++)
            {
                if(dist[i][k] + dist[k][j]  <  dist[i][j])
				{
					dist[i][j] = dist[i][k] + dist[k][j];
					path[i][j] = path[k][j];
				}
            }                    
        }
    }
}

void citire(char *fin, int* ID, int* N, int* nrCulAer, int* nrComenzi, int tipPinter[n], int graph[n][n], drum drumuri[n], int *fermaStart)
{
	FILE *f = fopen(fin, "r");
	
	int i, ig, jg, j;
	
	fscanf(f, "%d", ID);
	fscanf(f, "%d", N);
	
	// Citesc vectorul cu tipul punctelor de interes

	for(i = 0; i < *N; i++)
	{
		fscanf(f, "%d", &tipPinter[i]);
	}

	fscanf(f, "%d", nrCulAer);
	
	for(i = 0; i < *N; i++)
	{
		for(j = 0; j < *N; j++)
		{
			graph[i][j] = INF;
			
			 if(i  ==  j)
			{
				graph[i][j] = 0;
			}	
		}
	}

	for(i = 0; i < *nrCulAer; i++)
	{
		fscanf(f, "%d", &ig);
		fscanf(f, "%d", &jg);
		fscanf(f, "%d", &graph[ig][jg]);

	}

	fscanf(f, "%d", nrComenzi);
	
	for(i = 0; i < *nrComenzi; i++)
	{
		fscanf(f, "%d", &drumuri[i].start);
		fscanf(f, "%d", &drumuri[i].finish);
	}

	fscanf(f, "%d", fermaStart);	

	fclose(f);
}

void search4Farm(int tipPinter[n], int Farms[n], int N, int *nrFarms)
{
	int i,k = 0;

	for(i = 0; i < N; i++)
		if(tipPinter[i]  ==  1)
		{
			Farms[k] = i;
			k = k + 1;
		}

	*nrFarms = k;
}

void cerinta_1(char* fout, int N, int nrCulAer, int nrComenzi, int tipPinter[n], int graph[n][n], drum drumuri[n])
{

	FILE *g = fopen(fout, "w");

	int i;
	int ferma, costTotal, nrFarms;
	
	int dist[n][n], path[n][n], Farms[n], track[n];

	floyd(graph,N,dist,path);

	// Cautam ferma:
	search4Farm(tipPinter, Farms, N, &nrFarms);

	// Exista numai o singura ferma

	ferma = Farms[0];

	fprintf(g, "%d ", ferma);
	
	costTotal = 0;

	int k = 0;
	for(i = 0; i < nrComenzi; i++)
	{
		// Adaugam costul de la ferma la finish
		afiseazaDrum(g, drumuri[i].finish, path[ferma], dist[ferma], track, &k);
		costTotal = costTotal + dist[ferma][drumuri[i].finish];

		// Adaugam costul de la finish la ferma
		afiseazaDrum(g, ferma, path[drumuri[i].finish],dist[drumuri[i].finish], track, &k);
		costTotal = costTotal + dist[drumuri[i].finish][ferma];
	}

	for(i = 0; i < k-1; i++)
		fprintf(g,"%d ", track[i]);
	
	fprintf(g,"%d",track[k-1]);
	fprintf(g, "\n");
	fprintf(g, "%d\n", costTotal);
	
	fclose(g);
}

void fermaMini(int nod, int nodUrmator, int dist[][n], int nrFarms, int Farms[n], int *indxFermMin)
{
	int min = INF, i;
	int cmpDist;
	for(i = 0; i < nrFarms; i++)
	{
		cmpDist = dist[nod][Farms[i]];
		if (nodUrmator != -1)// Daca mai exista noduri
			cmpDist += dist[Farms[i]][nodUrmator];

		if(dist[nod][Farms[i]] != -1  && cmpDist < min)
		{
			*indxFermMin = Farms[i];
			min = cmpDist;
		}
	}
}


//verifica daca nodul in care ma aflu este o ferma.
int esteFerma(int Farms[], int nrFarms, int nod)
{
	int i;
	for(i = 0; i < nrFarms; i++)
	{
		if (Farms[i] == nod)
			return 1;
	}

	return 0;
}

void cerinta_2(char* fout, int N, int nrCulAer, int nrComenzi, int tipPinter[n], int graph[n][n], drum drumuri[n], int fermaStart)
{
	FILE *g = fopen(fout, "w");

	int i;
	int ferma, costTotal, nrFarms, indxFermMin;
	
	int dist[n][n], path[n][n], Farms[n], track[n], trackAux[n];

	floyd(graph,N,dist,path);

	// Cautam ferma:
	search4Farm(tipPinter, Farms, N, &nrFarms);

	ferma = fermaStart;

	fprintf(g, "%d ", ferma);
	
	costTotal = 0;

	int k = 0, kAux = 0, cautFerma, j, nodUrmator;
	for(i = 0; i < nrComenzi; i++)
	{
		// Adaugam costul de la ferma la finish
		afiseazaDrum(g, drumuri[i].finish, path[ferma], dist[ferma], track, &k);
		costTotal = costTotal + dist[ferma][drumuri[i].finish];

		//verific daca pe drumul cel mai scurt catre nodul urmator avem vre-o ferma.
		cautFerma = 1;
		if (i < nrComenzi - 1)
		{
			kAux = 0;
			afiseazaDrum(g, drumuri[i+1].finish, path[drumuri[i].finish],dist[drumuri[i].finish], trackAux, &kAux);
			
			for (j = 0; j < kAux; j++)
				if (esteFerma(Farms, nrFarms, trackAux[j]))
				{
					ferma = trackAux[j];
					cautFerma = 0;
					break;
				}
		}

		//daca nu am gasit ferma ma duc in ferma cea mai apropiata.
		if (cautFerma == 1)
		{
			nodUrmator = -1;
			if (i < nrComenzi - 1)
				nodUrmator = drumuri[i+1].finish;

			fermaMini(drumuri[i].finish, nodUrmator, dist, nrFarms, Farms, &ferma);
		}

		afiseazaDrum(g, ferma, path[drumuri[i].finish],dist[drumuri[i].finish], track, &k);
		costTotal = costTotal + dist[drumuri[i].finish][ferma];
	}

	for(i = 0; i < k-1; i++)
		fprintf(g,"%d ", track[i]);
	
	fprintf(g,"%d",track[k-1]);
	fprintf(g, "\n");
	fprintf(g, "%d\n", costTotal);
	
	fclose(g);

}
int main(int argc, char **argv)
{
	int ID, N, nrCulAer, nrComenzi, fermaStart;
	int tipPinter[n], graph[n][n];
	drum drumuri[n];

	citire(argv[1],&ID,&N,&nrCulAer,&nrComenzi,tipPinter,graph,drumuri,&fermaStart);
	
	if (ID  ==  1)
		cerinta_1(argv[2], N, nrCulAer, nrComenzi, tipPinter, graph, drumuri);
	else if(ID  ==  2)
		cerinta_2(argv[2], N, nrCulAer, nrComenzi, tipPinter, graph, drumuri, fermaStart);


}
