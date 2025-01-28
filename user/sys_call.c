#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user.h"


int main()
{
    int pid = freemem();
    printf("%d\n",pid);
}