/*
 * ITimer.h
 *
 *  Created on: Dec 18, 2013
 *      Author: surya
 */

#ifndef INTERFACETIMER_H_
#define INTERFACETIMER_H_

class ITimer{
	public:
		int mcsi_timerTime;		
	public:
		virtual void mcfn_onTimerExpiry(void *)=0;
};
#endif
