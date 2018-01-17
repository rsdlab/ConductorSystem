//Socket通信プログラム
#include "SocketServer.h"

	//サーバの初期設定
	//Initialize of TCP/IP server
	//portnum		:Port number to be enable to connect any TCP/IP client
	//
	//*Reterun Value*
	//true			: Connection OK
	//false			: Connection NG
bool SocketServer::Initialize(int portnum){
    struct sockaddr_in srcAddr;
  
    //Set of the parameter to sockaddr_in structure.
    memset(&srcAddr, 0, sizeof(srcAddr));
    srcAddr.sin_port = htons(portnum);
    srcAddr.sin_family = AF_INET;
    srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    //Create socket
    if((srcSocket = socket(AF_INET, SOCK_STREAM, 0))<0){
        std::cout<<"srcSocket error"<<std::endl;
        return false;
    }
  
    //Bind to Socket
    if(bind(srcSocket, (struct sockaddr *) &srcAddr, sizeof(srcAddr))<0){
        cout<<"bind error"<<endl;
        return false;
    }
  
    //Wait connection from TCP/IP client
    listen(srcSocket, 1);
    std::cout<<"Waiting for Connection..."<<std::endl;
    return true;
}

bool SocketServer::Accept(int sec, int microsec){
    struct sockaddr_in dstAddr;
    struct timeval timeout;
    fd_set read_fd;
    
    int dstAddrSize = sizeof(dstAddr);
    
    //time < 0 : not timeout
    //time > 0 : set timeout
    if(sec < 0 || microsec < 0){
        if((dstSocket = accept(srcSocket,(struct sockaddr *) &dstAddr, (socklen_t*) &dstAddrSize))<0){
            std::cout<<"accept error"<<std::endl;
            return false;
        }
            
    }else{
        //Register socket waiting for select
        FD_ZERO(&read_fd);
        FD_SET(srcSocket, &read_fd);
        //set timeout
        timeout.tv_sec = sec; //sec
        timeout.tv_usec = microsec;   //micro sec

        //checking socket accept
        if(select(srcSocket+1, &read_fd, NULL, NULL, &timeout) < 0){
            std::cout<<"select error"<<std::endl;
	    return false;
        }
        
        //count timeout
        if(FD_ISSET(srcSocket, &read_fd)){
            //Accept connection from TCP/IP client
            if((dstSocket = accept(srcSocket,(struct sockaddr *) &dstAddr, (socklen_t*) &dstAddrSize))<0){
                std::cout<<"accept error"<<std::endl;
		return false;
            }
        }else{
            std::cout<<"Accept timeout"<<std::endl;
            return false;
        }
    }

    std::cout<<"Connected from"<<inet_ntoa(dstAddr.sin_addr)<<std::endl;
    return true;
}



//サーバの切断
//Close server
void SocketServer::Close(){
    char str[50];
    sprintf(str,"Please Close your Socket!!");
    cout<<"Close Socket"<<endl;
    shutdown(srcSocket,2);
    shutdown(dstSocket,2);
}

//データの受信
//Receive message from connected COM port
//receivedata	: Receive data from connected COM port
//length		: Data length of receivedata
//
//*Return Value*
//Receive data size
//(If this function returns "-1", data receive is failed.)
int SocketServer::ReceiveMessage(char *receivedata, int length){
    int recvNum;
    recvNum = recv(dstSocket,receivedata,BUFFER_SIZE-1,0);
    if(recvNum==-1||recvNum==0){
        cout<<"Cannot receve data"<<endl;
        return -1;
        }
    return recvNum;
}

//データの送信
//Send message to connected TCP/IP port
//senddata		: Send data for connected TCP/IP port
//length		: Data length of send data
//
//*Reterun Value*
//true			: Data send OK
//false			: Data send NG.
bool SocketServer::SendMessage(char *senddata, int length){
    send(dstSocket,senddata,length,0);
    return true;
}


