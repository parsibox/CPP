#include<HttpServer.h>
bool  getFunction(HttpRequest httpMsg,HttpResponse& httpResp)
{
  printf("+===================================================+\n");
  printf("+===================================================+\n");
  printf("+===================================================+\n");
  printf("+===================================================+\n");
  printf("+===================================================+\n");
  httpResp.respBody="<html><body><p>get request reched to server</p><body></html>";
  return true;
}
int main()
{

	HttpServer Server(9898);
        std::map<int,std::string> maps;
        maps[100] = "surya";
        Server.registerService("GET","/surya",&getFunction);
	while(1)
	{
		sleep(100);
	}
	return 0;

}
