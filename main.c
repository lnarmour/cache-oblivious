#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


struct timeval time;
double elapsed_time;

#define min(x, y)   ((x)>(y) ? (y) : (x))
#define start_timer() gettimeofday(&time, NULL); elapsed_time = (((double) time.tv_sec) + ((double) time.tv_usec)/1000000)
#define stop_timer() gettimeofday(&time, NULL); elapsed_time = (((double) time.tv_sec) + ((double) time.tv_usec)/1000000) - elapsed_time


#define A(i,j) A[(i)*N+(j)]
#define B(i,j) B[(i)*N+(j)]
#define M(i,j) M[(i)*N+(j)]

void printM(int *M, int N) {
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      printf("%3d ", M(i,j));
    }
    printf("\n");
  }
  printf("\n");
}

// naive
void naive(int N, int *A, int *B) {
  for (int i=0; i<N; i++)
    for (int j=0; j<N; j++)
      B(j,i) = A(i,j);
}

// tiled-oblivious
void transpose_oblivious(int p, int *A, int *B, int N) {
  // base case
  if (p == 2) {
    for (int i=0; i<p; i++)
      for (int j=0; j<p; j++)
        B(j,i) = A(i,j);
    return;
  }

  int q = p / 2;
  transpose_oblivious(q, &A(0,0), &B(0,0), N);
  transpose_oblivious(q, &A(0,q), &B(q,0), N);
  transpose_oblivious(q, &A(q,0), &B(0,q), N);
  transpose_oblivious(q, &A(q,q), &B(q,q), N);
}

// tiled-aware
void transpose_aware(int N, int TS, int *A, int *B) {
  for (int ti=0; ti<N; ti+=TS)
    for (int tj=0; tj<N; tj+=TS)
      for (int i=ti; i<min(N,ti+TS); i++)
        for (int j=tj; j<min(N,tj+TS); j++)
          B(j,i) = A(i,j);
}


int main(int argc, char *argv[]) {

  #ifndef AWARE
  if (argc < 2) {
    printf("usage: %s N\n", argv[0]);
    return 1;
  }
  #else
  if (argc < 3) {
    printf("usage: %s N TS\n", argv[0]);
    return 1;
  }
  int TS = atoi(argv[2]);
  #endif

  int N = atoi(argv[1]);

  int *A = (int*)malloc(N*N*sizeof(int));
  int *B = (int*)malloc(N*N*sizeof(int));

  // initialize
  for (int i=0; i<N; i++)
  for (int j=0; j<N; j++)
    A(i,j) = i*N+j;

  start_timer();
  for (int c=0; c<100; c++) {
    // main computation
    #ifdef NAIVE
    naive(N, A, B);
    #elif defined OBLIVIOUS
    transpose_oblivious(N, A, B, N);
    #elif defined AWARE
    transpose_aware(N, TS, A, B);
    #endif
  }
  stop_timer();

  #ifndef AWARE
  if (argc > 2 && atoi(argv[2]) == 1) {
  #else
  if (argc > 3 && atoi(argv[3]) == 1) {
  #endif
    printM(A, N);
    printM(B, N);
  }

  printf("%f\n", elapsed_time);

}
