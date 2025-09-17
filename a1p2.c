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
    int rem = (((upper-lower)+1)%N)-1;
    int add = ((int)(((upper-lower)+1)/N))*N;
    int y = lower;
    int i;
    pid_t pid;
    for (i = 0; i < N; i++)
    {
        pid_t pid;
        pid = fork();
        if (pid < 0) {
            printf("Fork Failed");
            exit(-1);
        }
        else if (pid == 0) {
            break;
        }
        else {
            y+=add;
            if (rem-i>0 || i==N-1)
            {
                y++;
            }
            

        }
    }

    if (pid==0)
    {
        if (rem-i>0 || i==N-1)
            {
                add++;
            }
        
        for (int p = y; i < (y+add); i++)
        {
            is_prime(p);
        }
        
        
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
