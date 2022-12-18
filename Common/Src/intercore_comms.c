/*
 * intercore_comms.c
 *
 *  Created on: 13 Dec 2022
 *      Author: Daniel
 */

#include "stm32h7xx_hal.h"
#include "intercore_comms.h"

#define IS_HSEMID_IN_MASK(_HSEM_ID_, _SEM_MASK_) (_SEM_MASK_ & __HAL_HSEM_SEMID_TO_MASK(_HSEM_ID_))


volatile M7_STATE m7_state;

 int32_t lock_unlock_hsem(HSEM_ID hsem_id)
 {
     int32_t ret_val = 0;
     if (HAL_OK != HAL_HSEM_FastTake(hsem_id))
     {
         ret_val = 1;
     }
     else
     {
         HAL_HSEM_Release(hsem_id, 0);
     }
     return ret_val;
 }

 int32_t lock_hsem(HSEM_ID hsem_id)
 {
     int32_t ret_val = 0;
     if (HAL_OK != HAL_HSEM_FastTake(hsem_id))
     {
         ret_val = 1;
     }
     return ret_val;
 }

 void unlock_hsem(HSEM_ID hsem_id)
 {
     HAL_HSEM_Release(hsem_id, 0);
 }

 void lock_unlock_callback(uint32_t sem_mask)
 {
     if (IS_HSEMID_IN_MASK(HSEM_ACC, sem_mask))
     {
         if (M7_IDLE == m7_state)
         {
             m7_state = M7_ACC;
         }
         else
         {
             m7_state = M7_IDLE;
         }
     }
 }
