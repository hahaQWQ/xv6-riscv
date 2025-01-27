#include "kernel/types.h"
#include "user/user.h"
 __attribute__((noreturn)) void primes(int pipe_in) 
{
    // declare variables for prime and num.
    int prime,num;

    // declare pipes file descriptors.
    int p[2];
    
    // if a prime number received, create a new process to filter out multiples of this prime.
    if(read(pipe_in,&prime,sizeof(int))>0)
    {
        printf("prime %d\n",prime);
        // create pipe to communication between parent and child process.
        pipe(p);
        if(fork()==0)
        {
            // close the input pipe because it is no longer needed in this process.
            close(pipe_in);
            // the child process only reads from the pipe. so close the write end to free resources.
            close(p[1]);
            // Recursively call primes using the read end of the pipe.
            primes(p[0]);
            // finally, close the read end of the pipe.
            close(p[0]);
        }else{
            // the parent process only sends numbers that are not multiples of this prims.
            close(p[0]);
            // read numbers from the input pipe.
            while (read(pipe_in,&num,sizeof(int))>0)
            {
                // check if num is a multiple of this prime.
                if(num % prime != 0)
                {
                    // write it to the child process's pipe.
                    write(p[1],&num,sizeof(int));
                }
            }
            // close the write end of pipe.
            close(p[1]);
            // close the input pipe.
            close(pipe_in);
            // wait for thie child process to finish.
            wait(0);
        }
    }
    exit(0);
}
int main(int argc, char* argv[])
{
    // declare pipes of file desciptors
    int p[2];
    // create pipe
    pipe(p);
    // fork a process
    if(fork()==0)
    {
        // close the write end of pipe
        close(p[1]);
        // Recursively call primes 
        primes(p[0]);
        // close the read end of pipe
        close(p[0]);
    }else{
        // close the read end of pipe
        close(p[0]);
        for(int i=2;i<=280;i++)
        {
            write(p[1],&i,sizeof i);
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}