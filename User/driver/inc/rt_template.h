#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

#include <rtthread.h>
#include <system_cfg.h>


#ifdef CUSTOM_USING_TEMPLATE
struct rt_template_configure
{
    rt_uint32_t template_para1;
		rt_uint32_t template_para2;
		rt_uint32_t template_para3;
};
struct rt_template_device
{
    struct rt_device          parent;

    const struct rt_template_ops *ops;
		
		struct rt_template_configure   config;
		
    void *template_rx;
    void *template_tx;
};
typedef struct rt_template_device *rt_template_t;
struct rt_template_ops
{
		rt_err_t (*init)(rt_template_t item, struct rt_template_configure *cfg);
    rt_err_t (*read)(rt_template_t item);
		rt_err_t (*write)(rt_template_t item);
    rt_err_t (*control)(rt_template_t item);
};

rt_err_t rt_template_register(rt_template_t item,
                               const char              *name,
                               rt_uint32_t              flag,
                               void                    *data);

void custom_template_init(void);

#endif

#endif
