// libEziiiSMTP
//
// Main class.
//
// Author : neiro 27-11-05
// Licence / Documentation : see setup.h

#include "setup.h"

#ifdef USE_WXSOCKS
#include "socket-wx.h"
#endif
#ifdef USE_WINSOCKS
#include "socket-ws.h"
#endif

#include "libEziiiSMTP.h"

libEziiiSMTP::libEziiiSMTP(wxChar *FROMaddr)
{
	_FROMaddr = new string(FROMaddr);
}

libEziiiSMTP::~libEziiiSMTP()
{
	delete _FROMaddr;
	delete _subject;
	delete _message;
	delete _server;
}

void
libEziiiSMTP::addTO(wxChar *TOaddr)
{
	_TOaddr.push_back(new string(TOaddr));
}

void
libEziiiSMTP::addCC(wxChar *CCaddr)
{
	_TOaddr.push_back(new string(CCaddr));
}

void
libEziiiSMTP::addBCC(wxChar *BCCaddr)
{
	_TOaddr.push_back(new string(BCCaddr));
}

void
libEziiiSMTP::setSubject(wxChar *Subject)
{
	_subject = new string(Subject);
}

void
libEziiiSMTP::setMessage(wxChar *Message)
{
	_message = new string(Message);
}

void
libEziiiSMTP::setServer(wxChar *Server)
{
	_server = new string(Server);
}

void
libEziiiSMTP::setPort(unsigned short int Port)
{
	_port = Port;
}

bool
libEziiiSMTP::send()
{
	//wxChar *m_tmpnum = new wxChar[10];
	wxChar *m_tmpstr = new wxChar[1024];
	ServerAnswer *m_sa;
	SMTPcode *m_sc;
	SMTPstage m_ss;
	lESMTPsocket m_socket = lESMTPsocket();
	wxChar *m_hostname = m_socket.GetHostName();

	m_ss = SSsocketconx; // Connection au serveur
	m_sa = m_socket.OpenWelcomeConnection(strdup(_server->c_str()), _port, 10);
	m_sc = SMTPAanalyze(m_sa, m_ss);

	m_ss = SShelo; // Echange de politesse :)
	if (m_sc->gravity == SGok) {
		sprintf(m_tmpstr, "HELO %s\r\n", m_hostname);
		m_sa = m_socket.SendCommand(m_tmpstr);
		m_sc = SMTPAanalyze(m_sa, m_ss);
	}

	m_ss = SSmailfrom; // On donne le nom de l'écrivain ;)
	if (m_sc->gravity == SGok) {
		sprintf(m_tmpstr, "MAIL FROM:<%s>\r\n", _FROMaddr->c_str());
		m_sa = m_socket.SendCommand(m_tmpstr);
		m_sc = SMTPAanalyze(m_sa, m_ss);
	}

	m_ss = SSreceptto; // On donne le nom du/des destinataires
	for ( unsigned int i = 0 ; i < _TOaddr.size() ; i++ ) {
		if (m_sc->gravity == SGok) {
			sprintf(m_tmpstr, "RCPT TO:<%s>\r\n", (string *)(_TOaddr.at(i))->c_str());
			m_sa = m_socket.SendCommand(m_tmpstr);
			m_sc = SMTPAanalyze(m_sa, m_ss);
		}
	}

	m_ss = SSdata; // Attention, on va balancer des données =)
	if (m_sc->gravity == SGok) {
		m_sa = m_socket.SendCommand("DATA\r\n");
		m_sc = SMTPAanalyze(m_sa, m_ss);
	}

	m_ss = SSsenddata; // c'est parti pour les data...
	if (m_sc->gravity == SGok) {
		basic_string <char>::size_type m_indexChr;
		static const basic_string <char>::size_type npos = (const basic_string <char>::size_type)-1;
		while ((m_indexChr = _message->find("\r\n.\r\n", 0)) != npos)
            _message->replace(m_indexChr + 2, 1, "_");
		if (_subject->size() != 0) sprintf(m_tmpstr, "Subject:%s\r\n\r\n%s\r\n.\r\n", _subject->c_str(), _message->c_str());
		else sprintf(m_tmpstr, "%s\r\n.\r\n", _message->c_str());
		m_sa = m_socket.SendCommand(m_tmpstr);
		m_sc = SMTPAanalyze(m_sa, m_ss);
	}

	m_ss = SSquit; // on dit au revoir ;)
	if (m_sc->gravity == SGok) {
		m_sa = m_socket.SendCommand("QUIT\r\n");
		m_sc = SMTPAanalyze(m_sa, m_ss);
	}

	m_ss = SSsocketdeco; // déco
	m_socket.CloseConnection();

	if (m_sc->gravity == SGok) return true;
	return false;
}

