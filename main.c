#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "my_process.h"
#include "my_scheduler.h"

#define FIFO 1
#define RR 2
#define SJF 3
#define PSJF 4
#define GET_TIME 314
#define PRINTK 315

int cmp(const void *a, const void *b){
    return ((struct process *)a)->ready - ((struct process *)b)->ready;
}

int main(int argc, char* argv[]){
    char sched_name[64];
    int name,proc_num;
    struct process *proc;
    scanf("%s%d",sched_name,&proc_num);
    proc = (struct process *)malloc(proc_num*sizeof(struct process));
    for(int i = 0;i<proc_num;i++){
        scanf("%s%d%d",proc[i].name,&proc[i].t_ready,&proc[i].t_exec);
    }
    if(!strcmp(sched_name,"FIFO")){
        name = FIFO;
    }else if(!strcmp(sched_name,"RR")){
        name = RR;
    }else if(!strcmp(sched_name,"SJF")){
        name = SJF;
    }else if(!strcmp(sched_name,"PSJF")){
        name = PSJF;
    }
    qsort(proc,proc_num,sizeof(struct process),cmp);
    scheduling(proc,proc_num,name);
    exit(0);
}