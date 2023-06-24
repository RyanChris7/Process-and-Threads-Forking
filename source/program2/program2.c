#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/jiffies.h>
#include <linux/kmod.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");

static struct task_struct *task;
struct wait_opts {
  enum pid_type wo_type;
  int wo_flags;
  struct pid *wo_pid;
  struct siginfo __user *wo_info;
  int __user wo_stat;
  struct rusage __user *wo_rusage;
  wait_queue_entry_t child_wait;
  int notask_error;
};

// External Function

extern pid_t kernel_clone(struct kernel_clone_args *args);
extern pid_t kernel_thread(int (*fn)(void *), void *arg, unsigned long flags);
extern int do_execve(struct filename *filename,
                     const char __user *const __user *__argv,
                     const char __user *const __user *__envp);
										 extern long do_wait(struct wait_opts *wo);
extern long do_wait(struct wait_opts *wo);
extern struct filename *getname_kernel(const char *filename);

// Exec Process 
int exec_process(void){
	int res;
	const char file_location[] = "/home/vagrant/csc3150/Assignment1_119010507/source/program1/trap";
	struct filename *exec_file = getname_kernel(file_location);
	res = do_execve(exec_file, NULL, NULL);
	if(!res){
		return 0;
	} else{
		do_exit(res);
	}
}

// Wait Process
void wait_process(pid_t pid) {
	int status;
	struct wait_opts wo;
  struct pid *wo_pid = NULL;
  enum pid_type type;
  type = PIDTYPE_PID;
  wo_pid = find_get_pid(pid);
  wo.wo_type = type;
  wo.wo_pid = wo_pid;
  wo.wo_flags = WEXITED | WUNTRACED;
  wo.wo_info = NULL;
  wo.wo_stat = status;
  wo.wo_rusage = NULL;

	printk("[program2] : child process");
	/* Wait for child process */
  int n;
  n = do_wait(&wo);

  /* Process exit signals or termination status */
  int exit_signal;
	if(wo.wo_stat > 128){
		exit_signal = wo.wo_stat - 128;
	}else{
		exit_signal = wo.wo_stat;
	}

	if (wo.wo_stat == 0) {
    printk("[program2] : normal termination\n");
    printk("[program2] : The return signal is %d\n", wo.wo_stat);

  } else if ((((wo.wo_stat & 0xff00) >> 8) == 19)) {
    printk("[program2] : get SIGSTOP signal\n");
    printk("[program2] : The return signal is %d\n", 19);

  } else {
		if(exit_signal == 1){
			printk("[program2] : get SIGHUP signal\n");
		}	
		else if(exit_signal == 2){
			printk("[program2] : get SIGINT signal\n"); 
		}	
		else if(exit_signal == 3){
			printk("[program2] : get SIGQUIT signal\n"); 
		}			
		else if(exit_signal == 4){
			printk("[program2] : get SIGILL signal\n"); 
		}			
		else if(exit_signal == 5){
			printk("[program2] : get SIGTRAP signal\n"); 
		}
		else if(exit_signal == 6){
			printk("[program2] : get SIGABRT signal\n"); 
		}		
		else if(exit_signal == 7){
			printk("[program2] : get SIGBUS signal\n"); 
		}		
		else if(exit_signal == 8){
			printk("[program2] : get SIGFPE signal\n"); 
		}		
		else if(exit_signal == 9){
			printk("[program2] : get SIGKILL signal\n"); 
		}		
		else if(exit_signal == 11){
			printk("[program2] : get SIGSEGV signal\n"); 
		}		
		else if(exit_signal == 13){
			printk("[program2] : get SIGPIPE signal\n"); 
		}
		else if(exit_signal == 14){
			printk("[program2] : get SIGALRM signal\n"); 
		}
		else if(exit_signal == 15){
			printk("[program2] : get SIGTERM signal\n"); 
		}
		printk("[program2] : The return signal is %d\n", exit_signal);
	}
	put_pid(wo_pid);

	return;
}

//implement fork function
int my_fork(void *argc){
	
	//set default sigaction for current process
	int i;
	struct k_sigaction *k_action = &current->sighand->action[0];
	for(i=0;i<_NSIG;i++){
		k_action->sa.sa_handler = SIG_DFL;
		k_action->sa.sa_flags = 0;
		k_action->sa.sa_restorer = NULL;
		sigemptyset(&k_action->sa.sa_mask);
		k_action++;
	}
	
	// Fork Process using Kernel Clone
	  struct kernel_clone_args args = {
      .flags = SIGCHLD,   
			.parent_tid = NULL,
      .child_tid = NULL,
      .exit_signal = SIGCHLD,
      .stack = (unsigned long)&exec_process,
      .stack_size = 0,
      .tls = 0,
  };

	pid_t child_pid;
  child_pid = kernel_clone(&args);
	
	/* execute a test program in child process */
	  if (child_pid == 0) {
    exec_process();

  } else {
    /* wait until child process terminates */
    printk("[program2] : The child process has pid = %d\n", child_pid);
    printk("[program2] : This is the parent process, pid = %d\n", current->pid);

    wait_process(child_pid);
  }

  return 0;
}

static int __init program2_init(void){

	printk("[program2] : Module_init \n");
	printk("[program2] : Module_init create kthread start\n");


  /* create a kernel thread to run my_fork */

  task = kthread_run(&my_fork, NULL, "MyThread");
	
	return 0;
}

static void __exit program2_exit(void){
	printk("[program2] : Module_exit\n");
}

module_init(program2_init);
module_exit(program2_exit);
