#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    /*
        Cautionary notes:
        1. The string type does not exist (by default) in C. It is instead defined as either
           char[] or char* (they are technically different, but let's not worry about it yet)
        2. sizeof is an utility call that C provides so we can extract the size of an object
           in memory, or of a type. We'll be using it directly on the object here for convenience,
           but other examples of valid calls include sizeof(int) or sizeof(struct myStruct)
           (the latter demands that we have defined a struct called myStruct)
    */
    char str[] = "Example string written to file.\n";
    char target_file[] = "output_dump.txt"; // Note that extensions are not needed
                                            // in UNIX; this is just for clarity
    int mode = O_WRONLY | O_CREAT; // Aliases can be used instead of raw integers
    // If we are writing to an existing file, that's perfect
    // If not, we need to create it. We will use some fairly standard
    // permissions (the user can manually change them later)
    int flags = S_IRUSR | S_IWUSR | S_IRGRP; // Permissions: -wr--r----
    int fd = open(target_file, mode, flags); // Syscall to open a file (from fcntl.h)
    if(fd == -1){ // Did we fail to open the file?
        perror("Error opening file");
        exit(1); // Exit while showing error message
    }
    printf("Opened file: '%s'\nPermissions (if created): %o\n", target_file, flags);
    // If we are here, the file was opened successfully
    write(fd, str, sizeof(str)); // Write our string to the file
    close(fd); // Very important!
    printf("%ld bytes written.\n", sizeof(str));
    /*
        close will do two things for us:
        a. It will signal the OS that we are done with the file, and it can be freed up
           (i.e., any access modes that are valid for the current user are back on the table)
        b. It will signal that any yet unwritten changes need to be flushed now so the file can
           be closed properly (the process is actually asynchronous!)
    */

    // We are done and we have cleaned up. Time to wrap up by returning control to
    // the OS
    return 0;
}
