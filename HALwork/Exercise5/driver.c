#include <linux/gpio.h> 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/sched.h>

#define GPIO 19
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Martin");
MODULE_DESCRIPTION("switch driver");


ssize_t my_char_driver_read(struct file *filp, char __user *buff,  size_t  count, loff_t *offp);
ssize_t my_char_driver_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
int my_char_driver_release(struct inode *inode, struct file *filep);
int my_char_driver_open(struct inode *inode, struct file *filep);
static void my_char_driver_exit(void);
 static int my_char_driver_init(void);
 static irqreturn_t mygpio_isr(int irq, void *dev_id);
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static long my_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
 //static char memory[250] = {0};
//static int size2;
 static int devno;
 static struct cdev min_cdev;
// static struct file_operations my_fops;
 static int short_irq;
 //static dev_t mindev;
 static int flag = 0;
 static wait_queue_head_t qw;
 
 static int isr_gpio_value;
 static int proc_gpio_value;
 
 static irqreturn_t mygpio_isr(int irq, void *dev_id)
 {
     //printk("IRQ event %i\n",irq);
     flag =1;
     isr_gpio_value = gpio_get_value(GPIO);
     
     wake_up_interruptible(&qw);

 return IRQ_HANDLED;
 }
 
 static struct file_operations my_fops = {
   .owner = THIS_MODULE,
   .read = my_char_driver_read,
   //.write = my_char_driver_write,
   .open = my_char_driver_open,
   .release = my_char_driver_release,
   .unlocked = my_ioctl,
   .compat_ioctl = my_compat_ioctl
 };
 
 static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
     return 0;
 }
 static long my_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
     return my_ioctl(file,cmd, (unsigned long)comdsdsqpat_ptr(arg));
 }
ssize_t my_char_driver_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos) {
    int res;
    wait_event_interruptible(qw,flag!=0);
    flag=0;
    //int value = gpio_get_value(19);
    proc_gpio_value = gpio_get_value(GPIO);
    char  buf[10];
    //sprintf(buf,"%i \n",proc_gpio_value);
    sprintf(buf,"%i %i \n",proc_gpio_value,isr_gpio_value); //skriver ud i rigtigt format
    int length = strlen(buf)+1;
    res = copy_to_user(buff,buf,length);
    if(res != 0) {
      return res;  
    }
     *f_pos += length;
     return length;
     
 }
 
 ssize_t my_char_driver_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
     printk("Write");
     return 0;
 }
 
 static int my_char_driver_init(void){
    init_waitqueue_head(&qw);
     int err = 0;
    gpio_request(GPIO,"gpio");
    if(err<0) goto request;
    gpio_direction_input(GPIO);
    devno = MKDEV(21,0);
    //alloc_chrdev_region(&devno,0,1,"mindev");
    //printk("mit Major %d. Minor %d\n",MAJOR(devno),MINOR(devno));
    short_irq = gpio_to_irq(GPIO);
    err = request_irq(short_irq,mygpio_isr,IRQF_TRIGGER_RISING,"gpio16",NULL);
    err = register_chrdev_region(devno,2,"fops");
    if(err<0) goto free_buf;
    printk("init");
    cdev_init(&min_cdev,&my_fops);
    err = cdev_add(&min_cdev, devno, 2);
    if(err<0) goto unregister;
    return 0;
    
    
    free_buf:
        gpio_free(GPIO);
    unregister:
        unregister_chrdev_region(devno,2);
    request:
        return err;
        //gpio_request();
    
 }

 static void my_char_driver_exit(void){
     
     cdev_del(&min_cdev);
     unregister_chrdev_region(devno,2);
     free_irq(short_irq,NULL);
     gpio_free(GPIO);
     printk("Hello");
     
 }
 
 int my_char_driver_open(struct inode *inode, struct file *filep)
 {
 int major, minor;
 major = MAJOR(inode->i_rdev);
 minor = MINOR(inode->i_rdev);
 printk("Opening MyGpio Device [major], [minor]: %i, %i\n", major, minor);
 
 return 0;
 }

 int my_char_driver_release(struct inode *inode, struct file *filep)
{

 int minor, major;

 major = MAJOR(inode->i_rdev);
 minor = MINOR(inode->i_rdev);
 printk("Closing/Releasing MyGpio Device [major], [minor]: %i, %i\n", major, minor);
return 0;
 }
module_init(my_char_driver_init);
module_exit(my_char_driver_exit);
