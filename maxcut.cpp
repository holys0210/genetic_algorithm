#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_N 1000
#define SOL_N 50
#define MUT_CRI 0.2
#define MAX_TIME 175
#define LOC_N 16
#define K 3

int Vertex_N, Edge_N;
int edge[MAX_N+1][MAX_N+1];
	int mx=-987765321;

typedef struct {
	bool chromosome[MAX_N+1];
	int quality;
} solution;

vector<solution> population(SOL_N);
solution* best;
solution asdf;

void find_min(solution**);
void find_max(solution**);
void init_chrom(solution*);

int sum_of_f;
double fn[SOL_N];

int eval(solution* s){
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
	return sum;
}
bool sol_comp(solution a, solution b){
	return a.quality < b.quality;
}

void roulett_init(){
	sum_of_f=0;
	solution*lbest, *lworst;
	find_max(&lbest);
	find_min(&lworst);

	int best_f=lbest->quality, worst_f= lworst->quality;
	double cons=(double)worst_f-(double)best_f;
	for(int i=0; i<SOL_N; i++){
		fn[i]=(double)(worst_f-population[i].quality)+cons/(double)(K-1);
		sum_of_f+=fn[i];
	}
}

void roulett_selction(solution** s1, solution** s2){
	 int r1, r2;
	// roullet
	roulett_init();
	//TODO
	 int sum=0;
	 int pt=rand()%sum_of_f;
	 //r1
	 for(int i=0; i<SOL_N; i++){
		 sum+=fn[i];
		 if(pt<sum){
			 r1=i;
		 }

	 }
	 //r2
	 sum=0;
	 for(int i=0; i<SOL_N; i++){
		 sum+=fn[i];
		 if(pt<sum){
			 r2=i;
		 }
	 }
	*s1=&population[r1];
	*s2=&population[r2];
}
void rank_init(){
	sum_of_f=0;

	int best_f=Vertex_N, worst_f= 1;
	double cons=((double)worst_f-(double)best_f)/(double)(Vertex_N-1);
	for(int i=0; i<SOL_N; i++){
		fn[i]=(double)(Vertex_N)+cons*(double)i;
		sum_of_f+=fn[i];
	}
}


void rank_selection(solution** s1, solution** s2){
	sort(population.begin(), population.end(), sol_comp);
	int r1=SOL_N/2+1;
	int r2=SOL_N/2;

	rank_init();
	 int sum=0;
	 int pt=rand()%sum_of_f;
	 //r1
	 for(int i=0; i<SOL_N; i++){
		 sum+=fn[i];
		 if(pt<sum){
			 r1=i;
		 }

	 }
	 //r2
	 sum=0;
	 for(int i=0; i<SOL_N; i++){
		 sum+=fn[i];
		 if(pt<sum){
			 r2=i;
		 }
	 }
	*s1=&population[r1];
	*s2=&population[r2];
}

void random_selection(solution** s1, solution** s2){
	int r1=rand()%SOL_N;
	int r2=rand()%SOL_N;
	while(r1==r2){
		r2=rand()%SOL_N;
	}
	*s1=&population[r1];
	*s2=&population[r2];

}

void one_pt_crossover(solution* p1, solution* p2, solution* c){
	int xover_pt=rand()%Vertex_N;
	for(int i=1; i<=xover_pt; i++){
		c->chromosome[i]=p1->chromosome[i];
	}
	for(int i=xover_pt+1; i<=Vertex_N; i++){
		c->chromosome[i]=p2->chromosome[i];
	}
}

void multi_pt_crossover(solution* p1, solution* p2, solution* c){

	std::vector<int> v;

	for(int i=0; i<2; i++){
		v.push_back(rand()%Vertex_N);
	}
	std::sort(v.begin(), v.end());

	for(int i=1; i<=v[0]; i++){
		c->chromosome[i]=p1->chromosome[i];
	}
	for(int i=v[0]+1; i<=v[1]; i++){
		c->chromosome[i]=p2->chromosome[i];
	}
	for(int i=v[1]+1; i<=Vertex_N; i++){
		c->chromosome[i]=p1->chromosome[i];
	}
}


