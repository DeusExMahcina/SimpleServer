#include "ServerInit.hxx"
#include "ErrorHandler.hxx"

BOOL TestPort( const SOCKET Sock, const WORD wPort )
{
	sockaddr_in clientService;
    clientService.sin_family = AF_INET;
	/*we will connect to the targtet port on localhost to check if someone is listening. if someone does, the port is used*/
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(wPort);
	if(0 == connect(Sock,(SOCKADDR *) &clientService,sizeof(clientService)))
	{
		return TRUE;
	}//if
	return FALSE;
}//IsPortUsed

BOOL InitializeServer ( const PCHAR port )
{
	struct addrinfo * result = NULL;
	struct addrinfo hints;
	
	
	WSADATA wsData;
	SOCKET ServerSocket = INVALID_SOCKET;	//the main server socket
	SOCKET TestPortSock = INVALID_SOCKET;	//a socket to check if the port is being used


	/*initializing the parameters that would be later needed to create the socket*/
	ZeroMemory(&hints,sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	/*initializing the winsock dll*/
	if(WSAStartup(MAKEWORD(2,2) , &wsData) != 0){
		WSACleanup();
		return FALSE;
	}
	if(getaddrinfo( NULL, port , &hints, &result ) != 0){
		SetLocalError( ERR_INIT ) ;
		free(result);
		return FALSE;
	}
	/*creating both sockets, one for the actual connection and the other for testing*/
	ServerSocket = socket(result->ai_family,result->ai_socktype,result->ai_protocol);
	TestPortSock = socket(result->ai_family,result->ai_socktype,result->ai_protocol);
	if( TestPort( TestPortSock , atoi( port ) ) ){
		SetLocalError(ERR_INIT_PORT_USED);
		freeaddrinfo(result);
		WSACleanup();
		return FALSE;
	}
	if(INVALID_SOCKET == ServerSocket){
		freeaddrinfo(result);
		WSACleanup();
		return FALSE;
	}
	if( SOCKET_ERROR == bind( ServerSocket, result->ai_addr,(int)result->ai_addrlen ) ) {
		SetLocalError(ERR_INIT);
		freeaddrinfo(result);
		WSACleanup();
		return FALSE;
	}
	if( SOCKET_ERROR == listen( ServerSocket,SOMAXCONN ) ) {
		SetLocalError(ERR_INIT);
		WSACleanup();
		freeaddrinfo(result);
		return FALSE;
	}
	WSACleanup();
	freeaddrinfo(result);
	return TRUE;
}