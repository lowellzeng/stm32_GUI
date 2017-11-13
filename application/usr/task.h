/*****************************************************
 descriptio: used for store the vars which are used 
 in all task possibly
******************************************************/

#ifndef TASK_H
#define TASK_H
 typedef enum
{
    eHmiTimer = 0,
    eHmiKeyMod,
    eHmiKeyEnt,
    eHmiKeyEsc,
    eHmiKeyKwKwh,
    eHmiKeyKwKwhLong,
    eHmiKeyEntLong,
    eHmiKeyModLong
}HmiTaskEvent_t;

typedef enum{
    eSuperTimer = 0,
		eSuperRtcReady,
}SuperEvent_t;

typedef enum
{
    eCpiFSMTimer = 0,
}CpiFSMEvent_t;



typedef enum
{
    eManageChargerTimer = 0,
}ManageChargerEvent_t;

typedef enum{
    eDebugConsoleCommTimer = 0,
    eDebugConsoleCommEraseChipEnd,
}DebugConsoleCommEvent_t;

typedef enum
{
    eLogTimer = 0,
    eLogStatic,
    eLogGet,
    eLogSN,
    eLogEraseChip,
    eLogToBootloader,
    eLogChkStatistic,
    eLogSetRtc,
    eLogUpdataStartDate,
}LogEvent_t;

typedef enum{
    eFileTimer = 0,
    eFileSave,
	  eFileRead,
}FileEvent_t;

typedef enum{
    eUSBTimer = 0,
    eUSBSave,
	  eUSBRead,
}USBEvent_t;

typedef enum{
    eSDDiskTimer = 0,
    eSDDiskSave,
	  eSDDiskRead,
}SDDiskEvent_t;

typedef enum{
    eEthernetTimer = 0,
    eEthernetSave,
	  eEthernetRead,
}SEthernetEvent_t;

/////下面是 所有任务的 优先级
typedef enum{
	  eCpiFSMPrio = 0,  						/// prio 0 最高
    eManageChargerPrio,  			/// prio 1 
    eSuperPrio,
		eHmiPrio,   
	  eLogPrio,
		eDebugConsoleCommPrio, 								 
    eFilePrio,
		eSDDiskPrio,
		eUSBPrio,
		eEthernetPrio,	
	  eRsvd,                    //10  ---保留的最大值
}Taskprio_t;

void sManageChargerTask(void);
void sSuperTask(void);
void sCpiFSMTask(void);
void sHmiTask(void);
void sDebugConsoleCommTask(void);
void sLogTask(void);
void sFileTask(void);
void sSDDiskTask(void);
void sUSBTask(void);
void sEthernetTask(void);

#endif 

