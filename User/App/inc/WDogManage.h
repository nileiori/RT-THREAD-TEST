#ifndef	_WDOGMANAGE_H_
#define	_WDOGMANAGE_H_

#ifdef	_WDOGMANAGE_GLOBALS
#define	_WDOGMANAGE_EXT	
#else
#define	_WDOGMANAGE_EXT	extern
#endif

#include <rtdef.h>

#define	WDOG_EVENT_FLAG_0		(0x1 << 0)
#define	WDOG_EVENT_FLAG_1		(0x1 << 1)
#define	WDOG_EVENT_FLAG_2		(0x1 << 2)
#define	WDOG_EVENT_FLAG_3		(0x1 << 3)
#define	WDOG_EVENT_FLAG_4		(0x1 << 4)
#define	WDOG_EVENT_FLAG_5		(0x1 << 5)
#define	WDOG_EVENT_FLAG			(WDOG_EVENT_FLAG_1|WDOG_EVENT_FLAG_2|WDOG_EVENT_FLAG_3|WDOG_EVENT_FLAG_4/*|WDOG_EVENT_FLAG_5*/ )

_WDOGMANAGE_EXT rt_event_t wdog_event;

_WDOGMANAGE_EXT void watchDog_thread_init(void);

#endif
