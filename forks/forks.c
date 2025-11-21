#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define N_PROCS 10

/*
    Produces a seed for RNG using the current passed integer
    and system time at roughly the time of calling.
    Best used along with some random shuffling of the RNG
    to ensure some more varied results.
*/
long int generate_seed(int i){
    // Get current time to serve as seed
    long int t = time(NULL)*(-1);
    // Shuffle the value of t
    t = t >> (i+1);
    t = t ^ 1;
    t *= -1;
    t = t << ((i+1)*2);
    t |= 10;
    t = t >> ((i+1)/2);
    t += 100;
    t = t << (i/3);
    t *= -1;
    t = t >> (5*i);
    /*
        Reminder on bitwise operators:
        - A >> B : shift A right B bits
        - A << B : shift A left B bits
        - A ^ B  : A XOR B
        - A | B  : A OR B
        - (Unused) A & B : A AND B
    */
    return t;
}

void seed_rng(int i){
    /*
        Seed RNG in a two steps process:
        1. Generate a seed and apply it to random's RNG algorithm
        2. Shuffle the algorithm a random number of times to introduce additional
           (Jensen-Shannon) entropy into the system
    */
    srand(generate_seed(i)); // Use generated value as RNG seed
    int calls = rand() % 2000; // Up to 2000 discarded calls to rand to really shuffle things
    for(int j = 0; j <calls; j++) rand(); // rand() into the void just to really shuffle RNG 
}
/*
	A brief explanation on seed_rng: rand() generates a deterministic sequence of values. This sequence loops after 
	reaching its final value, ensuring the function can be called however many times are needed, and follows a similar
	scheme to the generate_seed function included in this very file (but, of course, it is a much more complicated 
	implementation). Because the input to the algorithm used by rand is its last output, it can a) be seeded by providing
	an arbitrary input (e.g., current time in milliseconds), and b) shuffled by repeatedly calling itself. The point of this 
	seed_rng function is, then, to first generate an initial state and number of shuffles, and then use that starting point
	to further modify the state of the RNG algorithm. This, in turn, induces additional randomness. 
	
	This convoluted approach is wholly unnecessary, as, for applications that require more randomness (e.g., cryptographic 
	applications), there are significantly more effective algorithms that can be implemented. Take it as an example and an 
	academic exercise, rather than a display of best practices.
*/

int main(){
    pid_t child_pid, wpid;
    int status = 0;
    for(int i=0; i<N_PROCS; i++){
        if((child_pid = fork()) == 0){
            seed_rng(i);
            sleep(rand()%6); // Sleep for a bit
            printf("Child %d wakes up\n", i+1);
            seed_rng(i);
            exit(rand()%2); // Flip a coint on whether to return 0 or 1
        }
    }
    printf("I'm the father\n");
    int non_zero = 0;
    while((wpid = wait(&status) > 0)){
		/* Note that, since there are no curly braces after the first if, C will only
		 run the very next instruction if the condition is met. This structure is, 
		 of course, recursive.
		 
		 In terms of readibility, this structure is potentially detrimental: it can
		 easily obscure semantic structures and, therefore, the intent of the code.
		 Nevertheless, it is common enough that having a documented example should facilitate 
		 reading existing code samples from the Internet.
		 */
        if(wpid > 0) if(WIFEXITED(status)){ // WIFEXITED decodes whether the child process exited normally
            if(WEXITSTATUS(status) != 0) non_zero++; // Count number of non-zero finishes
													 // WEXISTSTATUS performs the actual value decoding from status
        }
    }
    printf("All children exited\n");
    printf("\tChildren with exit code = 0:\t%d\n\tChildren with exit code != 0:\t%d\n", N_PROCS-non_zero, non_zero);
    
    return 0;
}

