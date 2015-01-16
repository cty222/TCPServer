#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct addrinfo ADDR_INFO;
// 80 Port  是系統在使用 1024 以下都是系統保留
// 要用router
char * HttpPortStr = "8080"; 

ADDR_INFO * 
SelfHTTPInfo 
(
    char *PortCharNumber
)
{
  int status;
  struct addrinfo *pSelfHTTPInfo, HttpInputInfo;
  
  memset(&HttpInputInfo, 0, sizeof(HttpInputInfo));
  
  HttpInputInfo.ai_family = AF_INET;     // IPv4  也可以用AF_UNSPEC
  HttpInputInfo.ai_flags = AI_PASSIVE;   // 將本機的位址（address of local host）指定給 socket structure
  HttpInputInfo.ai_socktype = SOCK_STREAM; // TCP stream sockets
  
  // 用來解析DNS, 如果是本地端Server, 用NULL 就代表解析自己
  status = getaddrinfo(NULL, PortCharNumber, &HttpInputInfo, &pSelfHTTPInfo);

  if (status != 0) {
	printf("getaddrinfo : %s\n", gai_strerror(status));
	return NULL;
  }  
  
  return  pSelfHTTPInfo;
}

int 
CreateRootPortSocket 
(
    ADDR_INFO *pSelfHTTPInfo
)
{
    int SockRoot;
    int yes = 1;
    
    SockRoot = socket(pSelfHTTPInfo->ai_family, pSelfHTTPInfo->ai_socktype, pSelfHTTPInfo->ai_protocol); 
    
    if (SockRoot <0) {
		printf("SockRoot error\n");
		return -1;
	}
    
    // 設定Socket, 
    // SO_REUSEADDR	 除非已經有 listening socket 綁定到這個 port，不然可以允許其它的 sockets bind() 
    // 這個 port。這樣可以讓你在 server 當機之後，重新啟動 server 時不會遇到 "Address already in use（位址已經在使用中）" 
    // 這個錯誤訊息。
    
    setsockopt (SockRoot, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));
    
    return SockRoot;
}

int 
AcceptConnectClient 
(
    int RootSocket
)
{
    int ClientSocket;
    int ConnectSock;
    struct sockaddr_in client;
    socklen_t len;
    
    len = sizeof(client);
	ClientSocket = accept(RootSocket, (struct sockaddr *)&client, &len);
	if (ClientSocket < 0) {
	   printf ("accept\n");
	   return -1;
	}

    return ClientSocket;
}

int ResolveConnectInfo
(
    char * InputDataBuf,
    unsigned int Length
)
{
    printf ("%s\n", InputDataBuf);
}

int ReadConnectInfo 
( 
    int TargetSocketConnet 
)
{
	int InfoLength;
    char InputBuf[2048];
   	InfoLength = read(TargetSocketConnet, InputBuf, sizeof(InputBuf)); 
    ResolveConnectInfo (InputBuf, InfoLength);
}

int
main()
{
    struct addrinfo *pSelfHTTPInfo;
    int SockHttpRoot;
    
    // 不確定
/*    

	char buf[2048];


	int linkCount = 0;
	int check = 0;        */
    
    pSelfHTTPInfo = SelfHTTPInfo (HttpPortStr);
    if (pSelfHTTPInfo == NULL)
    {
        printf("SelfHttpInfo () Failed");
        return -2;
    } 

	SockHttpRoot = CreateRootPortSocket (pSelfHTTPInfo);
    if (SockHttpRoot < 0)
    {
        printf("CreateRootPortSocket failed.");
        return -3;
    }

    // 將它綁定到我們於 getaddrinfo() 中傳遞的 port：
    if(bind(SockHttpRoot, pSelfHTTPInfo->ai_addr, pSelfHTTPInfo->ai_addrlen) != 0) {
		printf ("Bind error\n");
		return 1;
	}
 
    // 將 SockHttpRoot 設定為 server（監聽的）的 socket
    // 參數 backlog 代表在 kernel 開始拒絕新連線以前，你可以有多少的連線在等待。
    // 這裡設為5, Accrpt後 該連線就不算了.
    if(listen (SockHttpRoot, 5) != 0) {
		printf ("listen Error\n");
		return 1;
    }

    int ConnectSock;
	while (1) {
        ConnectSock = AcceptConnectClient (SockHttpRoot);
        if (ConnectSock < 0)
        {
            printf ("Accept Connect failed\n");
            continue;
        }

        ReadConnectInfo (ConnectSock);

		//write (fileno (stdout), inbuf, n);
		/*check = 0;
		while (1) {
			if (inbuf[check] == 'G') {
				if(inbuf[check + 4] == '/' ) {
					if(inbuf[check + 5] == ' ' || inbuf[check + 5] == '?') {
				        linkCount++;

    				    snprintf (buf, sizeof(buf),
            				"HTTP/1.1 200 OK\r\n"
            				"Content-Type: text/html\r\n"
            				"\r\n"
            				"<html>\r\n"
            				"<form>\r\n"
            				"<input  type='submit' value='Count = %d'/>\r\n"
            				"</html\r\n>"
            				, linkCount);

						write (sock, buf, (int)strlen(buf));
					}
					else if(inbuf[check + 5] == 'f') {
						printf ("No favicon.ico\n");
					}
					else {
						printf ("No this file\n");
					}
					break; 
				}
			}
			else if (inbuf[check] == '\r') {
				break;
			}
			else {
				check++;
			}
		}      */

		close (ConnectSock);
	}

    close (SockHttpRoot);

	return 0; 
}
