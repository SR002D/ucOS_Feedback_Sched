
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
tcb_ext_info task_info_array[] = {
	{0,0,1,4,1,4,0},{0,0,2,5,2,5,0},{1,0,2,10,2,10,0}
};
OS_STK Task1Stk[TASK_STK_SIZE];
OS_STK Task2Stk[TASK_STK_SIZE];
OS_STK Task3Stk[TASK_STK_SIZE];

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

static void NoCycleTask3(void *pdata);

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
	printf("Time       Event\tFrom\tTo");
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
	
	OSTaskCreateExt(CycleTask1,                //ָ����������ָ��
		(void *)0,                         //Pdataָ��ָ��һ�����ݽṹ���ýṹ�����ڽ�������ʱ�����񴫵ݲ�����
		(OS_STK *)&Task1Stk[TASK_STK_SIZE - 1],//ptosΪָ�������ջջ����ָ�롣
		task_info_array[0].p,             //prioΪ�����һ�����ȼ���������ָ��
		1,          //id������ı�ʶ�����������ȼ���ͬ
		(OS_STK *)&Task1Stk[0],         //pbosΪָ���ջ�׶˵�ָ�롣
		TASK_STK_SIZE,                //stk_size ָ�������ջ�Ĵ�С��
		(void *)&task_info_array[0],//pext��һ���û��������ݽṹ��ָ�룬����ΪTCB����չ��
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);//opt�����������صĲ�����Ϣ���Ƿ��������ջ���Ƿ���������ջ��
	/*OSTaskCreateExt(CycleTask2,
		(void *)0,
		(OS_STK *)&Task2Stk[TASK_STK_SIZE - 1],
		task_info_array[1].p,
		task_info_array[1].p,
		(OS_STK *)&Task2Stk[0],
		TASK_STK_SIZE,
		(void *)&task_info_array[1],
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);*/
	
	OSTaskCreateExt(NoCycleTask3,
		(void *)0,
		(OS_STK *)&Task3Stk[TASK_STK_SIZE - 1],
		task_info_array[2].p,
		task_info_array[2].p,
		(OS_STK *)&Task3Stk[0],
		TASK_STK_SIZE,
		(void *)&task_info_array[2],
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	/*OSTCBCur = OSTCBPrioTbl[OS_TASK_IDLE_PRIO];
	OSTCBHighRdy = OSTCBPrioTbl[OS_TASK_IDLE_PRIO];*/
}

void CycleTask1(void *pdata) {
	// ģ����������һֱѭ�����У�һֱδɾ��
	while (1) {	
		while (((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c > 0) //C ticks
		{
			// do nothing
			//printf("\ncycle task 1 is running, rest_c is %d", ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c);
		}
		                                  
		OS_ENTER_CRITICAL();

		// rest_c = 0 ,�������
		/*INT32U timestamp = OSTimeGet();
		printf("\n%-10d Complete\t%d\t%d", timestamp, OSTCBCur->OSTCBPrio, OSPrioHighRdy);*/

		tcb_ext_info *task_info = (tcb_ext_info*)OSTCBCur->OSTCBExtPtr;
		//printf("\ncycle task 1 delay for %d ticks.", task_info->rest_p);
		//�����������ʱ��
		((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c = ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->c;
		//printf("\ncycle task 1 set rest_c to %d", ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->c);
		OS_EXIT_CRITICAL();
		OSTimeDly(task_info->rest_p);
	}
}

void CycleTask2(void *pdata) {
	// ģ����������һֱѭ�����У�һֱδɾ��
	while (1) {
		while (((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c > 0) //C ticks
		{
			// do nothing
			//printf("\ncycle task 1 is running, rest_c is %d", ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c);
		}

		OS_ENTER_CRITICAL();
		tcb_ext_info* task_info = (tcb_ext_info*)OSTCBCur->OSTCBExtPtr;
		printf("\ncycle task 2 delay for %d ticks.", task_info->rest_p);
		//�����������ʱ��
		((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->rest_c = ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->c;
		printf("\ncycle task 2 set rest_c to %d", ((tcb_ext_info*)OSTCBCur->OSTCBExtPtr)->c);
		OS_EXIT_CRITICAL();
		OSTimeDly(task_info->rest_p);
	}
}

void NoCycleTask3(void *pdata) {
	printf("NoCycleTask3 running");
}