#ifndef _LIBEZIIISMTP_H
#define _LIBEZIIISMTP_H

#include "setup.h"

#include <string>
#include <vector>

using namespace std;

enum SMTPgravity {
	SGok,
	SGwarn,
	SGfail,
	SGfatal,
	SGfatalmustquit
};

enum SMTPstage {
	SSsocketconx,
	SShelo,
	SSmailfrom,
	SSreceptto,
	SSdata,
	SSsenddata,
	SSquit,
	SSsocketdeco
};

struct SMTPcode {
	unsigned short int code;
	SMTPgravity gravity;
	SMTPstage stage;
	wxChar *message;
	wxChar *servermessage;
};

class libEziiiSMTP
{
public:
	libEziiiSMTP(wxChar *FROMaddr);
	~libEziiiSMTP();
	void addTO(wxChar *TOaddr);
	void addCC(wxChar *CCaddr);
	void addBCC(wxChar *BCCaddr);
	void setSubject(wxChar *Subject);
	void setMessage(wxChar *Message);
	void setServer(wxChar *Server);
	void setPort(unsigned short int Port);
	bool send();
private:
	SMTPcode *SMTPAanalyze(void *SMTPServerAnswer, SMTPstage stage);
	string *_FROMaddr;
	vector<string *> _TOaddr;
	vector<string *> _CCaddr;
	vector<string *> _BCCaddr;
	string *_subject;
	string *_message;
	string *_server;
	unsigned short int _port;
};

#endif _LIBEZIIISMTP_H