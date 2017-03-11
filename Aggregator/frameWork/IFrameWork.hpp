#ifndef __FRAMEWORK__
#define __FRAMEWORK__
#include<string>
namespace FrameWork
{
	class IFrameWork
	{
		private:
			std::string meC_moduleName;
			std::string meC_version;
			std::string meC_releaseDate;
			bool meb_appicationStatus;
		public:
			IFrameWork(std::string moduleName,std::string version,std::string releaseDate);
			virtual void mcfn_initApplication() = 0;
                        virtual void mcfn_clearApplication() = 0;
			virtual void mcfn_stop() ;
			virtual void mcfn_handleApplication();
			void mcfn_printBanner(bool isEventLogObjCreated= false);
			bool mcfn_getApplicationStatus();
                        std::string mcfn_getModuleName();
	};
	inline bool IFrameWork::mcfn_getApplicationStatus()
	{
		return meb_appicationStatus;
	}
	inline void IFrameWork::mcfn_stop()
	{
		meb_appicationStatus = false;
	}
        inline std::string IFrameWork::mcfn_getModuleName()
        {
           return meC_moduleName;
      }
}
#endif
