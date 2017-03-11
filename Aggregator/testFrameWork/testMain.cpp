#include"testMain.h"
#include "IConfigParams.hpp"

#define MODULE_NAME "TEST"
#define VERSION "1.0"
#define RELEASE_DATE "27-06-2015"

testClass CG_application;
FrameWork::IConfiguration CG_Cfg("test");

testClass::testClass( ) : FrameWork::IFrameWork(MODULE_NAME,VERSION,RELEASE_DATE)
{

}

void  testClass::mcfn_initApplication() 
{

     printf("class been created\n");
}
