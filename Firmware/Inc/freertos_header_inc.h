#ifndef __FREERTOS_HEADER_INC_H
#define __FREERTOS_HEADER_INC_H

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Standard demo includes - these are needed here as the tick hook function is
defined in this file. */
#include "TimerDemo.h"
#include "QueueOverwrite.h"
#include "EventGroupsDemo.h"
#include "IntSemTest.h"
#include "QueueSet.h"
#include "TaskNotify.h"

#endif
