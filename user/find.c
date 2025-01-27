#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"




int substr(char* str1,char* str2)
{
    if(*str2=='\0'){
        return 1;
    }
    while (*str1)
    {
        char* p1 = str1;
        char* p2 = str2;
        while (*p1 && *p2 && *p1==*p2)
        {
            p1++;
            p2++;   
        }
        if(*p2=='\0'){
            return 1;
        }
        str1++;
    }
    
    return 0;
    
}

// if the path is file that check the pattern 
void find(char* path,char* pattern)
{
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512];
    if(substr(path,pattern))
    {
        printf("%s\n",path);
    }
    
    if((fd=open(path,O_RDONLY))<0)
    {
        printf("falied to open \"%s\"\n",path);
        return;
    }
    if(fstat(fd,&st)<0){
        close(fd);
        printf("failed to get stat info of \"%s\"\n",path);
        return;
    }
    
    if(st.type==T_DIR)
    {
        if(strlen(path)+DIRSIZ+2>sizeof(buf))
        {
            printf("path buffer overflow.\n");
            exit(0);
        }
        strcpy(buf,path);
        char* p = buf+strlen(path);
        *p++ = '/';
        while (read(fd,&de,sizeof(de))>0)
        {
            if(de.inum==0 || de.name[0]=='.' || (de.name[0]=='.' && de.name[1]=='.'))
            {
                continue;
            }
            memcpy(p,de.name,DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf,pattern);
        }
        
    }
    // important! because the vx6 only have 16 file
    close(fd);

    
}

int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        printf("usage: find xxx\n");
    }
    find(argv[1],argv[2]);


    exit(0);
}