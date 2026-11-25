#include <linux/init.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
struct color { int red; int blue; int green; struct list_head list; };
static LIST_HEAD(color_list);
/* Shared by the init error path and module exit: unlink and kfree every node; returns how many were freed. */
static unsigned int color_list_free_all(void)
{
    struct color *entry, *next;
    unsigned int freed = 0;
    list_for_each_entry_safe(entry, next, &color_list, list) {
        list_del(&entry->list);
        kfree(entry);
        ++freed;
    }
    return freed;
}
static int __init color_list_init(void)
{
    /* Correct violet RGB; the book interchanges green/blue field order. */
    static const int rgb[4][3] = {{138, 43, 226}, {255, 0, 0}, {0, 255, 0}, {0, 0, 255}};
    struct color *entry;
    int i;
    for (i = 0; i < 4; ++i) {
        entry = kmalloc(sizeof(*entry), GFP_KERNEL);
        if (!entry) {
            color_list_free_all();
            printk(KERN_ERR "color: kmalloc failed\n");
            return -ENOMEM;
        }
        entry->red = rgb[i][0];
        entry->green = rgb[i][1];
        entry->blue = rgb[i][2];
        INIT_LIST_HEAD(&entry->list);
        list_add_tail(&entry->list, &color_list);
    }
    i = 0;
    list_for_each_entry(entry, &color_list, list)
        printk(KERN_INFO "color[%d]: red=%d, green=%d, blue=%d\n", i++, entry->red, entry->green, entry->blue);
    return 0;
}
static void __exit color_list_exit(void)
{
    unsigned int freed = color_list_free_all();
    printk(KERN_INFO "color_list: freed %u nodes\n", freed);
}
module_init(color_list_init);
module_exit(color_list_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("IV-2-4 BD2 team");
MODULE_DESCRIPTION("F.CSM302 BD2: circular list of four colors");