void uniform_crossover(solution* p1, solution* p2, solution* c){
	double cri=0.2;
	solution* a, *b;
	if(p1->quality> p2->quality){
		a=p1;
		b=p2;
	}
	else{
		a=p2;
		b=p1;
	}

	double r;
	for(int i=1; i<=Vertex_N; i++){
		r=(double)rand()/(double)RAND_MAX;
		
		if(r>cri){
			c->chromosome[i]=a->chromosome[i];
		}
		else{
			c->chromosome[i]=b->chromosome[i];
		}
	}

}

void uni_mutation(solution* c){
	double r;
	for(int k=1; k<=Vertex_N; k++){
		r=(float)rand()/(float)(RAND_MAX);
		if(r<MUT_CRI){
			c->chromosome[k]= !(c->chromosome[k]);
		}
	}
	int r1=rand()%Vertex_N;
	int r2=rand()%Vertex_N;
	c->chromosome[r1]= !(c->chromosome[r1]);
	c->chromosome[r2]= !(c->chromosome[r2]);

	// evaluate quality
	c->quality=eval(c);
}


void non_uni_mutation(solution* c){
	double r;
	double a=MUT_CRI*(1.0-(double)time(0)/MAX_TIME);
	for(int k=1; k<=Vertex_N; k++){
		r=(float)rand()/(float)(RAND_MAX);
		if(r<MUT_CRI){
			c->chromosome[k]= !(c->chromosome[k]);
		}
	}

	int r1=rand()%Vertex_N;
	int r2=rand()%Vertex_N;
	c->chromosome[r1]= !(c->chromosome[r1]);
	c->chromosome[r2]= !(c->chromosome[r2]);

	// evaluate quality
	c->quality=eval(c);

}

int sim(solution a, solution* b){
	int sum=0;
	for(int i=0; i<Vertex_N; i++){
		if(a.chromosome[i]^b->chromosome[i]){
			sum++;
		}
	}
	return sum;
}

void pop_replacement(solution* p1, solution* p2, solution* c){
	solution* p;
	find_min(&p);
	p->quality=c->quality;
}

void parent_replacement(solution* p1, solution* p2, solution* c){
	solution* p;
	if(p1->quality>p2->quality){
		p=p2;
	}
	else{
		p=p1;
	}
	p->quality=c->quality;
}

void sim_replacement(solution* p1, solution* p2, solution* c){
	int m, mi;
	m=sim(population[0], c);
	mi=0;
	int temp;
	for(int i=1; i<SOL_N; i++){
		temp=sim(population[i], c);
		if(temp>m){
			m=temp;
			mi=i;
		}
	}
	solution* p=&population[mi];

	bool* p_ch=p->chromosome, *c_ch=c->chromosome;

	for(int k=1; k<Vertex_N; k++){
		p_ch[k]=c_ch[k];
	}
	p->quality=c->quality;
}
	
void replacement(solution* p1, solution* p2, solution* c){

	solution* p;
	// parent								if child > parent
	// worst in population	o.w.
	//TODO

	p->quality=c->quality;
}

void find_best(solution** a, solution** b){
	int x, y;
	int c, d;
	x=0;
	y=0;
	c=population[0].quality;
	d=population[0].quality;

	for(int i=1; i<SOL_N; i++){
		if(d<population[i].quality){
			x=y;
			y=i;
			c=d;
			d=population[i].quality;
		}
	}
	*a=&population[x];
	*b=&population[y];
//	printf("%d %d", x, y);
}
void find_worst(solution** a, solution** b){
	int x, y;
	int c, d;
	x=0;
	y=0;
	c=population[0].quality;
	d=population[0].quality;

	for(int i=1; i<SOL_N; i++){
		if(d>population[i].quality){
			x=y;
			y=i;
			c=d;
			d=population[i].quality;
		}
	}
	*a=&population[x];
	*b=&population[y];
//	printf("%d %d", x, y);
}


bool delta(solution s, int index){
	s.chromosome[index] = !s.chromosome[index];

	return eval(&s) > s.quality;
}

