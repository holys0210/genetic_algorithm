#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define MAXN	318 // Maximum value of N
#define PSIZE	100 // Size of the population

/*****************************************************************
	Input variables
*****************************************************************/
// (X[i], Y[i]) := the i-th location
double X[MAXN], Y[MAXN];
// The number of locations
int N;

// Time limit for the test case
long long TimeLimit;


// Dist[i][j] := the distance between (X[i], Y[i]) and (X[j], Y[j])
// will be automatically calculated
double Dist[MAXN][MAXN];



/*****************************************************************
	GA variables and functions
	Note that the representation is currently order-based.
	A chromosome will be a permutation of (0, 1, ..., N-1).
*****************************************************************/
typedef struct {
	int ch[MAXN];	// chromosome
	double f;		// fitness
} SOL;

// population of solutions
SOL population[PSIZE];

// best (found) solution
// eval() updates this
SOL record;


// calculate the fitness of s and store it into s->f
double eval(SOL *s) {
	int i;

	s->f = 0;
	for (i = 0; i < N; i++) {
		s->f += Dist[s->ch[i]][s->ch[(i+1)%N]];
	}

	if (record.f > s->f) {
		record.f = s->f;
		for (i = 0; i < N; i++) {
			record.ch[i] = s->ch[i];
		}
	}

	return s->f;
}


// generate a random order-based solution at s
void gen_rand_sol(SOL *s) {
	int i;
	for (i = 0; i < N; i++) {
		s->ch[i] = i;
	}
	for (i = 0; i < N; i++) {
		int r = i + rand() % (N - i);	// r is a random number in [i..N-i)
		int tmp = s->ch[i]; s->ch[i] = s->ch[r]; s->ch[r] = tmp; // swap
	}
	
	// calculate the fitness
	eval(s);
}


// choose one solution from the population
// currently this operator randomly chooses one w/ uniform distribution
void selection(SOL **p)
{
	*p = &population[rand() % PSIZE];
}


// combine the given parents p1 and p2
// and store the generated solution at c
// currently the child will be same as p1
void crossover(const SOL *p1, const SOL *p2, SOL *c) {
	int i;
	for (i = 0; i < N; i++) {
		c->ch[i] = p1->ch[i];
	}
	eval(c);
}


// mutate the solution s
// currently this operator does nothing
void mutation(SOL *s) {
	/*
		EMPTY
	*/
	eval(s);
}


// replace one solution from the population with the new offspring
// currently any random solution can be replaced
void replacement(const SOL *offspr) {
	int i, p = rand() % PSIZE;
	population[p].f = offspr->f;
	for (i = 0; i < N; i++) {
		population[p].ch[i] = offspr->ch[i];
	}
}


// a "steady-state" GA
void GA() {
	int i;
	SOL c;
	time_t begin = time(NULL);

	for (i = 0; i < PSIZE; i++) {
		gen_rand_sol(&population[i]);
	}

	while (1) {
		if(time(NULL) - begin >= TimeLimit - 1) return; // end condition
		SOL *p1, *p2;
		selection(&p1); selection(&p2);
		crossover(p1, p2, &c);
		mutation(&c);
		replacement(&c);
	}
}


// read the test case from stdin
// and initialize some values such as record.f and Dist
void init() {
	int i, j, tmp;
	double time_limit;

	tmp = scanf("%d", &N);
	for (i = 0; i < N; i++) {
		tmp = scanf("%lf %lf", &X[i], &Y[i]);
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			double dx = X[i] - X[j], dy = Y[i] - Y[j];
			Dist[i][j] = sqrt(dx*dx + dy*dy);
		}
	}
	tmp = scanf("%lf", &time_limit);
	TimeLimit = (long long) time_limit;

	record.f = 1e100;
}


// print the best solution found to stdout
void answer() {
	int i;

	for (i = 0; i < N; i++) {
		if (i > 0) printf(" ");
		printf("%d", record.ch[i]+1);
	}
	printf("\n");
}


int main() {
	srand(time(NULL));

	init();
	GA();
	answer();
	return 0;
}

