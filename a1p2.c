#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

int is_prime(int num);

int main(int argc, char *argv[]){
    pid_t pid=0;
    int lower = atoi(argv[1]);
    int upper = atoi(argv[2]);
    int N = atoi(argv[3]);
    int tot = (upper-lower)+1;
    int i=0;
    if (tot <N){
        N=tot;
    }
    int rem = (tot%N);
    int add = ((tot-rem)/N);
    int y = lower;
    int id = shmget(IPC_PRIVATE, tot*sizeof(int),IPC_CREAT|0666);
    int *foundPrimes = (int *)shmat(id,NULL,0);
    memset(foundPrimes,-1,tot*sizeof(int));
    for (i = 0; i < N; i++){
        int top = add;
        if (rem-i>1||(rem!=0&&N-i==1)){
            top++;
        }
        pid = fork();
        if (pid < 0) {
            printf("Fork Failed");
            exit(-1);
        }
        else if (pid == 0) {
            for (int p = y; p < (y+top)+1; p++){
                if(is_prime(p)==1){
                    foundPrimes[p-lower] = p;
                }
            }
            exit(0);
        }
        else {
            printf("Child PID checking %d checking range [%d, %d]\n",pid,y,((y+top)-1));
            y+=top;  
        }
    }
    printf("\nParent: All children finished. Primes found:\n");
    int isWaiting =0;
    while (isWaiting!=-1){
        isWaiting=wait(NULL);
    }
    for (int u = 0; u < tot; u++){
        
        if (foundPrimes[u]>0){
            printf("%d",foundPrimes[u]);
            if (u!=tot-1){
                printf(" ");
            }   
        }
    }
    printf("\n");
    shmctl (id, IPC_RMID,NULL);
    return(0);
}

int is_prime(int num) {
    if (num < 2) return 0;
        for (int i = 2; i <= sqrt(num); i++) {
            if (num % i == 0) return 0;
        }
    return 1;
}
