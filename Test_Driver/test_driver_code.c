/* link for example : https://olegkutkov.me/2018/03/14/simple-linux-character-device-driver/ */
/* this example ilustrates every aspect of device driver */


#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/ioctl.h>

MODULE_LICENSE("GPL");


#define test_Major 45
#define test_Minor 100

#define CHRDEV_FIRST 0
#define CHRDEV_COUNT 150
#define BUFFER_LEN   80

static char msg[BUFFER_LEN] = {0};
static char *msg_ptr = &msg[0];

static int test_driver_code_init(void);
static void test_driver_code_exit(void);

static ssize_t test_read(struct file *test_read_p , char __user *test_user_p,size_t test_size , loff_t *test_offset_p);
//static size_t test_write(struct file *test_write , const char __user *test_user, loff_t *test_offset);
static int test_release(struct inode *test_inode_p,struct file *test_file_p);
static int test_open(struct inode *test_inode_p,struct file *test_file_p);

static ssize_t test_write (struct file *test_write_p, const char __user *test_user_p, size_t test_size, loff_t *test_offset_p);


struct test_device_data
{
    struct cdev cdev;
    size_t size;
};

const struct file_operations f_o =
{
    .owner   = THIS_MODULE,
    .open    = test_open,
    .release = test_release,
    .read    = test_read,
    .write   = test_write
//    .ioctl   = test_ioctl
};

const char *name = "test_char_driver";

static int test_driver_code_init(void)
{
    int ret;
    printk(KERN_INFO "Test module for char Driver\n");
    printk(KERN_INFO "I will stop when i succeed\n");

    ret =register_chrdev_region(CHRDEV_FIRST,CHRDEV_COUNT,name);
    if(ret < 0)
    {
        printk(KERN_INFO "ERROR in register_chrdev_region\n");
    }
    printk(KERN_INFO "device registered succeed\n");
    
}

static void test_driver_code_exit(void)
{
    printk(KERN_INFO "Exiting Test char module\n");
    unregister_chrdev_region(CHRDEV_FIRST,CHRDEV_COUNT);
}

module_init(test_driver_code_init);
module_exit(test_driver_code_exit);
// need to study input parameters which is to be passed 

static int test_open(struct inode *test_inode_p,struct file *test_file_p)
{
    struct test_device_data *my_data;

    my_data = container_of(test_inode_p->i_cdev,struct test_device_data,cdev);
    test_file_p->private_data = my_data; 
}

static ssize_t test_read(struct file *test_read_p , char __user *test_user_p,size_t test_size , loff_t *test_offset_p)
{
    struct test_device_data *my_data = (struct test_device_data*)test_read_p->private_data;
 //   my_data->size = 40 ;

//    size_t len = min(my_data->size - *test_offset,test_size);
        
//    if(len <= 0)
//        return 0;

    if(copy_to_user(test_user_p,msg_ptr+*test_offset_p , test_size))
        return -EFAULT;

//    *test_offset += len;

//    return len;
    
    /* return type of below function is unsigned long */
    //copy_to_user(void __user *to, const void *from, unsigned long n)

}

static ssize_t test_write (struct file *test_write_p, const char __user *test_user_p, size_t test_size, loff_t *test_offset_p)
//static size_t test_write(struct file *test_write , const char __user *test_user, loff_t *test_offset)
{
    struct test_device_data *my_data = (struct test_device_data *)test_write_p->private_data;
  //  my_data->size = 40 ;

  //  size_t len = min(my_data->size - *test_offset ,test_size);

  //  if(len <= 0 )
  //      return 0;

    if(copy_from_user(msg_ptr,test_user_p,test_size))
       return -EFAULT;

  //  *test_offset += len;

  //  return len;  

    /* return type of below function is unsigned long */
    //copy_from_user(void *to, const void __user *from, unsigned long n)

}

static int test_release(struct inode *test_inode_p,struct file *test_file_p)
{
    return 0 ;
}
