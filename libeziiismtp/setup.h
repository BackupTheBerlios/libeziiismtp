// libEziiiSMTP
//
// setup file... just setup sockets...
//
// Author : neiro 27-11-05
// Licence / Documentation : use page down key ;)

#ifndef _LIBEZIIISMTPSETUP_H
#define _LIBEZIIISMTPSETUP_H

//							SETUP HERE !!!!!!!!!!!!!!!!!!
#define USE_WXSOCKS
// or #define USE_WINSOCKS
// and soon... USE_JNETLIB
//							THAT'S ALL :)

#ifdef USE_WXSOCKS
#pragma message("wxWidgets build...")
#else
typedef char wxChar;
#endif USE_WXSOCKS

#ifdef USE_JNETLIB
#pragma message("JNetLib build...")
#endif

#ifdef USE_WINSOCKS
#pragma message("WinSocks build...")
#endif

#endif //_LIBEZIIISMTPSETUP_H

/* licence here (based on new BSD licence) :

Copyright (c) 2005, neiro
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
		this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
		this list of conditions and the following disclaimer in the documentation
		and/or other materials provided with the distribution.
    * The name 'neiro' can't be used to endorse or promote products derived from
		this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Documentation (very light ^^) :
Hum... You have to choose your "sockets type", include "libEziiiSMTP.h" where you
want to send mails, and write something like that on your code :

	libEziiiSMTP test = libEziiiSMTP("from@myserv.abc");
	test.setServer("smtp.myisp.com");
	test.setPort(25);
	test.addTO("mailadress@antherserv.bla");
	test.setMessage("this is a test mail...");
	test.setSubject("hello world");
	test.send();

Keep in mind this lib is at an alpha status... there is too many bugs to use it
on production...
Any help, suggestion, patch, chunk of code, ect, are welcome.
*/