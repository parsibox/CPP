#include<stdio.h>
#include<Rsa.h>
#include<Licence.h>
int main()
{
        CRsa rsaObj;
        std::string testMessage= "3c4a92f682f820150528";
        unsigned char result[2024]={"0x00"};
        int resultLen = rsaObj.mcfn_encriptWithPublicKey((unsigned char*)testMessage.c_str(),testMessage.length(),result);
        printf("encripetd Length:%d\n",resultLen);
        for(int i=0;i<resultLen;i++)
                printf("%02x",result[i]);
        printf("\n");
        unsigned char decript[2024]={0x00};
        int decResult= rsaObj.mcfn_decriptWithPrivateKey(result,resultLen,decript);
        printf("Decripetd Result[%d]:%s\n",decResult,decript);
        int iL_timeToLive =0 ;
        CLicence CL_licence("./Licence.txt",iL_timeToLive);
        sleep(10);
        return 0;
}
