
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
	{0,0,2,4,2,4,1,1,0},{0,0,3,6,3,6,1,1,0}
};

tcb_ext_info task_info_array_nocycle[] = {
	{1,0,4,10,4,10,1,1,0}, {1,0,3,5,3,5,5,5,0}
};
OS_STK Task1Stk[TASK_STK_SIZE];
OS_STK Task2Stk[TASK_STK_SIZE];
OS_STK Task3Stk[TASK_STK_SIZE];
OS_STK Task4Stk[TASK_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  StartupTask (void  *p_arg);

static void createTasks();

//static void startTasks(char *task_name);

static void CycleTask1(void* pdata);

static void CycleTask2(void *pdata);

static void NoCycleTask1(void *pdata);

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
	OS_EVENT* device = OSSemCreate(1); 
	
	//OSTaskCreateExt(CycleTask1,                //ָ����������ָ��
	//	(void *)0,                         //Pdataָ��ָ��һ�����ݽṹ���ýṹ�����ڽ�������ʱ�����񴫵ݲ�����
	//	(OS_STK *)&Task1Stk[TASK_STK_SIZE - 1],//ptosΪָ�������ջջ����ָ�롣
	//	task_info_array_cycle[0].p,             //prioΪ�����һ�����ȼ���������ָ��
	//	1,          //id������ı�ʶ�����������ȼ���ͬ
	//	(OS_STK *)&Task1Stk[0],         //pbosΪָ���ջ�׶˵�ָ�롣
	//	TASK_STK_SIZE,                //stk_size ָ�������ջ�Ĵ�С��
	//	(void *)&task_info_array_cycle[0],//pext��һ���û��������ݽṹ��ָ�룬����ΪTCB����չ��
	//	OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);//opt�����������صĲ�����Ϣ���Ƿ��������ջ���Ƿ���������ջ��
	/*OSTaskCreateExt(CycleTask2,
		(void *)0,
		(OS_STK *)&Task2Stk[TASK_STK_SIZE - 1],
		task_info_array_cycle[1].p,
		2,
		(OS_STK *)&Task2Stk[0],
		TASK_STK_SIZE,
		(void *)&task_info_array_cycle[1],
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);*/

	
	OSTaskCreateExt(NoCycleTask1,
		(void *)0,
		(OS_STK *)&Task3Stk[TASK_STK_SIZE - 1],
		task_info_array_nocycle[0].p,
		3,
		(OS_STK *)&Task3Stk[0],
		TASK_STK_SIZE,
		(void *)&task_info_array_nocycle[0],
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
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

void CycleTask1(void *pdata) {
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

void CycleTask2(void *pdata) {
	// ģ����������һֱѭ�����У�һֱδɾ��
	while (1) {
		while (((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c > 0) //C ticks
		{
			// do nothing
		}

		tcb_ext_info* task_info = (tcb_ext_info*)OSTCBCur->OSTCBExtPtr;
	
		//�����������ʱ��
		((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c = ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->c;
		//����ʣ��ʱ��Ƭ����
		((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->time_quanta_ctr = ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->time_quanta;
		INT32U timestamp = OSTimeGet();
		printf("%-10d\t%d\tComplete\t%d\t%d\n", timestamp, OSTCBCur->OSTCBId, timestamp - 1, timestamp);

		OSTimeDly(task_info->rest_p);
	}
}

void NoCycleTask1(void *pdata) {
	printf("NoCycleTask1 start run\n");
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
	printf("NoCycleTask2 start run\n");
	while (((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c > 0) //C ticks
	{
		// do nothing
	}
	INT32U timestamp = OSTimeGet();
	printf("%-10d\t%d\tComplete\t%d\t%d\n", timestamp, OSTCBCur->OSTCBId, timestamp - 1, timestamp);

	// ������ɣ�ɾ��������
	OSTaskDel(OSTCBCur);
}