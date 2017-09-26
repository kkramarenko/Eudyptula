/*
* This Linux kernel module of usb keyboard, which loads
* automaticaly when keyboard plugged and removes when
* keyboard unplugged.
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/kernel.h>
#include <linux/hid.h>

static int kbd_probe(struct usb_interface *interface,
			const struct usb_device_id *id)
{
	pr_info("TASK 5 -> Keyboard plugged\n");
	return 0;
}

static void kbd_disconnect(struct usb_interface *interface)
{
	pr_info("TASK 5 -> Keyboard unplugged\n");
}

static struct usb_device_id kbd_table[] = {
	{USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
	 USB_INTERFACE_SUBCLASS_BOOT,
	 USB_INTERFACE_PROTOCOL_KEYBOARD)},
	{}
};
MODULE_DEVICE_TABLE(usb, kbd_table);

static struct usb_driver kbd_driver = {
	.name = "task5_drv",
	.id_table = kbd_table,
	.probe = kbd_probe,
	.disconnect = kbd_disconnect
};

static int __init task5_init(void)
{
	int res = 0;

	pr_info("TASK 5 -> Keyboard driver load\n");
	res = usb_register(&kbd_driver);
	if (res)
		pr_err("TASK 5 -> Register driver failed with error %d\n", res);

	return res;
}

static void __exit task5_exit(void)
{
	pr_info("TASK 5 -> Keyboard driver unload\n");
	usb_deregister(&kbd_driver);
}

module_init(task5_init);
module_exit(task5_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple linux kernel module for usb keyboard");
MODULE_AUTHOR("Konstantin Kramarenko <kostya.kram@gmail.com>");
