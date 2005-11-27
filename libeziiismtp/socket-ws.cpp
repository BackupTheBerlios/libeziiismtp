// libEziiiSMTP
//
// winsocks / libEziiiSMTP adapter :)
//
// Author : neiro 27-11-05
// Licence / Documentation : see setup.h

#include "socket-ws.h"
#include <stdio.h>

lESMTPsocket::lESMTPsocket()
{
	int wsaserr = WSAStartup(MAKEWORD(2, 2) , &m_wsad);
	if (wsaserr) m_sock = INVALID_SOCKET;
	else m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//if (m_sock == INVALID_SOCKET) return;
}

lESMTPsocket::~lESMTPsocket()
{
	WSACleanup();
}

char *
lESMTPsocket::GetHostName()
{
	char m_hostname[128];
	gethostname(m_hostname, 128);
	if (strlen(m_hostname) == 0) return "unknown_hostname";
	return strdup(m_hostname);
}

bool
lESMTPsocket::OpenConnection(char *hostname, unsigned short int port, unsigned short int timeout)
{
	struct hostent *m_host;
	unsigned int m_addr;
	struct sockaddr_in m_server;

	m_addr = inet_addr(hostname);
	if (m_addr == INADDR_NONE)
		m_host = gethostbyname(hostname);
	else
		m_host = gethostbyaddr((char*)&m_addr, sizeof(m_addr), AF_INET);

	m_server.sin_addr.S_un.S_addr = inet_addr(m_host->h_addr);
	m_server.sin_family = AF_INET;
	m_server.sin_port = htons(port);
	if (connect(m_sock, (struct sockaddr*)&m_server, sizeof(m_server)))
	{
		closesocket(m_sock);
		return false;	
	}
	return true;
}

ServerAnswer *
lESMTPsocket::OpenWelcomeConnection(char *hostname, unsigned short int port, unsigned short int timeout)
{
	struct hostent *m_host;
	unsigned int m_addr;
	struct sockaddr_in m_server;

	m_addr = inet_addr(hostname);
	if (m_addr == INADDR_NONE)
		m_host = gethostbyname(hostname);
	else
		m_host = gethostbyaddr((char*)&m_addr, sizeof(m_addr), AF_INET);

	memcpy(&m_server.sin_addr, m_host->h_addr_list[0], m_host->h_length);
	m_server.sin_family = AF_INET;
	m_server.sin_port = htons(port);
	if (connect(m_sock, (struct sockaddr*)&m_server, sizeof(m_server)))
	{
		closesocket(m_sock);
		return NULL;	
	}
	ServerAnswer *m_CA = new ServerAnswer;
	m_CA->message = new char[1024];
	m_CA->code = 0;
	Read(m_CA->message, 1024);
	sscanf(m_CA->message, "%hu", &(m_CA->code));
	return m_CA;
}

bool
lESMTPsocket::Read(void *buffer, unsigned int nbytes)
{
	return recv(m_sock, (char *)buffer, nbytes, 0);
}

ServerAnswer *
lESMTPsocket::SendCommand(char *command)
{
	Write(command, strlen(command));
	ServerAnswer *m_CA = new ServerAnswer;
	m_CA->message = new char[1024];
	m_CA->code = 0;
	Read(m_CA->message, 1024);
	sscanf(m_CA->message, "%hu", &(m_CA->code));
	return m_CA;
}

bool
lESMTPsocket::Write(const void *buffer, unsigned int nbytes)
{
	return send(m_sock, (char *)buffer, nbytes, 0);
}

bool
lESMTPsocket::CloseConnection()
{
	return !closesocket(m_sock);
}

void
lESMTPsocket::FreeAnswer(ServerAnswer *m_sa)
{
	if (m_sa == NULL) return;
	if (m_sa->message != NULL) delete m_sa->message;
	delete m_sa;
}