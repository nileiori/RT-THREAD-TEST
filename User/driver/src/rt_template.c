#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>


#include <rtdbg.h>
#include <rt_template.h>


#ifdef CUSTOM_USING_TEMPLATE
/* RT-Thread Device Interface */
/*
 * This function initializes io device.
 */
static rt_err_t rt_template_init(struct rt_device *dev)
{
    rt_err_t result = RT_EOK;
    rt_template_t temp;

    RT_ASSERT(dev != RT_NULL);
    temp = (rt_template_t)dev;

    /* initialize rx/tx */
    temp->template_rx = RT_NULL;
    temp->template_tx = RT_NULL;

    /* apply configuration */
    if (temp->ops->init)
        result = temp->ops->init(temp, &temp->config);

    return result;
}

static rt_err_t rt_template_open(struct rt_device *dev, rt_uint16_t oflag)
{
    
		
    return RT_EOK;
}

static rt_err_t rt_template_close(struct rt_device *dev)
{
    

    return RT_EOK;
}

static rt_size_t rt_template_read(struct rt_device *dev,
                                rt_off_t          pos,
                                void             *buffer,
                                rt_size_t         size)
{
		rt_err_t result = RT_EOK;
		rt_template_t temp;

		RT_ASSERT(dev != RT_NULL);
		temp = (rt_template_t)dev;

		/* apply configuration */
		if (temp->ops->read)
				result = temp->ops->read(temp);

		return result;
}


static rt_size_t rt_template_write(struct rt_device *dev,
                                 rt_off_t          pos,
                                 const void       *buffer,
                                 rt_size_t         size)
{
		rt_err_t result = RT_EOK;
		rt_template_t temp;

		RT_ASSERT(dev != RT_NULL);
		temp = (rt_template_t)dev;

		/* apply configuration */
		if (temp->ops->write)
				result = temp->ops->write(temp);

		return result;
}


static rt_err_t rt_template_control(struct rt_device *dev,
                                  int              cmd,
                                  void             *args)
{
		rt_err_t result = RT_EOK;
		rt_template_t temp;

		RT_ASSERT(dev != RT_NULL);
		temp = (rt_template_t)dev;

		/* apply configuration */
		if (temp->ops->control)
				result = temp->ops->control(temp);

		return result;
}


#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops default_template_ops = 
{
    rt_template_init,
    rt_template_open,
    rt_template_close,
    rt_template_read,
    rt_template_write,
    rt_template_control
};
#endif
rt_err_t rt_template_register(rt_template_t item,
                               const char              *name,
                               rt_uint32_t              flag,
                               void                    *data)
{
    rt_err_t ret;
    rt_device_t device;
    RT_ASSERT(item != RT_NULL);

    device = &(item->parent);

    device->type        = RT_Device_Class_Miscellaneous;//RT_Device_Class_Custom;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &default_template_ops;
#else
    device->init        = rt_template_init;
    device->open        = rt_template_open;
    device->close       = rt_template_close;
    device->read        = rt_template_read;
    device->write       = rt_template_write;
    device->control     = rt_template_control;
#endif
    device->user_data   = data;

    /* register a character device */
    ret = rt_device_register(device, name, flag);

    return ret;
}

struct rt_template_device template_item;
rt_uint32_t	template_data;
static rt_err_t template_init(rt_template_t item, struct rt_template_configure *cfg)
{   
		#ifdef	RT_USING_CONSOLE		 
		rt_kprintf("template_init...\r\n");
		#endif	 
    return RT_EOK;
}
static rt_err_t template_read(rt_template_t item)
{   
		#ifdef	RT_USING_CONSOLE		 
		rt_kprintf("template_read...\r\n");
		#endif	 
    return RT_EOK;
}
static rt_err_t template_write(rt_template_t item)
{    
		#ifdef	RT_USING_CONSOLE		 
		rt_kprintf("template_write...\r\n");
		#endif	 
    return RT_EOK;
}
static rt_err_t template_control(rt_template_t item)
{ 	
		#ifdef	RT_USING_CONSOLE		 
		rt_kprintf("template_control...\r\n");
		#endif	 
		return RT_EOK;
}

const static struct rt_template_ops template_ops =
{
    template_init,
		template_read,
		template_write,
    template_control,
};
void custom_template_init(void)
{
	template_item.ops = &template_ops;

	/* register io device */
  rt_template_register(&template_item, CUSTOM_DEVICE_NAME_ID1,
									RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX,
									&template_data);
}
rt_err_t  custom_init(void)
{
    //RT_ASSERT(template_item.ops != RT_NULL);
    return template_item.parent.init(&template_item.parent);
}
rt_err_t  custom_read(void)
{
		rt_uint8_t buf[10];
    //RT_ASSERT(template_item.ops != RT_NULL);
    return template_item.parent.read(&template_item.parent,1,buf,0);
}
rt_err_t  custom_write(void)
{
		rt_uint8_t buf[10];
    //RT_ASSERT(template_item.ops != RT_NULL);
    return template_item.parent.write(&template_item.parent,1,buf,0);
}
rt_err_t  custom_control(void)
{
		rt_uint8_t arg = 0;
    //RT_ASSERT(template_item.ops != RT_NULL);
    return template_item.parent.control(&template_item.parent,1,&arg);
}
void custom_test(void)
{
		custom_init();
		custom_read();
		custom_write();
		custom_control();
}

#endif

