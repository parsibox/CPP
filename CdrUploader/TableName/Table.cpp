#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/**
  char* fnG_getFields(const char* pscL_string,char scL_Delimiter,int count)
  {
  if(pscL_string == NULL)
  return "\0";
  char a[200]={0x00};
  strcpy(a,pscL_string);
  char *temp = (char*)a;
  int i;
  for(i=0;i<count;){
  if((*temp++) == scL_Delimiter) ++i;
  if(*temp == '\0') break;
  }
  if(i != count)
  return "\0";
  char* pscL_Temp = (char*)temp;
  while(*temp++ != scL_Delimiter);
  temp--;
 *temp = '\0';
 return pscL_Temp;
 }*/
const char* fnG_getFields(const char* pscL_string,char scL_Delimiter,int count)
{
	if(pscL_string == NULL)
		return "\0";
	int i=0;
	for(i=0;pscL_string[i]!='\0';i++){
		if(pscL_string[i]==scL_Delimiter){
			count--;
			if(count<=0){
				i++;
				break;
			}
		}
	}
	if(count>0)
		return "\0";
	//printf("FileName:%s\n",pscL_string);
	return pscL_string+i;
}

extern "C" void fnG_GetTable(char* filename,char* TableName,char delimiter)
{
	//MTSMSCCDR_2012_11_28_12_14_30_87_1.TXT
	char pscL_tablehour[10],TablePrefix[]="CDR";
	memset(pscL_tablehour,0x00,sizeof(pscL_tablehour));
	int day=0,hour=0;
	day   = atoi(fnG_getFields(filename,delimiter,3));
	hour  = atoi(fnG_getFields(filename,delimiter,4));
	if(hour >= 12)
		strcpy(pscL_tablehour,"12_23");
	else
		strcpy(pscL_tablehour,"00_12");

	int length = sprintf(TableName,"%s_%02d_%s",TablePrefix,day,pscL_tablehour);
	TableName[length]='\0';
}

extern "C" void fnG_getSMSCTable(char* filename,char* TableName,char delimiter)
{
	char pscL_Retry[6]={0x00};
	char pscL_postFix[5]={0x00};
	char Table[6]="SMSC";
	strncpy(pscL_Retry,fnG_getFields(filename,delimiter,2),3);
	strncpy(pscL_postFix,fnG_getFields(filename,delimiter,1),2);
	int length = sprintf(TableName,"%s_%s_%s",pscL_Retry,Table,pscL_postFix);
	TableName[length]='\0';
}

extern "C" void fnG_getConcatTableName(char* filename,char* TableName,char delimiter)
{
	char pscL_postFix[5]={0x00};
	char Table[6]="SMSC";
	strncpy(pscL_postFix,fnG_getFields(filename,delimiter,4),2);
	int length = sprintf(TableName,"003_%s_%s",Table,pscL_postFix);
	TableName[length]='\0';
}

extern "C" void fnG_getCDRTable(char* filename,char* TableName,char delimiter)
{
	//DELIVER_2013_02_04_19_56_51.TXT
	int day=0,hour=0;//month=0;
	char pscL_tablehour[10],TablePrefix[]="CDR";
	memset(pscL_tablehour,0x00,sizeof(pscL_tablehour));
	hour  = (atoi)(fnG_getFields(filename,delimiter,4));
	day   = (atoi)(fnG_getFields(filename,delimiter,3));
	if(hour >= 12)
		strcpy(pscL_tablehour,"12_23");
	else
		strcpy(pscL_tablehour,"00_12");
	int length = sprintf(TableName,"%s_%02d_%s",TablePrefix,day,pscL_tablehour);
	TableName[length]='\0';
}

extern "C" void fnG_Table(char* _filename,char* TableName,char delimiter)
{
	char filename[10]={0x00};
	strncpy(filename,_filename,4);
	filename[4]='\0';
	//printf("%s\n",filename);
	if(strcmp(filename,"BIND") == 0)
		strcpy(TableName,"AUTHENTICATION");
	else //if(strcmp(filename,"SUBMIT")==0)
		strcpy(TableName,"SUBMIT");
}

extern "C" void fnG_getCMSCDRTable(char* filename,char* TableName,char delimiter)
{
	//SUBMITSM_2013_03_22_10_40_33_003_0001.TXT
	//SUB_CDR_22_00_12_03 
	int day=0,hour=0,month=0;
	char pscL_tablehour[10],TablePrefix[]="SUB_CDR";
	memset(pscL_tablehour,0x00,sizeof(pscL_tablehour));
	hour  = (atoi)(fnG_getFields(filename,delimiter,4));
	day   = (atoi)(fnG_getFields(filename,delimiter,3));
	month = (atoi)(fnG_getFields(filename,delimiter,2));
	if(hour >= 12)
		strcpy(pscL_tablehour,"12_23");
	else
		strcpy(pscL_tablehour,"00_12");
	int length = sprintf(TableName,"%s_%02d_%s_%02d",TablePrefix,day,pscL_tablehour,(month%4==0)?4:month%4);
	TableName[length]='\0';
}

extern "C" void fnG_GetSMSCCDRTable(char* filename,char* TableName,char delimiter)
{
	//MTSMSCCDR_2012_11_28_12_14_30_87_1.TXT
	//CDR_20130326120100_1.TXT
	char pscL_tablehour[10],TablePrefix[]="CDR";
	memset(pscL_tablehour,0x00,sizeof(pscL_tablehour));
	int day=0,hour=0,year=0,month=0;
	sscanf(fnG_getFields(filename,delimiter,1),"%4d%2d%2d%2d%*s",&year,&month,&day,&hour);
	if(hour >= 12)
		strcpy(pscL_tablehour,"12_23");
	else
		strcpy(pscL_tablehour,"00_12");

	int length = sprintf(TableName,"%s_%02d_%s",TablePrefix,day,pscL_tablehour);
	TableName[length]='\0';
}

extern "C" void fnG_GetUSSDCDRTable(char* filename,char* TableName,char delimiter)
{
	//USSDSUBMIT_2015_02_26_20_38_44_5.TXT
	int month=0,day=0;
	month  = atoi(fnG_getFields(filename,delimiter,2));
	day   = atoi(fnG_getFields(filename,delimiter,3));

	int length =sprintf(TableName,"MDR_%02d_%02d",month%2,day);
	TableName[length]='\0';
}

extern "C" void fnG_GetCsvTableName(char* filename,char* TableName,char delimiter)
{
	strcpy(TableName,"csv_table");
}

/*
int main(int argc,char** argv)
{
	char filename[200]={0x00};
	strcpy(filename,argv[1]);
	char tablename[50];
	fnG_GetUSSDCDRTable(filename,tablename,'_');
	printf("%s\n",tablename);
}*/
