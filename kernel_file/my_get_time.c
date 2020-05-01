#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

#define mod 1000000000

asmlinkage long sys_my_get_time(void){
	struct timespec tmp;
	getnstimeofday(&tmp);
	return tmp.tv_sec*mod+tmp.tv_nsec;
}
