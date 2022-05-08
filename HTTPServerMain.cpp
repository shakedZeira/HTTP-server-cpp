#include "WebServer.h"
void main()
{
	WebServer webserver("127.0.0.1", 4200);
	if (webserver.init() != 0)
	{
		return;
	}
	webserver.run();
	system("pause");
}