SMTPcode *
libEziiiSMTP::SMTPAanalyze(void *SMTPServerAnswer, SMTPstage stage)
{
	ServerAnswer *m_sa = (ServerAnswer *)SMTPServerAnswer;
	SMTPcode *m_sc = new SMTPcode;
	if (m_sa != NULL) {
		switch (m_sa->code)
		{
		case 0:   //libEziiiSMTP can't connect ! server unreachable...
			m_sc->gravity = SGfatal;
			break;
		case 211: //System status, or system help reply
			m_sc->gravity = SGwarn;
			break;
		case 214: //Help message
			m_sc->gravity = SGwarn;
			break;
		case 220: //<domain> Service ready
			if (stage != SSsocketconx) m_sc->gravity = SGfatal;
			else m_sc->gravity = SGok;
			break;
		case 221: //<domain> Service closing transmission channel
			if (stage != SSsocketdeco) m_sc->gravity = SGfatal;
			else m_sc->gravity = SGok;
			break;
		case 250: //Requested mail action okay, completed
			m_sc->gravity = SGok;
			break;
		case 251: //User not local; will forward to <forward-path>
			m_sc->gravity = SGfail;
			break;
		case 354: //Start mail input; end with <CRLF>.<CRLF>
			if (stage == SSdata) m_sc->gravity = SGok;
			else m_sc->gravity = SGfail;
			break;
		case 421: //<domain> Service not available, closing transmission channel
			m_sc->gravity = SGfatalmustquit;
			break;
		case 450: //Requested mail action not taken: mailbox unavailable
			m_sc->gravity = SGfatalmustquit;
			break;
		case 451: //Requested action aborted: local error in processing
			m_sc->gravity = SGfatalmustquit;
			break;
		case 452: //Requested action not taken: insufficient system storage
			m_sc->gravity = SGfatalmustquit;
			break;
		case 500: //Syntax error, command unrecognized
			m_sc->gravity = SGfail;
			break;
		case 501: //Syntax error in parameters or arguments
			m_sc->gravity = SGfail;
			break;
		case 502: //Command not implemented
			m_sc->gravity = SGfail;
			break;
		case 503: //Bad sequence of commands
			m_sc->gravity = SGfail;
			break;
		case 504: //Command parameter not implemented
			m_sc->gravity = SGfail;
			break;
		case 550: //Requested action not taken: mailbox unavailable
			m_sc->gravity = SGfatalmustquit;
			break;
		case 551: //User not local; please try <forward-path>
			m_sc->gravity = SGfail;
			break;
		case 552: //Requested mail action aborted: exceeded storage allocation
			m_sc->gravity = SGfatalmustquit;
			break;
		case 553: //Requested action not taken: mailbox name not allowed
			m_sc->gravity = SGfatalmustquit;
			break;
		case 554: //Transaction failed
			m_sc->gravity = SGfatalmustquit;
			break;
		default:
			m_sc->gravity = SGfail;
		}
		m_sc->code = m_sa->code;
		m_sc->servermessage = new wxChar[1024];
		strcpy(m_sc->servermessage, m_sa->message);
		m_sc->servermessage = new wxChar[1024];
		if (m_sa->message != NULL)
		{
			strcpy(m_sc->servermessage, m_sa->message);
			delete m_sa->message;
		}
		else m_sc->servermessage = "";
		delete m_sa;
		return m_sc;
	}
	m_sc->code = 0;
	m_sc->gravity = SGfatal;
	m_sc->message = "";
	m_sc->servermessage = "";
	m_sc->stage = stage;
	return m_sc;
}