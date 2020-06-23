#include <linux/gpio.h> 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Martin");
MODULE_DESCRIPTION("switch driver");

ssize_t my_char_driver_read(struct file *filp, char __user *buff,  size_t  count, loff_t *offp);
ssize_t my_char_driver_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
int my_char_driver_release(struct inode *inode, struct file *filep);
int my_char_driver_open(struct inode *inode, struct file *filep);
static void my_char_driver_exit(void);
 static int my_char_driver_init(void);
//static char memory[250] = {0};
//static int size2;
 static int devno;
 static struct cdev min_cdev;
// static struct file_operations my_fops;
 
 //static dev_t mindev;
 
 static struct file_operations my_fops = {
   .owner = THIS_MODULE,
   .read = my_char_driver_read,
   //.write = my_char_driver_write,
   .open = my_char_driver_open,
   .release = my_char_driver_release,
 };
 
ssize_t my_char_driver_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos) {
    int res;
    int value = gpio_get_value(16);
    char  buf[3];
    sprintf(buf,"%i",value);
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
    int err = 0;
    gpio_request(16,"gpio16");
    if(err<0) goto request;
    gpio_direction_input(16);
    devno = MKDEV(21,0);
    //alloc_chrdev_region(&devno,0,1,"mindev");
    //printk("mit Major %d. Minor %d\n",MAJOR(devno),MINOR(devno));
    err = register_chrdev_region(devno,2,"fops");
    if(err<0) goto free_buf;
    printk("init");
    cdev_init(&min_cdev,&my_fops);
    err = cdev_add(&min_cdev, devno, 2);
    if(err<0) goto unregister;
    return 0;
    
    
    free_buf:
        gpio_free(16);
    unregister:
        unregister_chrdev_region(devno,2);
    request:
        return err;
        //gpio_request();
    
 }

 static void my_char_driver_exit(void){
     
     cdev_del(&min_cdev);
     unregister_chrdev_region(devno,2);
     gpio_free(16);
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
