#ifndef _LIBEZIIISMTPSOCKET_H
#define _LIBEZIIISMTPSOCKET_H

#include <winsock.h>

struct ServerAnswer {
	unsigned short int code;
	char *message;
};

class lESMTPsocket
{
public:
	lESMTPsocket();
	~lESMTPsocket();
	char *GetHostName();
	bool OpenConnection(char *hostname, unsigned short int port, unsigned short int timeout);
	ServerAnswer *OpenWelcomeConnection(char *hostname, unsigned short int port, unsigned short int timeout);
	bool Read(void *buffer, unsigned int nbytes);
	ServerAnswer *SendCommand(char *command);
	bool Write(const void *buffer, unsigned int nbytes);
	bool CloseConnection();
	void FreeAnswer(ServerAnswer *m_sa);
private:
	WSADATA m_wsad;
	SOCKET m_sock;
};

#endif //_LIBEZIIISMTPSOCKET_H