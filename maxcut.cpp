#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_N 1000
#define SOL_N 50
#define MUT_CRI 0.005
#define MAX_TIME 170

int Vertex_N, Edge_N;
int edge[MAX_N+1][MAX_N+1];

typedef struct {
	bool chromosome[MAX_N+1];
	int fitness;
} solution;

solution population[SOL_N];
solution best;

void eval(solution* s){
	bool* chro = s->chromosome;
	int sum=0;
	for(int i=1; i<=Vertex_N; i++){
		bool b=chro[i];
		for(int j=i+1; j<=Vertex_N; j++){
			if(b^(chro[j])){
				sum+=edge[i][j];
			}
		}
	}
	s->fitness=sum;
}


void selection(solution** s){
	// roullet
	//TODO

	//tourament
	//TODO

	// rank-based
	// TODO

	//sharing
	// TODO

	//random
	*s=&population[rand()%SOL_N];

}

void crossover(solution* p1, solution* p2, solution* c){
	// 1-pt fixed
	// TODO

	// 1-pt random
	int xover_pt=rand()%Vertex_N;
	for(int i=1; i<=xover_pt; i++){
		c->chromosome[i]=p1->chromosome[i];
	}
	for(int i=xover_pt+1; i<=Vertex_N; i++){
		c->chromosome[i]=p2->chromosome[i];
	}


	// multi-pt
	// TODO

	// uniform
	// TODO



}

void mutation(solution* c){
	// typical
	double r;
	for(int k=1; k<=Vertex_N; k++){
		r=(float)rand()/(float)(RAND_MAX);
		if(r<MUT_CRI){
			c->chromosome[k]= !(c->chromosome[k]);
		}
	}

	// non-uniform
	//TODO

	// evaluate fitness
	eval(c);

}

void replacement(solution* p1, solution* p2, solution* c){
	// worst in population
	// TODO

	// worst in parents
	solution* p;
	if(p1->fitness>p2->fitness){
		p=p2;
	}
	else{
		p=p1;
	}


	bool* p_ch=p->chromosome, *c_ch=c->chromosome;

	for(int k=1; k<Vertex_N; k++){
		p_ch[k]=c_ch[k];
	}

	p->fitness=c->fitness;

	// parent								if child > parent
	// worst in population	o.w.
	//TODO

	// similar
	// TODO


}

void GA(){
	time_t start=time(0);

	int index=0;

	solution *p1, *p2;
	solution c;
	// 180second
	while((time(0)-start) < MAX_TIME){
		selection(&p1);
		selection(&p2);

		crossover(p1, p2, &c);


		mutation(&c);

		replacement(p1, p2,&c);
	}

	return ;
}

void init(FILE *fp){
	fscanf(fp, "%d%d", &Vertex_N, &Edge_N);
	int i, j, dist;

	//initailize
	for(int x=1; x<=Vertex_N; x++){
		for(int y=1; y<=Vertex_N; y++){
			edge[x][y]=0;
		}
	}

	// get input
	for(int k=0; k<Edge_N; k++){
		fscanf(fp, "%d%d%d", &i, &j, &dist);
		edge[i][j]=dist;
		edge[j][i]=dist;
	}

	// generate random solution
	for(int sol_i=0; sol_i<SOL_N; sol_i++){
		// random
		for(int k=1; k<=Vertex_N; k++){
			population[sol_i].chromosome[k]=rand()%2;
		}
		eval(&population[sol_i]);

	}

	return ;
}

void find_max(solution** s){
	int max_f=population[0].fitness, max_i=0;
	for(int i=1; i<SOL_N; i++){
		if(max_f<population[i].fitness){
			max_f=population[i].fitness;
			max_i=i;
		}
	}
	*s=&population[max_i];
}



void answer(FILE *fp){
	solution* best;
	find_max(&best);
	bool *b=best->chromosome;
	for(int i=1; i<=Vertex_N; i++){
		if(b[i]){
			fprintf(fp, "%d ", i);
		}
	}
	fprintf(fp, "\nfitness : %d\n", best->fitness);
	return ;
}

int main(int argc, char** argv){
	srand(time(NULL));

	FILE *fin=fopen(argv[1], "r");
	FILE *fout=fopen(argv[2], "w");


	init(fin);
	GA();
	answer(stdout);

	return 0;
}
