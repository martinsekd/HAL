#include <linux/gpio.h> 
#include <linux/of_gpio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Martin");
MODULE_DESCRIPTION("LED driver");

ssize_t my_char_driver_read(struct file *filp, char __user *buff,  size_t  count, loff_t *offp);
ssize_t my_char_driver_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
int my_char_driver_release(struct inode *inode, struct file *filep);
int my_char_driver_open(struct inode *inode, struct file *filep);
static const struct of_device_id of_plat_drv_platform_device_match[];


//int probe();
//int remove();
static void my_char_driver_exit(void);
 static int my_char_driver_init(void);
//static char memory[250] = {0};
//static int size2;
 static int devno;
 static struct cdev min_cdev;
// static struct file_operations my_fops;
 static struct class * my_plat;
 static struct device * my_device;
 static struct platform_driver plat_drv_platform_driver;
 //static dev_t mindev;
 int my_minor = 1;

 static struct file_operations my_fops = {
   .owner = THIS_MODULE,
   .read = my_char_driver_read,
   .write = my_char_driver_write,
   .open = my_char_driver_open,
   .release = my_char_driver_release,
   //.probe = probe,
   //.remove = remove
 };
 
 struct gpio_dev {
     int no;
     int dir;
 };
 static struct gpio_dev gpio_devs[2] = {{12,0}, {21,1}};
 static int gpios_len = 2;
 
ssize_t my_char_driver_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos) {
    int res;
    int minor2;
    minor2 = iminor(filp->f_inode);
    int value = gpio_get_value(gpio_devs[minor2].no);
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
 const int useMinor = 0;

 static int plat_drv_probe(struct platform_device *pdev) {
    int nbr = 21;
    int err;
    int len = 0;
    
    len = of_gpio_count(pdev->dev.of_node);
    if(len<0) {
        printk("Fail to read\n");
    } else {
        printk("Antal: %d\n",len);
    }
    
    printk("Ny device: %s\n",pdev->name);
    
    struct gpio_dev gpio_devs2[len];
    
    for(int i=0;i<len;i++) {
        enum of_gpio_flags flags;
        int gpionummer = of_get_gpio(pdev->dev.of_node,i);
        int direction = of_get_gpio_flags(pdev->dev.of_node,i,&flags);
        direction = (int) (flags);
        printk("gpio %d og dir %d\n",gpionummer,direction);
        gpio_devs2[i].no = gpionummer;
        gpio_devs2[i].dir = direction;
    }
    
    struct device *my_device;
     printk("ny plat device: %s\n",pdev->name);
     for(int i=0; i<gpios_len; i++) {
       char bufprobe[10];
       sprintf(bufprobe,"gpio%d",gpio_devs[i].no);
         err = gpio_request(gpio_devs[i].no,bufprobe);
         if(err<0) goto request;
         if(gpio_devs[i].dir==1) {
             printk("retning: %d\n",gpio_devs[i].dir);
         gpio_direction_output(gpio_devs[i].no,1);
         } else {
         gpio_direction_input(gpio_devs[i].no);
         }
         nbr = gpio_devs[i].no;
         my_device = device_create(my_plat, NULL, MKDEV(MAJOR(devno),i), NULL, "dev%d",nbr);
     }
     
   
     if(IS_ERR(my_device))
         return -EINVAL;
    else  
         return 0;
    
    request:
        return err;
    
}
static int plat_drv_remove(struct platform_device *pdev) {
    printk("Device fjernet\n");
    gpio_free(12);
    gpio_free(21);
    device_destroy(my_plat,MKDEV(MAJOR(devno), 0));
    device_destroy(my_plat,MKDEV(MAJOR(devno), 1));
    return 0;
    
}

static const struct of_device_id of_plat_drv_platform_device_match[] = {
    { .compatible = "ase, plat_drv", }, {},
};

static struct platform_driver plat_drv_platform_driver = {
    .probe = plat_drv_probe,
    .remove = plat_drv_remove,
    .driver = {
        .name = "plat_drv",
            .of_match_table = of_plat_drv_platform_device_match,
            .owner = THIS_MODULE,
    },
};

 ssize_t my_char_driver_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
     printk("Write");
     int res;
     char buf[2];
     res = copy_from_user(buf,buff,count);
     if(res<0) return res;
     int value;
        int minor2;
    minor2 = iminor(filp->f_inode);
    
     sscanf(buf,"%d",&value);
     if(value==1) {
         gpio_set_value(21,1);
         printk("1\n");
         printk("%d,\n",gpio_devs[minor2].no);
     } else {
         gpio_set_value(21,0);
         printk("0\n");
     }
     return count;
 }
 
 
 static int my_char_driver_init(void){
    int err = 0;
    err = gpio_request(21,"gpio20");
    if(err<0) goto request;
    gpio_direction_output(21,1);

    //devno = MKDEV(22,0);
    //alloc_chrdev_region(&devno,0,1,"mindev");
    //printk("mit Major %d. Minor %d\n",MAJOR(devno),MINOR(devno));
    err = alloc_chrdev_region(&devno,0,2,"LED");
    if(err<0) goto free_buf;
    my_plat = class_create(THIS_MODULE, "LED_class2");
    
    printk("OK\n");
    //my_device = device_create(my_plat,NULL,MKDEV(MAJOR(devno),my_minor),"LED%d",101);
    printk("OK\n");
    printk("init\n");
    cdev_init(&min_cdev,&my_fops);
    err = cdev_add(&min_cdev, devno, 2);
    if(err<0) goto unregister;
    printk("cdev added\n");
    err = platform_driver_register(&plat_drv_platform_driver);
    return 0;
    
    
    free_buf:
        gpio_free(21);
    unregister:
        unregister_chrdev_region(devno,2);
    request:
        return err;
        //gpio_request();
    
 }

 static void my_char_driver_exit(void){
     
     
     gpio_free(12);
     gpio_free(21);
     unregister_chrdev_region(devno,2);
     class_destroy(my_plat);
     device_destroy(my_plat,MKDEV(MAJOR(devno), 0));
    device_destroy(my_plat,MKDEV(MAJOR(devno), 1));
     cdev_del(&min_cdev);
     platform_driver_unregister(&plat_drv_platform_driver);
     
     
     printk("Goodbye Yo\n");
     
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
