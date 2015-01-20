#include <iostream>
#include "HTTPConnect.h"

// 80 Port  是系統在使用 1024 以下都是系統保留
// 要用router
char * HttpPortStr = "8080"; 

using namespace std;
using namespace TCPServer;

int
main()
{

/*    
	char buf[2048];
	 = 0;
	int check = 0;        */
	
    HTTPConnect RootConnect (HttpPortStr); 
    if (RootConnect.pSelfHTTPInfo == NULL)
    {
        printf("SelfHttpInfo () Failed\n");
        return -1;
    } 

    RootConnect.CreateRootPortSocket ();
    if (RootConnect.SockHttpRoot < 0)
    {
        printf("CreateRootPortSocket failed.\n");
        return -1;
    }

    // 將它綁定到我們於 getaddrinfo() 中傳遞的 port：
    if(RootConnect.Bind() != 0) {
		printf ("Bind error\n");
		return 1;
	}
 
    // 將 SockHttpRoot 設定為 server（監聽的）的 socket
    // 參數 backlog 代表在 kernel 開始拒絕新連線以前，你可以有多少的連線在等待。
    // 這裡設為5, Accrpt後 該連線就不算了.
    if(RootConnect.Listen() != 0) {
		printf ("listen Error\n");
		return 1;
    }

    int ConnectSock;
	while (1) {
        ConnectSock = RootConnect.AcceptConnectClient ();
        if (ConnectSock < 0)
        {
            printf ("Accept Connect failed\n");
            continue;
        }

        RootConnect.ReadConnectInfo (ConnectSock);

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

		RootConnect.Close (ConnectSock);
	}

    close (RootConnect.SockHttpRoot);

	return 0; 
}
