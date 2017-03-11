#ifndef __LICENCE__H
#define __LICENCE__H
#include<string>

class CDate
{
  public:
    int mcsi_day;
    int mcsi_month;
    int mcsi_year;

  public:

    CDate(int day,int month,int year);
    CDate(std::string CL_date);
    ~CDate();

    bool mcfn_isLeap( int iL_year );
    int mcfn_daysTillMonth( int iL_month );
    int mcfn_calculateDays(CDate &CL_date,int iL_yearRef);
    int mcfn_dateDiff(CDate &CL_date);

};



class CLicenceData
{
  public:

    std::string mcC_mac;
    std::string mcC_todayDate;
    std::string mcC_expiryDate;

  public:

    CLicenceData();
    ~CLicenceData();

    bool mcfn_setMacAddress(std::string CL_interface);
    bool mcfn_setTodayDate();
};


class CLicence
{
  CLicenceData meC_licData;
  public:

    CLicence(std::string CL_fileName,int &iL_timeToAlive);
    ~CLicence();
    bool mcfn_validateLicence(std::string CL_fileName);
    bool mcfn_validateLicenceString(std::string CL_licence);
    int mefn_convertHexToBinay(std::string CL_asciiData, unsigned char * pcL_binayData);
    bool mefn_isValidLicence(std::string CL_licence);
};

#endif
