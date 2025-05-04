
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
	{0, 0, 2, 6, 2, 6, 5, 5},      // ��������1������ʱ��2������6
	{0, 0, 3, 10, 3, 10, 8, 8},    // ��������2������ʱ��3������10
	{0, 0, 1, 5, 1, 5, 3, 3},      // ��������3������ʱ��1������5
	{0, 0, 4, 8, 4, 8, 12, 12},    // ��������4������ʱ��4������8
	{0, 0, 2, 7, 2, 7, 7, 7},      // ��������5������ʱ��2������7
	{0, 0, 5, 12, 5, 12, 15, 15},  // ��������6������ʱ��5������12
	{0, 0, 2, 9, 2, 9, 9, 9},      // ��������7������ʱ��2������9
	{0, 0, 3, 11, 3, 11, 14, 14},  // ��������8������ʱ��3������11
	{0, 0, 2, 10, 2, 10, 10, 10},  // ��������9������ʱ��2������10
	{0, 0, 4, 15, 4, 15, 18, 18}   // ��������10������ʱ��4������15
};

tcb_ext_info task_info_array_nocycle[] = {
	{1, 0, 2, 5, 2, 5, 5, 5},      // ����������11������ʱ��2����ֹ����5
	{1, 0, 3, 8, 3, 8, 8, 8},      // ����������12������ʱ��3����ֹ����8
	{1, 0, 1, 3, 1, 3, 3, 3},      // ����������13������ʱ��1����ֹ����3
	{1, 0, 4, 6, 4, 6, 12, 12},    // ����������14������ʱ��4����ֹ����6
	{1, 0, 2, 4, 2, 4, 7, 7},      // ����������15������ʱ��2����ֹ����4
	{1, 0, 5, 10, 5, 10, 15, 15},  // ����������16������ʱ��5����ֹ����10
	{1, 0, 2, 5, 2, 5, 9, 9},      // ����������17������ʱ��2����ֹ����5
	{1, 0, 3, 7, 3, 7, 14, 14},    // ����������18������ʱ��3����ֹ����7
	{1, 0, 2, 6, 2, 6, 10, 10},    // ����������19������ʱ��2����ֹ����6
	{1, 0, 4, 8, 4, 8, 18, 18}     // ����������20������ʱ��4����ֹ����8
};

//OS_STK Task2Stk[TASK_STK_SIZE];
//OS_STK Task3Stk[TASK_STK_SIZE];
//OS_STK Task4Stk[TASK_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  StartupTask (void  *p_arg);

static void createTasks();

//static void startTasks(char *task_name);

static void CycleTask(void* pdata);

//static void CycleTask(void *pdata);

static void NoCycleTask(void *pdata);

