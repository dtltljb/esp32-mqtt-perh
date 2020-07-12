// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


#ifndef timer_group_task
#define timer_group_task

#ifdef __cplusplus
extern "C" {
#endif

#define TIMER_DIVIDER         16  //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds
//#define TIMER_INTERVAL0_SEC   (3.4179) // sample test interval for the first timer
#define TIMER_INTERVAL0_SEC   (0.5) // sample test interval for the first timer
#define TIMER_INTERVAL1_SEC   (5.78)   // sample test interval for the second timer
#define TEST_WITHOUT_RELOAD   0        // testing will be done without auto reload
#define TEST_WITH_RELOAD      1        // testing will be done with auto reload

	/*
 * A sample structure to pass events
 * from the timer interrupt handler to the main program.
 */
typedef struct {
    int type;  // the type of timer's event
    int timer_group;
    int timer_idx;
    uint64_t timer_counter_value;
} timer_event_t;

	
extern xQueueHandle timer_queue;	
	
 void example_tg0_timer_init(int timer_idx, 
    bool auto_reload, double timer_interval_sec);
	
 void timer_example_evt_task(void *arg);
    

#ifdef __cplusplus
}
#endif

#endif // adc34_example_task
