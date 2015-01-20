#include "HTTPConnect.h"

namespace TCPServer
{

HTTPConnect::HTTPConnect()
{
	char * Port = "8080";
	pSelfHTTPInfo = SelfHTTPInfo (Port);
	MaxConnectSocketCount = 5;
	LinkCounter = 0;
}

HTTPConnect::HTTPConnect (char *PortCharNumber)
{
	pSelfHTTPInfo = SelfHTTPInfo (PortCharNumber);
	MaxConnectSocketCount = 5;
	LinkCounter = 0;
}

HTTPConnect::~HTTPConnect()
{
}

ADDR_INFO* TCPServer::HTTPConnect::SelfHTTPInfo(char* PortCharNumber)
{
  int status;
  struct addrinfo *SelfHTTPInfo, HttpInputInfo;
  
  memset(&HttpInputInfo, 0, sizeof(HttpInputInfo));
  
  HttpInputInfo.ai_family = AF_INET;     // IPv4  也可以用AF_UNSPEC
  HttpInputInfo.ai_flags = AI_PASSIVE;   // 將本機的位址（address of local host）指定給 socket structure
  HttpInputInfo.ai_socktype = SOCK_STREAM; // TCP stream sockets
  
  // 用來解析DNS, 如果是本地端Server, 用NULL 就代表解析自己
  status = getaddrinfo(NULL, PortCharNumber, &HttpInputInfo, &SelfHTTPInfo);

  if (status != 0) {
	printf("getaddrinfo : %s\n", gai_strerror(status));
	return NULL;
  }  
  
  return  SelfHTTPInfo;
}

STATUS TCPServer::HTTPConnect::CreateRootPortSocket()
{
    int yes = 1;
	
    SockHttpRoot = socket(pSelfHTTPInfo->ai_family, pSelfHTTPInfo->ai_socktype, pSelfHTTPInfo->ai_protocol); 
    
    if (SockHttpRoot <0) {
		printf("SockRoot error\n");
		return -1;
	}
    
    // 設定Socket, 
    // SO_REUSEADDR	 除非已經有 listening socket 綁定到這個 port，不然可以允許其它的 sockets bind() 
    // 這個 port。這樣可以讓你在 server 當機之後，重新啟動 server 時不會遇到 "Address already in use（位址已經在使用中）" 
    // 這個錯誤訊息。
    setsockopt (SockHttpRoot, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

    return 0;
}

STATUS TCPServer::HTTPConnect::Bind()
{
	return bind(SockHttpRoot, pSelfHTTPInfo->ai_addr, pSelfHTTPInfo->ai_addrlen);
}

STATUS TCPServer::HTTPConnect::Listen()
{
	return listen (SockHttpRoot, MaxConnectSocketCount);
}


STATUS TCPServer::HTTPConnect::Close(int ConnectSock)
{
	close (ConnectSock);
}

int TCPServer::HTTPConnect::AcceptConnectClient()
{
    int ClientSocket;
    int ConnectSock;
    struct sockaddr_in client;
    socklen_t len;
    
    len = sizeof(client);
	ClientSocket = accept(SockHttpRoot, (struct sockaddr *)&client, &len);
	if (ClientSocket < 0) {
	   printf ("accept\n");
	   return -1;
	}

	LinkCounter++;
    return ClientSocket;
}

void TCPServer::HTTPConnect::ResolveConnectInfo(char* InputDataBuf, unsigned int Length)
{
	printf ("%s\n", InputDataBuf);
}

void TCPServer::HTTPConnect::ReadConnectInfo(int TargetSocketConnet)
{
	int InfoLength;
    char InputBuf[2048];
   	InfoLength = read(TargetSocketConnet, InputBuf, sizeof(InputBuf)); 
    ResolveConnectInfo (InputBuf, InfoLength);
}


}


