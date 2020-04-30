#define _GNU_SOURCE
#include "my_process.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define get_time 334
#define printk 335

int process_assign_CPU(int pid, int core){
    if(core > sizeof(cpu_set_t)){
        fprintf(stderr,"core index err\n");
        return -1;
    }
    cpu_set_t m;
    CPU_ZERO(&m);
    CPU_SET(core,&m);
    if(sched_setaffinity(pid,sizeof(m),&m)<0){
        perror("set_affinity\n");
        exit(1);
    }
    return 0;
}

int process_execute(struct process proc){
    int pid = fork();
    if(pid < 0){
        perror("fork\n");
        return -1;
    }
    if(pid == 0){
        struct timespec start_t,end_t;
        char dmsg[256];
        syscall(get_time,&start_t);
        for(int i = 0;i<proc.exec;i++){
            _unit_();
        }
        syscall(get_time,&end_t);
        sprintf(dmsg,"[Project1] %d %lu,%09lu %lu.%09lu\n",getpid(),start_t.tv_sec,start_t.tv_nsec,end_t.tv_sec,end_t.tv_nsec);
        syscall(printk,dmsg);
        exit(0);
    }
    process_assign_CPU(pid,child_CPU);
    return pid;
}

int process_block(int pid){
    struct sched_param par;
    par.sched_priority = 0;
    int ret = sched_setscheduler(pid,SCHED_IDLE,&par);
    if(ret < 0){
        perror("sched_setscheduler\n");
        return -1;
    }
    return ret;
}

int process_wakeup(int pid){
    struct sched_param par;
    par.sched_priority = 0;
    int ret = sched_setscheduler(pid,SCHED_OTHER,&par);
    if(ret < 0){
        perror("sched_setscheduler\n");
        return -1;
    }
    return ret;
}