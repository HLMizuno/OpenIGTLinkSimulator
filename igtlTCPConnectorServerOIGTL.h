/*=========================================================================

  Program:   OpenIGTLink Connector Class for OpenIGTLink Simulator
  Language:  C++

  Copyright (c) Brigham and Women's Hospital. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

  =========================================================================*/


#ifndef __TCPConnectorServerOIGTL_H
#define __TCPConnectorServerOIGTL_H

#include "igtlWin32Header.h"
#include "igtlMultiThreader.h"
#include "igtlServerSocket.h"
#include "igtlMessageHeader.h"

#include "igtlTCPConnectorServer.h"


class qDataGeneratorBase;


namespace igtl
{

  class IGTLCommon_EXPORT TCPConnectorServerOIGTL : public TCPConnectorServer
  {
  public:
  
    typedef TCPConnectorServerOIGTL   Self;
    typedef Object  Superclass;
    typedef SmartPointer<Self>        Pointer;
    typedef SmartPointer<const Self>  ConstPointer;

    igtlTypeMacro(igtl::TCPConnectorServerOIGTL, igtl::TCPConnectorServer)
      igtlNewMacro(igtl::TCPConnectorServerOIGTL);

  public:

    virtual const char * GetClassName() { return "OpenIGTLink"; };
    int SetMessageHandler();
    virtual int    PushMessage(igtl::MessageBase * message);

    // Functions to register message handlers. RegisterMessageHandler(type, handler) registers
    // a handler for any messages with the specified type, while RegisterMessageHandler(type, name, handler)
    // registers handler for messages with the specified type and name. If the functions are called
    // multiple times, RegisterMessageHandler(type, name, handler) will 'overwrite'
    // the registered information. For example, if those functions are called as:
    //      
    //   RegisterMessageHandler("type1", handler1);
    //   RegisterMessageHandler("type1", "name1", handler2);
    //   RegisterMessageHandler("type1", "name2", handler3);
    // 
    // the ConnectorServerOIGTL class will use handler2 and hander3 for "type1" messages with 
    // message names of "name1" and "name2" respectively, and use handler1 for other "type1"
    // messages. This handler information is always "overwritten". For example, if the function is called as:
    //
    //   RegisterMessageHandler("type1", "name1", handler2);
    //   RegisterMessageHandler("type1", "name1", handler3);
    //
    // "type1" messages with a name "name1" will be handled by handler3. Another example is, if the
    // function is called as:
    //
    //   RegisterMessageHandler("type1", "name1", handler2);
    //   RegisterMessageHandler("type1", "name2", handler3);
    //   RegisterMessageHandler("type1", handler1);
    //
    // the last call will overwrite the others, meaning that any type1 message will be handled by handler1.
  
    void    RegisterMessageHandler(const char* type, qDataGeneratorBase* handler);
    void    RegisterMessageHandler(const char* type, const char* name, qDataGeneratorBase* handler);
    void    UnRegisterMessageHandler(const char* type);

  protected:

    TCPConnectorServerOIGTL();
    ~TCPConnectorServerOIGTL();

    void PrintSelf(std::ostream& os) const;

    virtual int Initialize();
    virtual int ReceiveMessage();
    virtual int Finalize();

    int ReceiveTransform(igtl::MessageHeader * header);
  
  private:
  
    igtl::MessageHeader::Pointer HeaderMsg;

    // Templates to record message handlers
    // The ConnectorServerOIGTL class manages message handlers (generators) based on
    // message type string and message name string. The following pseudo code shows a process
    // to identify the message handler after receiving a message, given message type TYPE
    // and message name NAME
    //   
    //   MessageHandlerNameMapType map = MessageHandlerTypeMap[TYPE]
    //   if (map == Null)
    //     Skip()
    //   
    //   key = '+' + NAME
    //   qDataGeneratorBase handler = map[key]
    //   
    //   if (handler == Null)
    //     handler = map['*']
    //
    // To differenciate the wild card "*" from message name "*", keys for MessageHandlerNameMapType
    // is generated by adding '+' at the top of message name string. Therefore,
    //
    //   - key for the wild card: "*"
    //   - key for string "*": "+*"


    typedef std::map< std::string, qDataGeneratorBase* >    MessageHandlerNameMapType;
    typedef std::map< std::string, MessageHandlerNameMapType > MessageHandlerTypeMapType;

    MessageHandlerTypeMapType MessageHandlerTypeMap;

  };

}

#endif //__THREAD_H
