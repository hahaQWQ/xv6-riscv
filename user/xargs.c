#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"  // For MAXARG


int get_cmd(char*buf, int sz)
{
    memset(buf,0,sz);
    int i, cc;
    char c;

    for(i=0; i+1 < sz; ){
        cc = read(0, &c, 1);
        if(cc < 1)
            break;
        buf[i++] = c;
        if(c == '\n' || c == '\r')
            break;
    }
    // fix last char bug
    buf[--i] = '\0';
    if(buf[0]=='\0') return 0;
    return 1;
}

void test_exec(char* e,char** args)
{
    printf("execute : '%s'\n",e);
    for(int i=0;i<MAXARG;i++)
    {
        if(args[i]==0)
        {
            break;
        }
        printf("args[%d] : %s\n",i,args[i]);
    }
    exec(e,args);
}

int main(int argc,char** argv)
{
    int executable_idx = 1;
    int num_of_per_cmd = 1;
    int i = 0;
    int j = 0;
    char buffer[512];
    char* p;
    char* s;
    char *executable_argv[MAXARG];
    int num_flag = 0;
    
    if(argc < 2)
    {
        fprintf(2,"usage: xargs [-n:int] cmd ...args\n");
        exit(1);
    }
    //      0        1   2    3
    // {xargs path, -n, num, cmd}
    if(strcmp(argv[1],"-n")==0 && argc>2)
    {
        num_of_per_cmd = atoi(argv[2]);
        executable_idx = 3;
        num_flag = 1;
    }
    for (i = executable_idx; i < argc; i++)
    {
        executable_argv[i-executable_idx] = argv[i];
    }
    
    while (get_cmd(buffer,sizeof(buffer)))
    {
        p = buffer;
        s = buffer;
        if(num_flag)
        {
            for(j=0;j<num_of_per_cmd;j++)
            {
                while (*p!=' ' && *p!='\0') p++;
                *p= '\0';
                executable_argv[j+argc-executable_idx] = s;
                s = ++p;
            }
            executable_argv[num_of_per_cmd+argc-executable_idx] = 0;
        }else{
            executable_argv[argc-executable_idx]=buffer;
        }
        
        if(fork()==0)
        {
            exec(executable_argv[0],executable_argv);
            exit(0);
        }else{
            wait(0);
        }
    }
    

}
