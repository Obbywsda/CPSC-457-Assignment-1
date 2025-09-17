#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int is_prime(int num);

int main(int argc, char *argv[]){
    int lower = atoi(argv[1]);
    int upper = atoi(argv[2]);
    int N = atoi(argv[3]);
    
    if ((upper-lower)+1 <N)
    {
        N=(upper-lower)+1;
        
    }
    double c = ((upper-lower)+1)/N;
    int y = lower;
    int i;
    pid_t pid;
    for (i = 0; i < N; i++)
    {
        
        pid = fork();
        if (pid < 0) {
            printf("Fork Failed");
            exit(-1);
        }
        else if (pid == 0) {
            break;
        }
        else {
            y+=(((int)(c*(i+1)-((int)(c*i)))));

        }
    }

    if (pid==0)
    {
        
    }
    
    
    
    


    
    exit(0);
}

int is_prime(int num) {
    if (num < 2) return 0;
        for (int i = 2; i <= sqrt(num); i++) {
            if (num % i == 0) return 0;
        }
    return 1;
}
