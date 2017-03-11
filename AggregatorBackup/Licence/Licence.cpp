#include "Licence.h"
#include "Rsa.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netdb.h>
#include <string.h>
#include <time.h>

CDate::CDate(int iL_day,int iL_month,int iL_year)
{
  mcsi_day = iL_day ;
  mcsi_month = iL_month ;
  mcsi_year = iL_year ;
}

CDate::CDate(std::string CL_date)
{
  sscanf(CL_date.c_str(),"%4d%2d%2d",&mcsi_year,&mcsi_month,&mcsi_day);
}

CDate::~CDate()
{
}

bool CDate::mcfn_isLeap(int iL_year)
{
  return (iL_year % 4 == 0) && ( iL_year % 100 != 0) || (iL_year % 400 == 0);
}

int CDate::mcfn_daysTillMonth(int iL_month )
{
  //this days caluclation for non leap year
  int iL_days = 0;
  switch (  iL_month )
  {
    case 1: iL_days = 0;
            break;
    case 2: iL_days = 31;
            break;
    case 3: iL_days = 59;
            break;
    case 4: iL_days = 90;
            break;
    case 5: iL_days = 120;
            break;
    case 6: iL_days = 151;
            break;
    case 7: iL_days = 181;
            break;
    case 8: iL_days = 212;
            break;
    case 9: iL_days = 243;
            break;
    case 10:iL_days = 273;
            break;
    case 11:iL_days = 304;
            break;
    case 12:iL_days = 334;
            break;
  }
  return iL_days;
}

int CDate::mcfn_calculateDays(CDate &CL_date,int iL_yearRef)
{
  int iL_numOfDays = 0 ,iL_year = 0 ;
  for (iL_year = iL_yearRef; iL_year < CL_date.mcsi_year; iL_year++)
  {
    if(mcfn_isLeap(iL_year))
      iL_numOfDays++;
  }
  if (mcfn_isLeap(iL_year) && CL_date.mcsi_month > 2) 
    iL_numOfDays++;
  iL_numOfDays += mcfn_daysTillMonth(CL_date.mcsi_month) + CL_date.mcsi_day + (CL_date.mcsi_year - iL_yearRef) * 365;
  return iL_numOfDays;
}

int CDate::mcfn_dateDiff(CDate &CL_date)
{
  int  iL_yearRef;
  iL_yearRef = (mcsi_year < CL_date.mcsi_year)? mcsi_year: CL_date.mcsi_year;
  return abs( mcfn_calculateDays( *this, iL_yearRef ) - mcfn_calculateDays( CL_date, iL_yearRef) );
}


CLicenceData::CLicenceData()
{
  mcfn_setMacAddress("eth0");
  mcfn_setTodayDate();
}

CLicenceData::~CLicenceData()
{
}

bool CLicenceData::mcfn_setMacAddress(std::string CL_interfaceName)
{
  struct ifreq s;
  int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

  strcpy(s.ifr_name,CL_interfaceName.c_str());
  if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) 
  {
    char pcL_mac[16] ;
    memset(pcL_mac,0x00,sizeof(pcL_mac));
    for (int i = 0; i < 6; ++i)
      sprintf(pcL_mac,"%s%02x",pcL_mac,(unsigned char) s.ifr_addr.sa_data[i]);
    mcC_mac.assign(pcL_mac);
    return true;
  }
  return false;
}

bool CLicenceData::mcfn_setTodayDate()
{
  char dateString[20]={0x00};
  time_t presentTime = time(NULL);
  struct tm tmp;

  localtime_r(&presentTime,&tmp);
  if (strftime(dateString, sizeof(dateString),"%Y%m%d", &tmp) == 0) {
    fprintf(stderr, "strftime returned 0");
    exit(EXIT_FAILURE);
  }
  mcC_todayDate.assign(dateString);
}


CLicence::CLicence(std::string CL_fileName,int &iL_timeToAlive)
{
  if(false == mcfn_validateLicence(CL_fileName))
  {
    fprintf(stderr, "Invalid/Expired Licence applied ,Please check with support team.\n");
    exit(1);
  }
  else
  {
    CDate  CL_today(meC_licData.mcC_todayDate);
    CDate  CL_expiryTime(meC_licData.mcC_expiryDate);
    int iL_validDays = CL_today.mcfn_dateDiff(CL_expiryTime);
    fprintf(stdout, "Licence is valied for Next %d days .\n",iL_validDays);
    iL_timeToAlive = iL_validDays *24*60*60;
  }
}

CLicence::~CLicence()
{
}

bool CLicence::mcfn_validateLicence(std::string CL_fileName)
{
  std::ifstream CL_licenceFile ( CL_fileName.c_str() );
  std::string CL_licence ="";
  bool bL_retrun = false;
  if (CL_licenceFile.is_open())
  {
    if( getline (CL_licenceFile,CL_licence) )
    {
      bL_retrun = mcfn_validateLicenceString(CL_licence);
    }
    CL_licenceFile.close();
  }
  return bL_retrun;
}

bool CLicence::mcfn_validateLicenceString(std::string CL_licence)
{
  CRsa CL_rsaObj;
  unsigned char pcL_decriptedText[2024]={0x00};
  unsigned char pcL_binaryLicence[2024]={0x00};
  int iL_binaryLen = mefn_convertHexToBinay(CL_licence,pcL_binaryLicence);
  int iL_decResult = CL_rsaObj.mcfn_decriptWithPrivateKey( pcL_binaryLicence, iL_binaryLen, pcL_decriptedText);
  if(iL_decResult != -1)
  {
    return mefn_isValidLicence((char*)pcL_decriptedText);
  }
  return false;
}

int CLicence::mefn_convertHexToBinay(std::string CL_hexData, unsigned char * pcL_binayData)
{
  char hex[3]={0x00,0x00,0x00};
  int len=0;
  for (int i=0;i<CL_hexData.length();i+=2,len++)
  {
    hex[0] = CL_hexData[i];
    hex[1] = CL_hexData[i+1];
    pcL_binayData[len]=(strtol(hex,NULL,16));
  }
  return len;
}

bool CLicence:: mefn_isValidLicence (std::string CL_licence)
{
  char  pcL_mac[20]={0x00};
  char  pcL_expiryTime[20]={0x00};
  sscanf((char*)CL_licence.c_str(),"%12c%8c",pcL_mac,pcL_expiryTime);

  if( meC_licData.mcC_mac.compare( pcL_mac ) == 0 &&
      atoi( meC_licData.mcC_todayDate.c_str() ) <= atoi(pcL_expiryTime) )
  {
    meC_licData.mcC_expiryDate.assign(pcL_expiryTime);
    return true;
  }
  return false;
}
