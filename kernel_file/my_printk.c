#include <linux/linkage.h>
#include <linux/kernel.h>

#define mod 1000000000

asmlinkage void sys_my_printk(int pid, long start_t, long end_t){
	printk("[Project1] %d %ld.%09ld %ld.%09ld\n",pid,start_t/mod,start_t%mod,end_t/mod,end_t%mod);
}
