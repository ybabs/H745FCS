#pragma once

#ifdef __cplusplus
 extern "C" {
#endif





 typedef enum
 {
 	HSEM_BOOT = 0,
 	HSEM_ACC,
	HSEM_GYRO,
	HSEM_MAG,
	HSEM_BARO,
	HSEM_GPS,
 } HSEM_ID;

 typedef enum
 {
 	M7_IDLE,
 	M7_ACC

 } M7_STATE;

 extern volatile M7_STATE m7_state;
 int32_t lock_unlock_hsem(HSEM_ID hsem_id);
 int32_t lock_hsem(HSEM_ID hsem_id);
 void unlock_hsem(HSEM_ID hsem_id);
 void lock_unlock_callback(uint32_t sem_mask);




#ifdef __cplusplus
}
#endif

