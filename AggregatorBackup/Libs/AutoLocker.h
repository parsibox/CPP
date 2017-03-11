/*
 * AutoLocker.h
 *
 *  Created on: Oct 23, 2013
 *      Author: surya
 */

#ifndef CAUTOLOCKER_H_
#define CAUTOLOCKER_H_
#include"CriticalSection.h"

class CAutoLocker {
	private:
		CCriticalSection *pmeC_Lock;
	public:
		CAutoLocker(CCriticalSection &CL_Lock){
			pmeC_Lock=&CL_Lock;
			pmeC_Lock->Lock();
		}
		~CAutoLocker(){
			pmeC_Lock->Unlock();
		}
};
#endif /* CAUTOLOCKER_H_ */
