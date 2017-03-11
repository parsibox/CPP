#include<stdio.h>
#include<string.h>
#include<stdlib.h>
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
}
extern "C" void fnG_GetTable(char* filename,char* TableName,char delimiter)
{
	//MTSMSCCDR_2012_11_28_12_14_30_87_1.TXT
	char pscL_tablehour[10],TablePrefix[]="CDR";
	memset(pscL_tablehour,0x00,sizeof(pscL_tablehour));
	int day=0,hour=0;//month=0;
	//month = (atoi)(fnG_getFields(filename,delimiter,2));
	day   = (atoi)(fnG_getFields(filename,delimiter,3));
	hour  = (atoi)(fnG_getFields(filename,delimiter,4));

	if(hour >= 12)
		strcpy(pscL_tablehour,"12_23");
	else
		strcpy(pscL_tablehour,"00_12");
	
	int length = sprintf(TableName,"%s_%02d_%s",TablePrefix,day,pscL_tablehour);
	TableName[length]='\0';
}
/*int main(int argc,char** argv)
{
	char filename[200]={0x00};
	strcpy(filename,argv[1]);
	char tablename[50];
	fnG_GetTable(filename,tablename,'_');
	printf("%s\n",tablename);
}*/
