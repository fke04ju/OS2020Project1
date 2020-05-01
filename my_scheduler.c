#define _GNU_SOURCE
#include "my_process.h"
#include "my_scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <sys/wait.h>

static int context_switch_time,unit_time,running_process,end_proc_num;

int next_proc_id(struct process *proc, int proc_num, int name){
    if(running_process != -1 && (name == FIFO || name == SJF)){
        return running_process;
    }
    int ret = -1;
    if(name == SJF || name == PSJF){
        for(int i = 0;i<proc_num;i++){
            if(proc[i].pid == -1 || proc[i].exec == 0){
                continue;
            }
            if(ret == -1 || proc[i].exec < proc[ret].exec){
                ret = i;
            }
        }
    }else if(name == FIFO){
        for(int i = 0;i<proc_num;i++){
            if(proc[i].pid == -1 || proc[i].exec == 0){
                continue;
            }
            if(ret == -1 || proc[i].ready < proc[ret].ready){
                ret = i;
                break;
            }
        }
    }else if(name == RR){
        if(running_process == -1){
            for(int i = 0;i<proc_num;i++){
                if(proc[i].pid != -1 && proc[i].exec > 0){
                    ret = i;
                    break;
                }
            }
        }else if((unit_time-context_switch_time)%500 == 0){
            ret = (running_process+1)%proc_num;
            while(proc[ret].pid == -1 || proc[ret].exec == 0){
                ret = (ret+1)%proc_num;
            }
        }else{
            ret = running_process;
        }
    }
    return ret;
}

int scheduling(struct process *proc, int proc_num, int name){
    for(int i = 0;i<proc_num;i++){
        proc[i].pid = -1;
    }
    process_assign_CPU(getpid(),parent_CPU);
    process_wakeup(getpid());
    unit_time = 0;
    running_process = -1;
    end_proc_num = 0;
    while(1){
        if(running_process != -1 && proc[running_process].exec == 0){
            waitpid(proc[running_process].pid, NULL, WUNTRACED);
            printf("%s %d\n", proc[running_process].name, proc[running_process].pid);
            running_process = -1;
            end_proc_num ++;
            if(end_proc_num == proc_num){
                break;
            }
        }
        for(int i = 0;i<proc_num;i++){
            if(proc[i].ready == unit_time){
                proc[i].pid = process_execute(proc[i]);
                process_block(proc[i].pid);
            }
        }
        int next_process = next_proc_id(proc,proc_num,name);
        if(next_process != -1){
            if(running_process != next_process){
                process_wakeup(proc[next_process].pid);
                process_block(proc[running_process].pid);
                running_process = next_process;
                context_switch_time = unit_time;
            }
        }
        _unit_();
        if(running_process != -1){
            proc[running_process].exec--;
        }
        unit_time++;
        //printf("time : %d , running : %d\n",unit_time,proc[running_process].pid);
    }
    return 0;
}
