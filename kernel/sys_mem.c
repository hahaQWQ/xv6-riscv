#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64 sys_mem()
{
    // how to calc the amount of free memory available?
    struct proc *p = myproc();
    
    
    return p->sz;
}