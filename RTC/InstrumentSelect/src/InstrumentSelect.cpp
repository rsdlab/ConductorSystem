// -*- C++ -*-
/*!
 * @file  InstrumentSelect.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "InstrumentSelect.h"

// Module specification
// <rtc-template block="module_spec">
static const char* instrumentselect_spec[] =
  {
    "implementation_id", "InstrumentSelect",
    "type_name",         "InstrumentSelect",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "VenderName",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.portnumber", "10000",
    "conf.default.timeout_sec", "1",

    // Widget
    "conf.__widget__.portnumber", "text",
    "conf.__widget__.timeout_sec", "text",
    // Constraints

    "conf.__type__.portnumber", "short",
    "conf.__type__.timeout_sec", "short",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
InstrumentSelect::InstrumentSelect(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_InstSelectOut("InstSelect", m_InstSelect)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
InstrumentSelect::~InstrumentSelect()
{
}



RTC::ReturnCode_t InstrumentSelect::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("InstSelect", m_InstSelectOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("portnumber", m_portnumber, "10000");
  bindParameter("timeout_sec", m_timeout_sec, "1");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t InstrumentSelect::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t InstrumentSelect::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t InstrumentSelect::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t InstrumentSelect::onActivated(RTC::UniqueId ec_id)
{
  if(ss.Initialize(m_portnumber) == false){
    std::cout<<"Continuous connection is not possible on the same port"<<std::endl;
    std::cout<<"Please wait 3 minutes or Please change portnumber"<<std::endl;
    ss.Close();
    return RTC::RTC_ERROR;
  }
  return RTC::RTC_OK;
}


RTC::ReturnCode_t InstrumentSelect::onDeactivated(RTC::UniqueId ec_id)
{
  // close socket and port
  ss.Close();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t InstrumentSelect::onExecute(RTC::UniqueId ec_id)
{
  memset(buffer,0,sizeof(buffer));
  
  if(ss.Accept(m_timeout_sec, 0) == false){
    std::cout<<"not accept"<<std::endl;
    return RTC::RTC_OK;
  }
  
  if(ss.ReceiveMessage(buffer,sizeof(buffer)) != -1){
    std::cout << "receive : " << buffer << std::endl;

    if(buffer[0] == '1'){
      m_InstSelect.PlayAndStop = true;
    }else{
      m_InstSelect.PlayAndStop = false;
    }
    
    if(buffer[1] == '1'){
      m_InstSelect.Part1 = true;
    }else{
      m_InstSelect.Part1 = false;
    }

    if(buffer[2] == '1'){
      m_InstSelect.Part2 = true;
    }else{
      m_InstSelect.Part2 = false;
    }    

    if(buffer[3] == '1'){
      m_InstSelect.Part3 = true;
    }else{
      m_InstSelect.Part3 = false;
    }  

    if(buffer[4] == '1'){
      m_InstSelect.Part4 = true;
    }else{
      m_InstSelect.Part4 = false;
    }  
   
    std::cout<<"send message :"<< m_InstSelect.PlayAndStop <<std::endl;
    std::cout<<"send message :"<< m_InstSelect.Part1 <<std::endl;
    std::cout<<"send message :"<< m_InstSelect.Part2 <<std::endl;
    std::cout<<"send message :"<< m_InstSelect.Part3 <<std::endl;
    std::cout<<"send message :"<< m_InstSelect.Part4 <<std::endl;

    m_InstSelectOut.write();
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t InstrumentSelect::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t InstrumentSelect::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t InstrumentSelect::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t InstrumentSelect::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t InstrumentSelect::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void InstrumentSelectInit(RTC::Manager* manager)
  {
    coil::Properties profile(instrumentselect_spec);
    manager->registerFactory(profile,
                             RTC::Create<InstrumentSelect>,
                             RTC::Delete<InstrumentSelect>);
  }
  
};


