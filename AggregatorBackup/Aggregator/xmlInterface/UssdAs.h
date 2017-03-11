#ifndef __TESTCLASS__
#define __TESTCLASS__

#include<IFrameWork.hpp>

class UssdAs: public FrameWork::IFrameWork
{
	public:
           UssdAs();
           void mcfn_initApplication() ;
           void mcfn_clearApplication();
};

#endif
