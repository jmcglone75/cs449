/*
 * "6 sided Die" minimal kernel module - /dev version
 *
 * Gerard McGlone <gjm36@pitt.edu>
 *
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>

#include <asm/uaccess.h>

  
/*
 * dice_read is the function called when a process calls read() on
 * /dev/dice.  It writes a random integer from 1-6 to the buffer passed in the
 * read() call.
 */

static ssize_t dice_read(struct file * file, char * buf, 
			  size_t count, loff_t *ppos)
{

	int num;
	get_random_bytes(&num, sizeof(int));
	
	if(num < 0)
		num = num*-1; //make the number positive if its negative
	 
	num = (num%6) + 1; //make num be in the range 1-6


	/*
	 * We only support reading the whole string at once.
	 */
	if (count < sizeof(int))
		return -EINVAL;
	/*
	 * If file position is non-zero, then assume the string has
	 * been read and indicate there is no more data to be read.
	 */
	if (*ppos != 0)
		return 0;
	/*
	 * Besides copying the string to the user provided buffer,
	 * this function also checks that the user has permission to
	 * write to the buffer, that it is mapped, etc.
	 */
	if (copy_to_user(buf, &num, sizeof(int)))
		return -EINVAL;
	/*
	 * Tell the user how much data we wrote.
	 */
	*ppos = sizeof(int);

	return sizeof(int);
}

/*
 * The only file operation we care about is read.
 */

static const struct file_operations hello_fops = {
	.owner		= THIS_MODULE,
	.read		= dice_read,
};

static struct miscdevice dice_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/dice.
	 */
	"dice",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&hello_fops
};

static int __init
dice_init(void)
{
	int ret;

	/*
	 * Create the "dice" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/dice device using
	 * the default rules.
	 */
	ret = misc_register(&dice_dev);
	if (ret)
		printk(KERN_ERR
		       "Unable to register \"Dice\" misc device\n");

	return ret;
}

module_init(dice_init);

static void __exit
dice_exit(void)
{
	misc_deregister(&dice_dev);
}

module_exit(dice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gerard McGlone <gjm36@pitt.edu>");
MODULE_DESCRIPTION("\"6 sided Die\" minimal module");
MODULE_VERSION("dev");
