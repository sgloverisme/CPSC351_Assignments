/**
 * simple.c
 *
 * A simple kernel module. 
 * 
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */
#define _GNU_SOURCE

#include <linux/init.h> 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/proc_fs.h> 
#include <asm/uaccess.h>
#include <asm/param.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

typedef struct file file;

unsigned long jiffies_loaded = 0;
unsigned long jiffies_now = 0;
unsigned long delta_jiffies=0; //getting delta jiffies
unsigned long seconds_total=0; // caluating seconds

ssize_t proc_read(file* file, char __user* usr_buf, 
                size_t count, loff_t* pos);

static struct file_operations proc_ops = {
       .owner = THIS_MODULE,
       .read = proc_read,
};

// called when the module is loaded
int proc_init(void) {
       printk(KERN_INFO "Loading proc\n");

       // TODO: fill in missing code for proc_init
       //       use code from Silberschatz 10th ed. The /proc file system 
       jiffies_loaded=jiffies; //setting the current jiff
       proc_create(PROC_NAME, 0666, NULL, &proc_ops); //creating seconds
       
       return 0;
}

// called when the module is removed
void proc_exit(void) {
       
       printk(KERN_INFO "Removing proc\n\n"); //removing

       remove_proc_entry(PROC_NAME, NULL);
}

// this function is called each time /proc/hello is called
ssize_t proc_read(file* file, char __user* usr_buf, 
                  size_t count, loff_t* pos) {
       int rv = 0;
       char buffer[BUFFER_SIZE];
       static int completed = 0;

       if (completed) {
              completed = 0;
              return 0;
       }               
       completed = 1;

       jiffies_now=jiffies; //setting the new jiffies
       delta_jiffies=jiffies_now-jiffies_loaded; //getting delta jiffies
       seconds_total=delta_jiffies/HZ; // caluating seconds

       printk(KERN_INFO "proc_read called by user\n");
       // TODO: fill in missing code to print seconds since cat /proc/seconds was called
       //       use code from Silberschatz 10th ed. The /proc file system
       rv = sprintf(buffer, "the amount of seconds elasped is: %lu\n", seconds_total);

       
       /* copies kernel space buffer to user space usr buf */ 
       copy_to_user(usr_buf, buffer, rv);
       return rv;
}

/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Proc Module");
MODULE_AUTHOR("SGG");

//=============================================================
// OUTPUT 
//=============================================================
// PUT YOUR OUTPUT HERE
/*
osc@ubuntu:~/final-src-osc10e/ch2$ sudo insmod simple.ko
osc@ubuntu:~/final-src-osc10e/ch2$ cat /proc/seconds
the amount of seconds elasped is: 5
osc@ubuntu:~/final-src-osc10e/ch2$ cat /proc/seconds
the amount of seconds elasped is: 52
osc@ubuntu:~/final-src-osc10e/ch2$ cat /proc/seconds
the amount of seconds elasped is: 63
*/
//=============================================================