int eval_modify(solution* s, int index){
	bool* chro = s->chromosome;
	int sum=0;
	for(int j=1; j<=Vertex_N; j++){
		bool b=chro[index];
		if(b^(chro[j])){
			sum-=edge[index][j];
		}
		else{
			sum+=edge[index][j];
		}

	}
	return sum;


}

solution local_opt(solution* sol){
	vector<int> perm;
	
	for(int i=1; i<=Vertex_N; i++){
		perm.push_back(i);
	}

	random_shuffle(perm.begin(), perm.end());

	bool improved=true;
	int new_qual;

	while(improved){
		improved=false;

		for(int i=1; i<=Vertex_N; i++){
			new_qual = eval_modify(sol, i);

			if(new_qual > 0){
				sol->chromosome[i] = !sol->chromosome[i];
				sol->quality+=new_qual;
				improved=true;
			}
		}
	}

}

void multilocal_opt(){
	int max_i;
	int max_qual;

	vector<solution> sol(LOC_N);
	for(int step=0; step<LOC_N; step++){
		init_chrom(&sol[step]);
		local_opt(&sol[step]);
	}
	solution msol=*max_element(sol.begin(), sol.end(), sol_comp);
}


void GA(){
	time_t start=time(0);

	int index=0;

	solution *p1, *p2;
	solution c;
	// 180second
	solution* a;
	int st=0;
	while((time(0)-start) < MAX_TIME){
		// selection
		/*
		roulett_selction(&p1, &p2);
		rank_selection(&p1, &p2);
		random_selection(&p1, &p2);
		*/
		random_selection(&p1, &p2);

		find_max(&p1);
		find_min(&p2);

		// crossover
		/*
		one_pt_crossover(p1, p2, &c);
		multi_pt_crossover(p1, p2, &c);
		uniform_crossover(p1, p2, &c);
		*/
		uniform_crossover(p1, p2, &c);


		// mutation
		/*
		uni_mutation(&c);
		non_uni_mutation(&c);
		*/

		uni_mutation(&c);
		// local optimization
		local_opt(&c);
//		multilocal_opt();

		// replacement
		/*
		pop_replacement(p1, p2, &c);
		parent_replacement(p1, p2, &c);
		sim_replacement(p1, p2, &c);
		*/
		sim_replacement(p1, p2, &c);

		int sum=0;
		for(int i=0; i<SOL_N; i++){
			sum+=population[i].quality;
		}
		find_max(&best);
		//printf("%d : %d\n",sum, best->quality);
		if(mx<best->quality){
			mx=best->quality;
			asdf=*best;
		}
		st++;


	}
	printf("%d\n", st);


	return ;
}

void init_chrom(solution* s){
	for(int k=1; k<=Vertex_N; k++){
		s->chromosome[k]=rand()%2;
	}
	s->quality=eval(s);
	
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
		init_chrom(&population[sol_i]);
	}

	return ;
}

void find_min(solution** s){
	int min_f=population[0].quality, min_i=0;
	for(int i=1; i<SOL_N; i++){
		if(min_f>population[i].quality){
			min_f=population[i].quality;
			min_i=i;
		}
	}
	*s=&population[min_i];
}

void find_max(solution** s){
	int max_f=population[0].quality, max_i=0;
	for(int i=1; i<SOL_N; i++){
		if(max_f<population[i].quality){
			max_f=population[i].quality;
			max_i=i;
		}
	}
	*s=&population[max_i];
}



void answer(FILE *fp){
	bool *b=asdf.chromosome;
	for(int i=1; i<=Vertex_N; i++){
		if(b[i]){
//			fprintf(fp, "%d ", i);
		}
	}
	fprintf(fp, "quality : %d\n", asdf.quality); 
	/*
	for(int i=1; i<=SOL_N; i++){
		printf("%d : %d\n", i, population[i].quality);
	}
	*/
}

int main(int argc, char** argv){
	srand(time(NULL));

	FILE *fin=fopen(argv[1], "r");
	FILE *fout=fopen(argv[2], "w");


	init(fin);
//	multilocal_opt();
	GA();
	answer(stdout);

	return 0;
}
