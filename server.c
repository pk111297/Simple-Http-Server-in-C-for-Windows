#include<winsock2.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
//#pragma comment(lib, "Ws2_32.lib")
int main()
{
//initialization
WSADATA wsaData;
int iResult;
// Initialize Winsock
if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0) 
{
printf("WSAStartup failed with Error %d!\n",WSAGetLastError());
WSACleanup();
exit(1);
}
//server
int sock,connected,bytes_recieved,true = 1;  
char send_data [1024],recv_data[1024];
//creating socket
if ((sock=socket(AF_INET, SOCK_STREAM,0))==-1) 
{
printf("Socket failed with Error %d!\n",WSAGetLastError());
WSACleanup();
exit(1);
}
//setting socket options
if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int))==-1) 
{
printf("Setsockopt failed with Error %d!\n",WSAGetLastError());
WSACleanup();
exit(1);
}
struct sockaddr_in server_addr;
server_addr.sin_family = AF_INET;         
server_addr.sin_port = htons(5600);     
server_addr.sin_addr.s_addr = INADDR_ANY; 
memset(&(server_addr.sin_zero),0,8);
//binding the socket
if(bind(sock,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))==-1) 
{
printf("Unable to bind failed with Error %d!\n",WSAGetLastError());
WSACleanup();
exit(1);
}
if(listen(sock,5)==-1) 
{
printf("Listening failed with Error %d!\n",WSAGetLastError());
WSACleanup();
exit(1);
}
//Looping starts
while(1)
{
printf("HTTP Server waiting on port 5600\n");
fflush(stdout);
int sin_size = sizeof(struct sockaddr_in);
struct sockaddr_in client_addr;
connected = accept(sock,(struct sockaddr *)&client_addr,&sin_size);
if(connected<=0)
{
printf("Listening failed with Error %d!\n",WSAGetLastError());
continue;
}
printf("Connection from (%s , %d)\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
char requestString[9999];
recv(connected,requestString,sizeof(requestString),0);
//printf("Received:%s",requestString);
if(strlen(requestString)==0)
{
printf("Request string Received is with length=0\n");
continue;
}
//Received String Ko Todna hai
char requestType[10];
char requestService[255];
sscanf(requestString,"%s %s",requestType,requestService);
printf("%s %s\n",requestType,requestService);
char *fileName;
if(strcmp(requestService,"/")==0)
{
char temp[]="index.html";
fileName=temp;
}
else
{
fileName=strchr(requestService,'/');
++fileName;
}
printf("%s\n",fileName);
//Checking whether a file exists in the current directory or not
if(access(fileName,F_OK)!=-1) 
{
// file exists
char *extension = strchr(fileName, '.');
char sendResponse[9999];
if(!strcmp(extension,".html")) sprintf(sendResponse,"HTTP/1.1 200 OK\r\nContent-Type: %s; charset=UTF-8\r\n\r\n","text/html");
if(!strcmp(extension,".ico")) sprintf(sendResponse,"HTTP/1.1 200 OK\r\nContent-Type: %s; charset=UTF-8\r\n\r\n","image/webp");
if(!strcmp(extension,".css")) sprintf(sendResponse,"HTTP/1.1 200 OK\r\nContent-Type: %s; charset=UTF-8\r\n\r\n","text/css");
if(!strcmp(extension,".jpg")) sprintf(sendResponse,"HTTP/1.1 200 OK\r\nContent-Type: %s; charset=UTF-8\r\n\r\n","image/jpeg");
if(!strcmp(extension,".js")) sprintf(sendResponse,"HTTP/1.1 200 OK\r\nContent-Type: %s; charset=UTF-8\r\n\r\n","text/javascript"); 
if(!strcmp(extension,".png")) sprintf(sendResponse,"HTTP/1.1 200 OK\r\nContent-Type: %s; charset=UTF-8\r\n\r\n","image/png");
FILE *file=fopen(fileName,"rb");
if(!file)
{
send(connected,"HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n",57,0);
continue;
}
char sendBuffer[2048];
send(connected,sendResponse,strlen(sendResponse),0);
while(fread(sendBuffer,1,1024,file)>0)
{
int c=send(connected,sendBuffer,strlen(sendBuffer),0);
if(c<=0)
{
printf("Error in Sending!\n");
//closesocket(connected);
break;
}
printf("STATUS:%d\n",c);
}
} 
else 
{
// file doesn't exist
printf("Send the 404 Error\n");
char sendResponse[9999]="HTTP/1.1 404 404 Not Found\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title>404 Not Found</title></head><body><h1>Not Found</h1>The requested URL was not found on this server.</body></html>";
//printf("%s",sendResponse);
int c=send(connected,&sendResponse,sizeof(sendResponse),0);
printf("STATUS:%d\n",c);
//printf("\nSent : %s\n",sendResponse);
}
closesocket(connected);
}
close(sock);
WSACleanup();
return 0;
}
