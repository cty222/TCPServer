#ifndef HTTPCONNECT_H
#define HTTPCONNECT_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "GlobalDefine.h"

typedef struct addrinfo ADDR_INFO;
typedef int SOCKET_NUMBER;

namespace TCPServer
{

class HTTPConnect
{
	private:
	ADDR_INFO * SelfHTTPInfo ( char *PortCharNumber);

	public:
	HTTPConnect();
	HTTPConnect( char *PortCharNumber);
	~HTTPConnect();
	
	// Function
	STATUS CreateRootPortSocket ();
	STATUS Bind ();
	STATUS Listen ();
	STATUS Close (int ConnectSock);

	SOCKET_NUMBER AcceptConnectClient ();
    void ResolveConnectInfo ( char * InputDataBuf,   unsigned int Length);
    void ReadConnectInfo ( int TargetSocketConnet);
		
	SOCKET_NUMBER SockHttpRoot;
	ADDR_INFO *pSelfHTTPInfo;
	unsigned int MaxConnectSocketCount;
	int LinkCounter;
};

}

#endif // HTTPCONNECT_H
