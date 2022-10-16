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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <asm/param.h>
#include <linux/jiffies.h>

typedef struct list_head list_head;
typedef struct date date;
struct date {
       int day;
       int month;
       int year;
       const char* name;
};
struct date birthdays[] = { // initating the list
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
       INIT_LIST_HEAD(&p->list); //pointing to the head of the list
       return p;
}

void birthday_print(birthday* p) {
              printk(KERN_INFO "birthday is: %d/%d/%d\n", p->day, p->month, p->year); //printing the birthdays#
}

void birthday_list_create(void) { 
       birthday *p;  //making a pointer
       int length =sizeof(birthdays) /sizeof(date); //the length of the list
       int i=0; //start of the list 
       for (i=0; i<length; ++i){ //going through the list 
              p=birthday_create(birthdays[i]); //making a pointer and putting it in the list
              printk(KERN_INFO "name is: %s", birthdays[i].name); //printing the birthday info 
              list_add_tail(&p->list, &birthday_list); // adding to the next list 
       }
}      

void birthday_list_print(void) { 
       birthday *p; //making a pointer
       list_for_each_entry(p, &birthday_list, list) {
              birthday_print(p); // printing the bday ##
       }
}

void birthday_list_delete(void) {
       birthday *p;
       birthday *next;
       list_for_each_entry_safe(p, next, &birthday_list, list){ 
              list_del(&p->list); //deleting the p from list
              kfree(p); // freeing the p 
       }
       
}

unsigned long jiffies_init=0;
unsigned long jiffies_exit=0;

/* This function is called when the module is loaded. */
int simple_init(void) {
       printk(KERN_INFO "Loading Module\n\n");
       printk(KERN_INFO "I can program simple kernel code\n");

      // printk(KERN_INFO "Creating birthday list...\n");
       //birthday_list_create();
      // printk(KERN_INFO "birthday list created!\n\n");
     //  birthday_list_print();

       printk(KERN_INFO "%lu\n\n", GOLDEN_RATIO_PRIME); 

       jiffies_init=jiffies;
       printk(KERN_INFO "in simple_inti, HZ is: %d\n", HZ);
       printk(KERN_INFO "in simple_inti, jiffies is: %lu\n", jiffies);
       printk(KERN_INFO "\n");
       
       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
   //    printk(KERN_INFO "Deleting birthday list...\n");
   //   birthday_list_delete();
   //    printk(KERN_INFO "birthday list deleted!\n");

    printk(KERN_INFO "gcd of 3300 and 24 is: %lu\n\n", gcd(3300, 24));
    jiffies_exit=jiffies;
    printk(KERN_INFO "in simple_exit, jiffies is: %lu\n", jiffies);
    printk(KERN_INFO "in simple_exit, delta jiffies is: %lu\n", jiffies_exit - jiffies_init);

	printk(KERN_INFO "\nRemoving Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");