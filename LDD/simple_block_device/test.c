#include <linux/init.h>
#include <linux/module.h>

#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include <linux/fs.h>
#include <linux/genhd.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple block device driver");
MODULE_AUTHOR("ilia");


#define BLOCK_MAJOR		222
#define BLOCK_NAME		"testblockname"
#define BLOCK_MINORS		1		/* one partition */
#define N_SECTORS		(1024 * 200)	/* 100 mb */
#define KERNEL_SECTOR_SIZE	512


/* 
 * Internal representation of the device
 */
static struct block_dev {
	struct blk_mq_tag_set tag_set;	/* tag set */
	struct request_queue *queue;	/* device request queue*/
	struct gendisk *gd;		/* gendist structure*/
	u8 *data;			/* data array */
	size_t size;			/* device size */
} dev;

/*
 * Device operations
 */
static const struct block_device_operations block_ops = {
	.owner = THIS_MODULE,
};

/*
 * Handle an I/O request
 */
static int do_simple_request(struct request *rq, unsigned int *nr_bytes)
{
    	int ret = 0;
    	struct bio_vec bvec;
    	struct req_iterator iter;
    	struct block_dev *dev = rq->q->queuedata;
    	loff_t pos = blk_rq_pos(rq) << SECTOR_SHIFT;
    	loff_t dev_size = (loff_t)(dev->size << SECTOR_SHIFT);
    
    	rq_for_each_segment(bvec, rq, iter)
    	{
        	unsigned long b_len = bvec.bv_len;

        	void* b_buf = page_address(bvec.bv_page) + bvec.bv_offset;

        	if ((pos + b_len) > dev_size)
            		b_len = (unsigned long)(dev_size - pos);

        	if (rq_data_dir(rq)) /* write */
            		memcpy(dev->data + pos, b_buf, b_len);
        	else /* read */
            		memcpy(b_buf, dev->data + pos, b_len);

        	pos += b_len;
        	*nr_bytes += b_len;
    	}

    	return ret;
}

/*
 * Request handling function
 */
static blk_status_t block_request(struct blk_mq_hw_ctx *hctx, 
				  const struct blk_mq_queue_data* bd)
{
    	unsigned int nr_bytes = 0;
    	blk_status_t status = BLK_STS_OK;
    	struct request *rq = bd->rq;

    	blk_mq_start_request(rq);

	printk(KERN_INFO "INFO: received request dir=%c\n", rq_data_dir(rq) ? 'W' : 'R');
    	if (do_simple_request(rq, &nr_bytes) != 0) {
        	status = BLK_STS_IOERR;
	}
    	printk(KERN_INFO "INFO: request processed %d bytes\n", nr_bytes);

    	blk_mq_end_request(rq, status);

    	return BLK_STS_OK;
}

static struct blk_mq_ops queue_ops = {
	.queue_rq = block_request,
};

/*
 * Module initialization function
 */
static int create_block_device(struct block_dev *dev)
{
	int err;

	/* Register a disk */
	dev->size = N_SECTORS * KERNEL_SECTOR_SIZE;
	dev->data = vmalloc(dev->size);
	if (dev->data == NULL) {
		printk(KERN_ERR "ERROR: vmalloc: out of memory\n");
		err = -ENOMEM;
		goto out_vmalloc;
	}

	/* Initialize tag set */
	dev->tag_set.ops = &queue_ops;
	dev->tag_set.nr_hw_queues = 1;
	dev->tag_set.queue_depth = 128;
	dev->tag_set.numa_node = NUMA_NO_NODE;
	dev->tag_set.cmd_size = 0;
	dev->tag_set.flags = BLK_MQ_F_SHOULD_MERGE;
	err = blk_mq_alloc_tag_set(&dev->tag_set);
	if (err) {
		printk(KERN_ERR "ERROR: blk_mq_alloc_tag_set: couldn't allocate tag set\n");
		goto out_alloc_tag_set;
	}

	/* Allocate queue */
	dev->queue = blk_mq_init_queue(&dev->tag_set);
	if (IS_ERR(dev->queue)) {
		printk(KERN_ERR "ERROR: blk_mq_init_queue: couldn't allocate queue\n");
		err = -ENOMEM;
		goto out_blk_init;
	}
	blk_queue_logical_block_size(dev->queue, KERNEL_SECTOR_SIZE);
	dev->queue->queuedata = dev;

	/* Initialize the gendist structure */
	dev->gd = alloc_disk(BLOCK_MINORS);
	if (!dev->gd) {
		printk(KERN_ERR "ERROR: alloc_disk: couldn't get a gendisk structure\n");
		err = -ENOMEM;
		goto out_alloc_disk;
	}
	dev->gd->major = BLOCK_MAJOR;
	dev->gd->first_minor = 0;
	dev->gd->fops = &block_ops;
	dev->gd->queue = dev->queue;
	dev->gd->private_data = dev;
	snprintf(dev->gd->disk_name, DISK_NAME_LEN, "testdiskname");
	set_capacity(dev->gd, N_SECTORS);

	add_disk(dev->gd);

	return 0;

out_alloc_disk:
	blk_cleanup_queue(dev->queue);
out_blk_init:
	blk_mq_free_tag_set(&dev->tag_set);
out_alloc_tag_set:
	vfree(dev->data);
out_vmalloc:
	return err;
}

static int __init block_init(void)
{
	int err = 0;
	
	/* Register block device */
	err = register_blkdev(BLOCK_MAJOR, BLOCK_NAME);
	if (err < 0) {
		printk(KERN_ERR "ERROR: register_blkdev: unable to register\n");
		return err;
	}
	printk(KERN_INFO "INFO: Block device registered\n");

	/* Create block device */
	err = create_block_device(&dev);
	if (err < 0) {
		unregister_blkdev(BLOCK_MAJOR, BLOCK_NAME);
		printk(KERN_ERR "ERROR: create_block_device: unable to create block device\n");
		return err;
	}

	printk(KERN_INFO "INFO: Block device created\n");
	return 0;
}

/*
 * Module cleanup function
 */
static void delete_block_device(struct block_dev *dev)
{
	if (dev->gd) {
		del_gendisk(dev->gd);
		put_disk(dev->gd);
	}

	if (dev->queue)
		blk_cleanup_queue(dev->queue);
	if (dev->tag_set.tags)
		blk_mq_free_tag_set(&dev->tag_set);
	if (dev->data)
		vfree(dev->data);
}

static void __exit block_exit(void)
{
	/* Cleanup block device */
	delete_block_device(&dev);
	printk(KERN_INFO "INFO: Block device deleted\n");
	
	/* Unregister block device */
	unregister_blkdev(BLOCK_MAJOR, BLOCK_NAME);
	printk(KERN_INFO "INFO: Block device unregistered\n");
}

module_init(block_init);
module_exit(block_exit);
/* sudo tail -f /var/log/messages | grep "kernel" & */
