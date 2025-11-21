#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define N_PROCS 5

int factorial(int x){
    printf("Computing %u!\n", x);
    int mult = 1;
    for(int i=x; i>0; i--){
        mult *= i;
    }
    return mult;
}

int main(){
    pid_t child_pid, wpid;
    int status = 0;
    int accum = 0;
    for(int i=0; i<N_PROCS; i++){
        if((child_pid = fork()) == 0){
            srand(time(NULL)); // Seed RNG with current system time
            sleep(rand()%2); // Sleep randomly for up to 5 seconds
            int result = factorial(i+1);    // Compute the factorial of assigned integer
            exit(result);   // Return computed factorial to parent
        }
    }
    while((wpid = wait(&status) > 0)){ // Wait until a child finishes
        if(wpid > 0)    // Check that the finish is correct (it should be if we are here)
            if(WIFEXITED(status)){ // Check exit code for validity
                int computed_value = WEXITSTATUS(status);   // Decode actual exit code (x!)
                accum += computed_value;    // Add factorial to our sum
            }
    }
    printf("All children exited\nTotal sum = %d\n(should be 153 for N = 5; current N = %d)\n", accum, N_PROCS);
    
    return 0;
}

