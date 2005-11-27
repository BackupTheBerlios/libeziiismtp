#ifndef _LIBEZIIISMTPSOCKET_H
#define _LIBEZIIISMTPSOCKET_H

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma implementation
    #pragma interface
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#  pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif

#include "wx/socket.h"
#include "wx/wfstream.h"

struct ServerAnswer {
	unsigned short int code;
	wxChar *message;
};

class lESMTPsocket
{
public:
	lESMTPsocket();
	~lESMTPsocket();
	wxChar *GetHostName();
	bool OpenConnection(wxChar *hostname, unsigned short int port, unsigned short int timeout);
	ServerAnswer *OpenWelcomeConnection(wxChar *hostname, unsigned short int port, unsigned short int timeout);
	bool Read(void *buffer, wxUint32 nbytes);
	ServerAnswer *SendCommand(wxChar *command);
	bool Write(const void *buffer, wxUint32 nbytes);
	bool CloseConnection();
	void FreeAnswer(ServerAnswer *m_sa);
private:
	wxSocketClient *m_sock;
};

#endif //_LIBEZIIISMTPSOCKET_H