// -*- C++ -*-
/*!
 * @file  VolumeAdjust.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "VolumeAdjust.h"

// Module specification
// <rtc-template block="module_spec">
static const char* volumeadjust_spec[] =
  {
    "implementation_id", "VolumeAdjust",
    "type_name",         "VolumeAdjust",
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
    "conf.default.base", "4.5",

    // Widget
    "conf.__widget__.base", "text",
    // Constraints

    "conf.__type__.base", "double",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
VolumeAdjust::VolumeAdjust(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_accIn("acc", m_acc),
    m_baseaccIn("baseacc", m_baseacc),
    m_volumesetOut("volumeset", m_volumeset)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
VolumeAdjust::~VolumeAdjust()
{
}



RTC::ReturnCode_t VolumeAdjust::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("acc", m_accIn);
  addInPort("baseacc", m_baseaccIn);
  
  // Set OutPort buffer
  addOutPort("volumeset", m_volumesetOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("base", m_base, "4.5");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t VolumeAdjust::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t VolumeAdjust::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t VolumeAdjust::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t VolumeAdjust::onActivated(RTC::UniqueId ec_id)
{
  count = 1;
  memset(data, 0, sizeof(data));
  basedata = m_base;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t VolumeAdjust::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t VolumeAdjust::onExecute(RTC::UniqueId ec_id)
{
  if(m_accIn.isNew())
    { 
      m_accIn.read();
      accdata = m_acc.data;
      if(count<(sizeof(data)/sizeof(data[0]))){
	count += 1;
      }else{
	count = 1;
      }
      data[count] = accdata;
      std::cout<<"accdata"<<accdata<<std::endl;

      sum = 0;
      ave= 0;
      for(i=0;i<(sizeof(data)/sizeof(data[0]));i++){
	sum += data[i];
      }
      ave = sum/(sizeof(data)/sizeof(data[0]));

      changedata = ave*100/(basedata*2);
  
      if(0<=changedata && changedata<=5){m_volumeset.data = 0;}
      else if(5<changedata && changedata<=15){m_volumeset.data = 10;}
      else if(15<changedata && changedata<=25){m_volumeset.data = 20;}
      else if(25<changedata && changedata<=35){m_volumeset.data = 30;}
      else if(35<changedata && changedata<=45){m_volumeset.data = 40;}
      else if(45<changedata && changedata<=55){m_volumeset.data = 50;}
      else if(55<changedata && changedata<=65){m_volumeset.data = 60;}
      else if(65<changedata && changedata<=75){m_volumeset.data = 70;}
      else if(75<changedata && changedata<=85){m_volumeset.data = 80;}
      else if(85<changedata && changedata<=95){m_volumeset.data = 90;}
      else if(95<changedata){m_volumeset.data = 100;}
      
      std::cout<<"set volume :"<< m_volumeset.data <<std::endl;
      m_volumesetOut.write();
      
    }
      
  if(m_baseaccIn.isNew())
    {
      m_baseaccIn.read();
      basedata = m_baseacc.data;
    }
  if(basedata == 0){
    return RTC::RTC_ERROR;
  }
  

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t VolumeAdjust::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t VolumeAdjust::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t VolumeAdjust::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t VolumeAdjust::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t VolumeAdjust::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void VolumeAdjustInit(RTC::Manager* manager)
  {
    coil::Properties profile(volumeadjust_spec);
    manager->registerFactory(profile,
                             RTC::Create<VolumeAdjust>,
                             RTC::Delete<VolumeAdjust>);
  }
  
};


