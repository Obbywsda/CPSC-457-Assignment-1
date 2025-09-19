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
    if (N<=0){
        //ensures that at least one child is created
        printf("Number of Children can't be zero");
        return(0);
    }
    if (lower>upper){
        //ensures that bottom of range isn't greater than top of range
        printf("start of range can't be less than end of range");
        return(0);
    }
    //gets the number of numbers needing to be checked in the inclusive range lower, upper
    int rangeCount = (upper-lower)+1;
    //sets N to the number of Numbers to check if the given value of N is greater that the number of nums
    if (rangeCount <N){
        N=rangeCount;
    }
    //gets how many children will need to check an additional number 
    int rem = (rangeCount%N);
    //gets the base number of numbers each child will need to check. Some will need to check one more if rem above isn't zero
    int add = ((rangeCount-rem)/N);
    //records the bottom of the child to be created nexts range
    int child_base = lower;
    //creates a shared memory at gets its pointer
    int shm_id = shmget(IPC_PRIVATE, rangeCount*sizeof(int),IPC_CREAT|0666);
    //attaches the shared memory
    int *foundPrimes = (int *)shmat(shm_id,NULL,0);
    //fills the memory with -1 
    memset(foundPrimes,-1,rangeCount*sizeof(int));
    //loops to create the children
    for (int NUM_OF_CHILD = 0; NUM_OF_CHILD < N; NUM_OF_CHILD++){
        //gets the max number of primes
        int MAX_PRIMES_PER_CHILD = add;
        //adds one to the max number if that child has had one added to its top of range for even distribution
        if (rem>NUM_OF_CHILD){
            MAX_PRIMES_PER_CHILD++;
        }
        //generates child
        pid = fork();
        //reports error and kills program if child failed to generate
        if (pid < 0) {
            printf("Fork Failed");
            exit(-1);
        }
        //code the only the children will run
        else if (pid == 0) {
            //loop for each child to check all numbers in its range, where child_base+MAX_PRIMES_PER_CHILD will be one greater than the range
            for (int p = child_base; p < (child_base+MAX_PRIMES_PER_CHILD); p++){
                //runs is_prime
                if(is_prime(p)==1){
                    //adds to shared memory if is_prime returns 1
                    //each section of mem is protected as p-lower returns what number of num that number is in the range as an array index.
                    foundPrimes[p-lower] = p;
                }
            }
            //detaches memory
            shmdt(foundPrimes);
            //exits child
            _exit(0);
        }
        //only ran by parent
        else {
            //prints the id of most recently created child, and the range its checking
            printf("Child PID checking %d checking range [%d, %d]\n",pid,child_base,((child_base+MAX_PRIMES_PER_CHILD)-1));
            //updates child base for next child
            child_base+=MAX_PRIMES_PER_CHILD;  
        }
    }
    //checks if all children have finished as wait will return -1 if no children exist.
    int isWaiting =0;
    while (isWaiting!=-1){
        isWaiting=wait(NULL);
    }
    //prints once children done
    printf("\nParent: All children finished. Primes found:\n");
    //loops through all values in mem
    for (int u = 0; u < rangeCount; u++){
        //checks if value is greater than 0, skipping all indexs that had no primes since we filled with -1 before adding any primes
        //also protects against negatives
        if (foundPrimes[u]>0){
            printf("%d",foundPrimes[u]);
            if (u!=rangeCount-1){
               //prints space between numbers 
                printf(" ");
            }   
        }
    }
    printf("\n");
    //clears memory
    shmctl (shm_id, IPC_RMID,NULL);
    return(0);
}

int is_prime(int num) {
    if (num < 2) return 0;
        for (int NUM_OF_CHILD = 2; NUM_OF_CHILD <= sqrt(num); NUM_OF_CHILD++) {
            if (num % NUM_OF_CHILD == 0) return 0;
        }
    return 1;
}
