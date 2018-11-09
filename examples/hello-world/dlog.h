/**
 * @file    dbg.h
 * @brief    The tv control module for Tizen
 * @author   TaeHwan Kim <tang.kim@samsung.com>
 *
 * This file contains definition of controller registers.
 *
 * Copyright 2015 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef __DALI_DEMO_LOGGER__
#define __DALI_DEMO_LOGGER__

#if   DALI_DEMO_UBUNTU

#include <stdlib.h>
#define DALI_LOGE(msg) std::cout << msg 
#define DALI_LOGE(msg) std::cout << msg 
#define DALI_LOGE(msg) std::cout << msg 

#else

#include <dlog.h>

#define DALI_LOGE(msg) do { \
	std::ostringstream os; \
	os << msg; \
	__dlog_print(LOG_ID_MAIN, DLOG_ERROR, "dali-demo", "%s", os.str().c_str());	\
	} while(0)

#define DALI_LOGD(msg) do { \
	std::ostringstream os; \
	os << msg; \
	__dlog_print(LOG_ID_MAIN, DLOG_DEBUG, "dali-demo", "%s", os.str().c_str());	\
	} while(0)

#define DALI_LOGI(msg) do { \
	std::ostringstream os; \
	os << msg; \
	__dlog_print(LOG_ID_MAIN, DLOG_INFO, "dali-demo", "%s", os.str().c_str());	\
	} while(0)


#endif

#endif /* __DALI_DEMO_LOGGER__ */
