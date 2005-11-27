// libEziiiSMTP
//
// wxWidgets / libEziiiSMTP adapter :)      tested with wxWidgets 2.6.2
//
// Author : neiro 27-11-05
// Licence / Documentation : see setup.h

#include "socket-wx.h"

lESMTPsocket::lESMTPsocket()
{
	m_sock = new wxSocketClient();
	m_sock->Notify(false);
}

lESMTPsocket::~lESMTPsocket()
{
	delete m_sock;
}

wxChar *
lESMTPsocket::GetHostName()
{
	wxString m_hostname = wxGetHostName();
	if (m_hostname.IsEmpty()) return "unknown_hostname";
	wxChar *mc_hostname = strdup(m_hostname.wx_str());
	m_hostname.Clear();
	//delete m_hostname;
	return mc_hostname;
}

bool
lESMTPsocket::OpenConnection(wxChar *hostname, unsigned short int port, unsigned short int timeout)
{
	wxIPV4address addr;
	addr.Hostname(hostname);
	addr.Service(port);
	m_sock->Connect(addr, false);
	m_sock->WaitOnConnect(timeout);
	if (m_sock->IsConnected()) return true;
	m_sock->Close();
	return false;
}

ServerAnswer *
lESMTPsocket::OpenWelcomeConnection(wxChar *hostname, unsigned short int port, unsigned short int timeout)
{
	wxIPV4address addr;
	addr.Hostname(hostname);
	addr.Service(port);
	m_sock->Connect(addr, false);
	m_sock->WaitOnConnect(timeout);
	if (!m_sock->IsConnected())
	{
		m_sock->Close();
		return NULL;
	}
	ServerAnswer *m_CA = new ServerAnswer;
	m_CA->message = new wxChar[1024];
	m_CA->code = 0;
	Read(m_CA->message, 1024);
	sscanf(m_CA->message, "%hu", &(m_CA->code));
	return m_CA;
}

bool
lESMTPsocket::Read(void *buffer, wxUint32 nbytes)
{
	m_sock->Read(buffer, nbytes);
	return !m_sock->Error();
}

ServerAnswer *
lESMTPsocket::SendCommand(wxChar *command)
{
	Write(command, strlen(command));
	ServerAnswer *m_CA = new ServerAnswer;
	m_CA->message = new wxChar[1024];
	m_CA->code = 0;
	Read(m_CA->message, 1024);
	sscanf(m_CA->message, "%hu", &(m_CA->code));
	return m_CA;
}

bool
lESMTPsocket::Write(const void *buffer, wxUint32 nbytes)
{
	m_sock->Write(buffer, nbytes);
	return !m_sock->Error();
}

bool
lESMTPsocket::CloseConnection()
{
	return m_sock->Close();
}

void
lESMTPsocket::FreeAnswer(ServerAnswer *m_sa)
{
	if (m_sa == NULL) return;
	if (m_sa->message != NULL) delete m_sa->message;
	delete m_sa;
}