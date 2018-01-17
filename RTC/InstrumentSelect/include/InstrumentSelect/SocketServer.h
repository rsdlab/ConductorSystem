///////////////////////////////////////////////////////////////////////
//Socket Communication Server Class
///////////////////////////////////////////////////////////////////////

#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <iostream>
#include <string.h>
#include <cstdio>
using namespace std;
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <sys/select.h>

//�o�b�t�@�i�K���j
#define BUFFER_SIZE 1024

class SocketServer{
	private:
		#ifdef WIN32
		 
		#else
  		//Socket Handler for TCP/IP client on Linux
  		int srcSocket; //self socket
  		int dstSocket; //pair socket
		#endif

    
    public:
        //�T�[�o�̏����ݒ�
        //Initialize of TCP/IP server
        //portnum		:Port number to be enable to connect any TCP/IP client
        //
        //*Reterun Value*
        //true			: Connection OK
        //false			: Connection NG
        bool Initialize(int portnum);


        //�T�[�o�̎�t
        bool Accept(int sec, int microsec);

        //�T�[�o�̐ؒf
        //Close server
        void Close();

        //�f�[�^�̎�M
        //Receive message from connected COM port
        //receivedata	: Receive data from connected COM port
        //length		: Data length of receivedata
        //
        //*Return Value*
        //Receive data size
        //(If this function returns "-1", data receive is failed.)
        int	ReceiveMessage(char *receivedata, int length);

        //�f�[�^�̑��M
        //Send message to connected TCP/IP port
        //senddata		: Send data for connected TCP/IP port
        //length		: Data length of send data
        //
        //*Reterun Value*
        //true			: Data send OK
        //false			: Data send NG.
        bool SendMessage(char *senddata, int length);

};

#endif  //SOCKETSERVER_H
