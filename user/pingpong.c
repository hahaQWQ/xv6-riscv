#include "kernel/types.h"
#include "user/user.h"


int main()
{
    int p[2];
    if(pipe(p)<0){
        printf("failed to create pipe.");
        exit(0);
    }
    int pid = fork();
    if(pid==0)
    {
        char b[1]={};
        read(p[0],b,1);
        printf("%d: received pong\n",getpid());
    }else{
        printf("%d: received ping\n",getpid());
        char b = '@';
        write(p[1],&b,1);
        wait(0);
    }
    exit(0);
}