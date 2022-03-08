/*
 * rc.h
 *
 *  Created on: 16 Nov 2021
 *      Author: Daniel Babatunde
 */

#ifndef INC_RC_H_
#define INC_RC_H_


typedef struct {

  float coeff[2];
  float output[2];

}RCFilter;

void RCInit(RCFilter *filt, float fc, float dt);
float RCUpdate(RCFilter *filt, float input);


#endif /* INC_RC_H_ */
