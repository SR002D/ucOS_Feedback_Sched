
/*
*********************************************************************************************************
*
*                                              uC/OS-II
*                                            EXAMPLE CODE
*
* Filename : main.c
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>

#include  "app_cfg.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  TASK_STK_SIZE    128
/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_STK  StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE];
tcb_ext_info task_info_array_cycle[] = {
	{0, 0, 2, 6, 2, 6, 5, 5},      // 周期任务1：运行时间2，周期6
	{0, 0, 3, 10, 3, 10, 8, 8},    // 周期任务2：运行时间3，周期10
	{0, 0, 1, 5, 1, 5, 3, 3},      // 周期任务3：运行时间1，周期5
	{0, 0, 4, 8, 4, 8, 12, 12},    // 周期任务4：运行时间4，周期8
	{0, 0, 2, 7, 2, 7, 7, 7},      // 周期任务5：运行时间2，周期7
	{0, 0, 5, 12, 5, 12, 15, 15},  // 周期任务6：运行时间5，周期12
	{0, 0, 2, 9, 2, 9, 9, 9},      // 周期任务7：运行时间2，周期9
	{0, 0, 3, 11, 3, 11, 14, 14},  // 周期任务8：运行时间3，周期11
	{0, 0, 2, 10, 2, 10, 10, 10},  // 周期任务9：运行时间2，周期10
	{0, 0, 4, 15, 4, 15, 18, 18}   // 周期任务10：运行时间4，周期15
};

tcb_ext_info task_info_array_nocycle[] = {
	{1, 0, 2, 5, 2, 5, 5, 5},      // 非周期任务11：运行时间2，截止日期5
	{1, 0, 3, 8, 3, 8, 8, 8},      // 非周期任务12：运行时间3，截止日期8
	{1, 0, 1, 3, 1, 3, 3, 3},      // 非周期任务13：运行时间1，截止日期3
	{1, 0, 4, 6, 4, 6, 12, 12},    // 非周期任务14：运行时间4，截止日期6
	{1, 0, 2, 4, 2, 4, 7, 7},      // 非周期任务15：运行时间2，截止日期4
	{1, 0, 5, 10, 5, 10, 15, 15},  // 非周期任务16：运行时间5，截止日期10
	{1, 0, 2, 5, 2, 5, 9, 9},      // 非周期任务17：运行时间2，截止日期5
	{1, 0, 3, 7, 3, 7, 14, 14},    // 非周期任务18：运行时间3，截止日期7
	{1, 0, 2, 6, 2, 6, 10, 10},    // 非周期任务19：运行时间2，截止日期6
	{1, 0, 4, 8, 4, 8, 18, 18}     // 非周期任务20：运行时间4，截止日期8
};

static int N = 20;
static float P = 1.5;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  StartupTask (void  *p_arg);

static void createTasks();

static void CycleTask(void* pdata);

static void NoCycleTask(void *pdata);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : none
*********************************************************************************************************
*/

int  main (void)
{
#if OS_TASK_NAME_EN > 0u
    CPU_INT08U  os_err;
#endif


    CPU_IntInit();

    Mem_Init();                                                 /* Initialize Memory Managment Module                   */
    CPU_IntDis();                                               /* Disable all Interrupts                               */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */

    OSInit();                                                   /* Initialize uC/OS-II                                  */
	printf("Time\tTask\tEvent\tFrom\tTo\n");
	createTasks();

#if OS_TASK_NAME_EN > 0u
    OSTaskNameSet(         APP_CFG_STARTUP_TASK_PRIO,
                  (INT8U *)"Startup Task",
                           &os_err);
#endif
    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)   */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }
}


/*
*********************************************************************************************************
*                                            STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'StartupTask()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  StartupTask (void *p_arg)
{
   (void)p_arg;

    OS_TRACE_INIT();                                            /* Initialize the uC/OS-II Trace recorder               */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
    
    APP_TRACE_DBG(("uCOS-III is Running...\n\r"));

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        OSTimeDlyHMSM(0u, 0u, 1u, 0u);
		APP_TRACE_DBG(("Time: %d\n\r", OSTimeGet()));
    }
}


