/**
 * simple.c    // DO NOT RENAME THIS FILE or the makefile will fail
 *
 * A simple kernel module. 
 * 
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */
#define _GNU_SOURCE

//#include <stdio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kernel.h>


typedef struct list_head list_head;
typedef struct date date;
struct date {
       int day;
       int month;
       int year;
       const char* name;
};
struct date birthdays[] = { 
			{ 25, 8, 1930, "Sean Connery" },
			{ 27, 1, 1756, "Wolfgang Amadeus Mozart" },
			{ 23, 4, 1564, "William Shakespeare" },
			{ 6, 3, 1475, "Michelangelo" },
			{ 28, 9, -551, "Confucius" },
};

typedef struct birthday birthday;
struct birthday { 
       int day;
       int month;
       int year;               // avoid putting name in birthday struct
       struct list_head list;
};

// list_head birthday_list;
static LIST_HEAD(birthday_list);

birthday* birthday_create(date aDay) { 
       birthday *p =kmalloc(sizeof(*p), GFP_KERNEL);
       printk(KERN_INFO, "Creating birthday for %s\n", aDay.name);
       p->day = aDay.day; //assigning the p's values to day's values
       p->month = aDay.month;
       p->year = aDay.year;
       INIT_LIST_HEAD(&p->list); 
       return p;
}

void birthday_print(birthday* p) {
              printk(KERN_INFO "birthday is: %d/%d/%d\n", p->day, p->month, p->year);
}

void birthday_list_create(void) { 
       birthday *p; 
       int length =sizeof(birthdays) /sizeof(date);
       int i=0; 
       for (i=0; i<length; ++i){
              p=birthday_create(birthdays[i]);
              printk(KERN_INFO "name is: %s", birthdays[i].name);
              list_add_tail(&p->list, &birthday_list);
       }
}      

void birthday_list_print(void) { 
       birthday *p; 
       list_for_each_entry(p, &birthday_list, list) {
              birthday_print(p);
       }
}

void birthday_list_delete(void) {
       birthday *p;
       birthday *next;
       list_for_each_entry_safe(p, next, &birthday_list, list){
              list_del(&p->list);
              kfree(p);
       }
       
}

/* This function is called when the module is loaded. */
int simple_init(void) {
       printk(KERN_INFO "Loading Module\n\n");
       printk(KERN_INFO "I can program simple kernel code\n");

       printk(KERN_INFO "Creating birthday list...\n");
       birthday_list_create();
       printk(KERN_INFO "birthday list created!\n\n");

       birthday_list_print();
       printk(KERN_INFO "\n");
       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
       printk(KERN_INFO "Deleting birthday list...\n");
       birthday_list_delete();
       printk(KERN_INFO "birthday list deleted!\n");
	printk(KERN_INFO "\nRemoving Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");


//===================================================================
// OUTPUT
//===================================================================

//osc@ubuntu:~/final-src-osc10e/ch2$ sudo rmmod simple.ko
//osc@ubuntu:~/final-src-osc10e/ch2$ sudo dmesg 
//[14562.454671] Loading Module

//[14562.454676] I can program simple kernel code
//[14562.454677] Creating birthday list...
//[14562.454680] name is: Sean Connery

//[14562.454683] name is: Wolfgang Amadeus Mozart

//[14562.454685] name is: William Shakespeare

//[14562.454688] name is: Michelangelo

//[14562.454690] name is: Confucius
//[14562.454691] birthday list created!

//[14562.454694] birthday is: 25/8/1930
//[14562.454695] birthday is: 27/1/1756
//[14562.454697] birthday is: 23/4/1564
//[14562.454699] birthday is: 6/3/1475
//[14562.454700] birthday is: 28/9/-551

//[14569.053953] Deleting birthday list...
//[14569.053957] birthday list deleted!

//                Removing Module
//===================================================================