static void NoCycleTask2(void* pdata);

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
	for (i = 0;i < 10;i++) {
		OS_STK* TaskStk = (OS_STK*)malloc(TASK_STK_SIZE * sizeof(OS_STK));
		OSTaskCreateExt(CycleTask,
			(void*)0,
			(OS_STK*)&TaskStk[TASK_STK_SIZE - 1],
			4,
			i,
			(OS_STK*)&TaskStk[0],
			TASK_STK_SIZE,
			(void*)&task_info_array_cycle[i],
			OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	}

	for (i = 0;i < 10;i++) {
		OS_STK* TaskStk = (OS_STK*)malloc(TASK_STK_SIZE * sizeof(OS_STK));
		OSTaskCreateExt(NoCycleTask,
			(void*)0,
			(OS_STK*)&TaskStk[TASK_STK_SIZE - 1],
			4,
			i+10,
			(OS_STK*)&TaskStk[0],
			TASK_STK_SIZE,
			(void*)&task_info_array_nocycle[i],
			OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	}


	/*OS_STK TaskStk1[TASK_STK_SIZE];
	OSTaskCreateExt(NoCycleTask,
		(void*)0,
		(OS_STK*)&TaskStk1[TASK_STK_SIZE - 1],
		4,
		10,
		(OS_STK*)&TaskStk1[0],
		TASK_STK_SIZE,
		(void*)&task_info_array_nocycle[0],
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OS_STK TaskStk2[TASK_STK_SIZE];
	OSTaskCreateExt(NoCycleTask2,
		(void*)0,
		(OS_STK*)&TaskStk2[TASK_STK_SIZE - 1],
		6,
		11,
		(OS_STK*)&TaskStk2[0],
		TASK_STK_SIZE,
		(void*)&task_info_array_nocycle[1],
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);*/
	
	//OSTaskCreateExt(CycleTask1,                //ָ����������ָ��
	//	(void *)0,                         //Pdataָ��ָ��һ�����ݽṹ���ýṹ�����ڽ�������ʱ�����񴫵ݲ�����
	//	(OS_STK *)&Task1Stk[TASK_STK_SIZE - 1],//ptosΪָ�������ջջ����ָ�롣
	//	task_info_array_cycle[0].p,             //prioΪ�����һ�����ȼ���������ָ��
	//	1,          //id������ı�ʶ�����������ȼ���ͬ
	//	(OS_STK *)&Task1Stk[0],         //pbosΪָ���ջ�׶˵�ָ�롣
	//	TASK_STK_SIZE,                //stk_size ָ�������ջ�Ĵ�С��
	//	(void *)&task_info_array_cycle[0],//pext��һ���û��������ݽṹ��ָ�룬����ΪTCB����չ��
	//	OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);//opt�����������صĲ�����Ϣ���Ƿ��������ջ���Ƿ���������ջ��
	/*OSTaskCreateExt(CycleTask,
		(void *)0,
		(OS_STK *)&Task2Stk[TASK_STK_SIZE - 1],
		task_info_array_cycle[1].p,
		2,
		(OS_STK *)&Task2Stk[0],
		TASK_STK_SIZE,
		(void *)&task_info_array_cycle[1],
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);*/
	
		/*
	OSTaskCreateExt(NoCycleTask1,
		(void*)0,
		(OS_STK*)&Task4Stk[TASK_STK_SIZE - 1],
		task_info_array_nocycle[1].p,
		4,
		(OS_STK*)&Task4Stk[1],
		TASK_STK_SIZE,
		(void*)&task_info_array_nocycle[1],
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);*/
	/*OSTCBCur = OSTCBPrioTbl[OS_TASK_IDLE_PRIO];
	OSTCBHighRdy = OSTCBPrioTbl[OS_TASK_IDLE_PRIO];*/
}

void CycleTask(void *pdata) {
	// ģ����������һֱѭ�����У�һֱδɾ��
	while (1) {	
		while (((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c > 0) //C ticks
		{
			// do nothing
		}
		OS_ENTER_CRITICAL();
		tcb_ext_info *task_info = (tcb_ext_info*)OSTCBCur->OSTCBExtPtr;
		
		//�����������ʱ��
		((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c = ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->c;
		//����ʣ��ʱ��Ƭ����
		((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->time_quanta_ctr = ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->time_quanta;
		INT32U timestamp = OSTimeGet();
		printf("%-10d\t%d\tComplete\t%d\t%d\n", timestamp, OSTCBCur->OSTCBId, timestamp - 1, timestamp);

		OSTimeDly(task_info->rest_p);
		OS_EXIT_CRITICAL();
	}   
}

//void CycleTask(void *pdata) {
//	// ģ����������һֱѭ�����У�һֱδɾ��
//	while (1) {
//		while (((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c > 0) //C ticks
//		{
//			// do nothing
//		}
//
//		tcb_ext_info* task_info = (tcb_ext_info*)OSTCBCur->OSTCBExtPtr;
//	
//		//�����������ʱ��
//		((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c = ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->c;
//		//����ʣ��ʱ��Ƭ����
//		((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->time_quanta_ctr = ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->time_quanta;
//		INT32U timestamp = OSTimeGet();
//		printf("%-10d\t%d\tComplete\t%d\t%d\n", timestamp, OSTCBCur->OSTCBId, timestamp - 1, timestamp);
//
//		OSTimeDly(task_info->rest_p);
//	}
//}

void NoCycleTask(void *pdata) {
	printf("NoCycleTask id %d start run\n", OSTCBCur->OSTCBId);
	while (((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c > 0) //C ticks
	{
		// do nothing
	}
	INT32U timestamp = OSTimeGet();
	printf("%-10d\t%d\tComplete\t%d\t%d\n", timestamp, OSTCBCur->OSTCBId, timestamp - 1, timestamp);

	// ������ɣ�ɾ��������
	OSTaskDel(OSTCBCur);
}

void NoCycleTask2(void* pdata) {
	printf("NoCycleTask2 id %d start run\n", OSTCBCur->OSTCBId);
	while (((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c > 0) //C ticks
	{
		// do nothing
	}
	INT32U timestamp = OSTimeGet();
	printf("%-10d\t%d\tComplete\t%d\t%d\n", timestamp, OSTCBCur->OSTCBId, timestamp - 1, timestamp);

	// ������ɣ�ɾ��������
	OSTaskDel(OSTCBCur);
}

//void NoCycleTask2(void* pdata) {
//	printf("NoCycleTask2 start run\n");
//	while (((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c > 0) //C ticks
//	{
//		// do nothing
//	}
//	INT32U timestamp = OSTimeGet();
//	printf("%-10d\t%d\tComplete\t%d\t%d\n", timestamp, OSTCBCur->OSTCBId, timestamp - 1, timestamp);
//
//	// ������ɣ�ɾ��������
//	OSTaskDel(OSTCBCur);
//}