void createTasks() {
	int i;
	for (i = 0;i < N*0;i++) {
		OS_STK* TaskStk = (OS_STK*)malloc(TASK_STK_SIZE * sizeof(OS_STK));
		tcb_ext_info* task_info_list = (tcb_ext_info*)malloc(sizeof(tcb_ext_info));


		int num = Math_Rand()%20 + 1;	// 生成1-20的随机数

		task_info_list->c = num;
		task_info_list->p = N*num/P;

		task_info_list->time_quanta = 5;
		task_info_list->t = 0;
		task_info_list->pri = 0;
		task_info_list->rest_c = task_info_list->c;
		task_info_list->rest_p = task_info_list->p;
		task_info_list->time_quanta_ctr = task_info_list->time_quanta;

		OSTaskCreateExt(CycleTask,
			(void*)0,
			(OS_STK*)&TaskStk[TASK_STK_SIZE - 1],
			4,
			i,
			(OS_STK*)&TaskStk[0],
			TASK_STK_SIZE,
			(void*)task_info_list,
			OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

		OSTaskTotalCtr++;
	}


	for (i = 0;i < N;i++) {
		OS_STK* TaskStk = (OS_STK*)malloc(TASK_STK_SIZE * sizeof(OS_STK));
		tcb_ext_info* task_info_list = (tcb_ext_info*)malloc(sizeof(tcb_ext_info));

		int num = Math_Rand() % 20 + 1;	// 生成1-20的随机数

		task_info_list->c = num;
		task_info_list->p = N * num / P;

		task_info_list->time_quanta = 5;
		task_info_list->t = 1;
		task_info_list->pri = 0;
		task_info_list->rest_c = task_info_list->c;
		task_info_list->rest_p = task_info_list->p;
		task_info_list->time_quanta_ctr = task_info_list->time_quanta;

		OSTaskCreateExt(NoCycleTask,
			(void*)0,
			(OS_STK*)&TaskStk[TASK_STK_SIZE - 1],
			4  ,
			i+10,
			(OS_STK*)&TaskStk[0],
			TASK_STK_SIZE,
			(void*)task_info_list,
			OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

		OSTaskTotalCtr++;
	}
}

void CycleTask(void *pdata) {
	// 模拟周期任务，一直循环进行，一直未删除
	while (1) {	
		while (((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c > 0) //C ticks
		{
			// do nothing
		}
		OS_ENTER_CRITICAL();
		tcb_ext_info *task_info = (tcb_ext_info*)OSTCBCur->OSTCBExtPtr;
		
		//重置任务完成时间
		((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c = ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->c;
		//重置剩余时间片长度
		((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->time_quanta_ctr = ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->time_quanta;
		INT32U timestamp = OSTimeGet();
		printf("[Time]%-10d\t%d\tComplete\t%d\t%d\n", timestamp, OSTCBCur->OSTCBId, timestamp - 1, timestamp);

		OSTimeDly(task_info->rest_p);
		OS_EXIT_CRITICAL();
	}   
}

void NoCycleTask(void *pdata) {
	while (((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c > 0) //C ticks
	{
		// do nothing
	}
	OS_ENTER_CRITICAL();

	tcb_ext_info* task_info = (tcb_ext_info*)OSTCBCur->OSTCBExtPtr;

	//printf("task_info rest_p %d\n", task_info->rest_p);

	INT32U timestamp = OSTimeGet();
	printf("[Time]%-10d\t%d\tComplete and Del\t%d\t%d\n", timestamp, OSTCBCur->OSTCBId, timestamp - 1, timestamp);

	// 运行完成，删除该任务
	OSTaskDel(OSTCBCur);
	OS_EXIT_CRITICAL();
}