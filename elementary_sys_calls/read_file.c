#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// We'll allocate ~1KB of memory for reading
#define MAX_BUFFER 1024 

int main(){
    char target_file[] = "output_dump.txt"; // Assume we have written here first
    char* read_buffer; // Splitting the process in parts: first, we declare the buffer.
    
    int mode = O_RDONLY;
    int flags = 0; // They are still required, but we don't need them since we are not creating a file
    // We won't allocate any memory until we are sure we have succesfully read the file
    int fd = open(target_file, mode, flags);
    if(fd == -1){
        perror("File could not be opened for reading");
        exit(1);
    }
    // File is open, let's read the contents
    int allocation_size = MAX_BUFFER*sizeof(char);
    // malloc: asks the OS to allocate RAM for us; returns pointer to allocated memory
    read_buffer = malloc(allocation_size); // Best practice; sizeof(char) is essentially 1
    if(read_buffer == NULL){
        // Allocation failed
        perror("Could not allocate buffer memory");
        exit(1);
    }
    printf("Allocated %d bytes to buffer\n(Memory position = 0x%X)\n", allocation_size, read_buffer);
    int read_bytes = read(fd, read_buffer, allocation_size); // Read a maximum of allocation_size bytes into buffer
    // We are done reading. Close the file
    close(fd);
    // Show what we read; note that it is in memory even with the file closed
    printf("Read contents (%d bytes):\n'%s'\n", read_bytes, read_buffer);
    // We are done with our buffer. Free the memory to avoid leaks
    free(read_buffer);
    // Now we can exit safely
    return 0;
    
}
