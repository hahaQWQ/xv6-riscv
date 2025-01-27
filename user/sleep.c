#include "kernel/types.h"
#include "user/user.h"



int main(int argc, char *argv[])
{
    if(argc>1)
    {
        int n = atoi(argv[1]);
        sleep(n);
    }else{
        printf("error\n");
    }
    

    exit(0);
}