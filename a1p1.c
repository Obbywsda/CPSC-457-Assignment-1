// CPSC 457
// a1p1


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// max size of matrix
int ROWS = 100;
int COLS = 1000;

// read only 100x1000 binary digits only and no white spaces
int read_matrix(int a[ROWS][COLS]) {

    int need = ROWS * COLS; // the total spaces in the matrix
    int count = 0; 
    int ch;

    // while loop to fill up a 2d array
    while ((ch = fgetc(stdin)) != EOF && count < need) {

        if (ch == '0' || ch == '1') {

            a[count / COLS][count % COLS] = (ch - '0');
            count++;
        }
        // ignore white spaces 
    }


    // error check to see if the input is exactly 100x1000, if the count is larger than the size then theres a problem
    if (count != need) {

        printf("Input size too big");
        return -1;
    }
    return 0;
}

int main(void) {

    
    // initialize matrix and fill it with the contents of the input text file
    int matrix[ROWS][COLS];
    if (read_matrix(matrix) != 0){ 

        fprintf(stderr, "error");
        return 1;
    }
    pid_t pids[ROWS]; //each id of child will scan a row

    // create 100 children, one per row
    for (int i = 0; i < ROWS; i++) {

        pid_t pid = fork();

        if (pid < 0) {

            perror("fork");
            // stop any started children before exiting
            for (int k = 0; k < i; k++) {

                int st;
                waitpid(pids[k], &st, 0); // wait for all ids to finish scanning
            }
            return 1;
        }
        //child process
        if (pid == 0) {

            //get pid and print child with id and row
            pid_t mypid = getpid();
            printf("Child %d (PID %d): Searching row %d\n", i, mypid, i);

            // scan row i exit(1) if a 1 is found, else exit(0)
            for (int j = 0; j < COLS; j++) {

                if (matrix[i][j] == 1) exit(1); //1 signals success to parent
            }
            exit(0); 
        } //end child process

        // parent stores mapping from row to pid
        pids[i] = pid;
    }

    // parent process waits for all children to finish
    int found_row = -1;
    pid_t found_pid = -1;

    //loop to stop child proccesses
    for (int k = 0; k < ROWS; k++) {

        int status = 0;
        pid_t wpid = wait(&status);

        if (wpid == -1) {
            perror("wait");
            return 1;
        }

        //if a child exited with 1 save the row
        if (WIFEXITED(status) && WEXITSTATUS(status) == 1 && found_row == -1) {

            // use pid to find  row index
            for (int i = 0; i < ROWS; i++) {

                if (pids[i] == wpid) {
                    found_row = i;
                    found_pid = wpid;
                    break;
                }
            }
        }
    }
    
    //no treasure
    if (found_row == -1) {
        printf("Parent: No treasure was found\n");
        return 0;
    }

    // parent find the 1 in the column
    int found_col = -1;
    for (int j = 0; j < COLS; j++) {

        if (matrix[found_row][j] == 1) {
            found_col = j;
            break;
        }
    }

    //print where the teasure is found
    printf("Parent: The treasure was found by child with PID %d at row %d and column %d \n",
           found_pid, found_row, found_col);
    return 0;
}

}
