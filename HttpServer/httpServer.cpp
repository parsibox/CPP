#ifdef _MSC_VER
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include <mongoose/WebController.h>

using namespace std;
using namespace Mongoose;

class MyController : public WebController
{
    public: 
        void hello(Request &request, StreamResponse &response)
        {
            response << "Hello " << htmlEntities(request.get("name", "... what's your name ?")) << endl;
        }
        void service(Request &request, StreamResponse &response){
		cout<<"Recevied MSISDN :"<<request.get("MSISDN","")<<endl;
		response << "0";
	}

        void setup()
        {
            addRoute("GET", "/hello", MyController, hello);
            addRoute("POST", "/hello", MyController, hello);
	    addRoute("POST", "/service", MyController, service);
            addRoute("GET", "/service", MyController, service);
        }
};


int main()
{
    MyController myController;
    Server server(8080);
    server.registerController(&myController);

    server.start(); 

    while (1) {
#ifdef WIN32
		Sleep(10000);
#else
        sleep(10);
#endif
    }
}
