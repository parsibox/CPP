#ifndef __RSA__H
#define __RSA__H
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <string>
class CRsa
{
  private:
        std::string meC_publicKey;
        std::string meC_privateKey;
        int mesi_padding ;
  private:
       RSA* mefn_createRSA(unsigned char * key,bool isPublic);
  public:
        CRsa();
        ~CRsa();
       int mcfn_encriptWithPublicKey(unsigned char * data,int data_len, unsigned char *encrypted);
       int mcfn_encriptWithPrivateKey(unsigned char * data,int data_len, unsigned char *encrypted);
       int mcfn_decriptWithPublicKey(unsigned char * enc_data,int data_len,unsigned char *decrypted);
       int mcfn_decriptWithPrivateKey(unsigned char * enc_data,int data_len,unsigned char *decrypted);
};
#endif
