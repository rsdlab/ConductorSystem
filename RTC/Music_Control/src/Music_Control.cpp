// -*- C++ -*-
/*!
 * @file  Music_Control.cpp
 * @brief Music Controler
 * @date $Date$
 *
 * $Id$
 */

#include "Music_Control.h"

// Module specification
// <rtc-template block="module_spec">
static const char* music_control_spec[] =
  {
    "implementation_id", "Music_Control",
    "type_name",         "Music_Control",
    "description",       "Music Controler",
    "version",           "1.0.0",
    "vendor",            "rsdlab",
    "category",          "Conductol",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "0",

    // Widget
    "conf.__widget__.debug", "text",
    // Constraints

    "conf.__type__.debug", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Music_Control::Music_Control(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_InstrumentControlIn("InstrumentControl", m_InstrumentControl),
    m_MusicVolumeIn("MusicVolume", m_MusicVolume),
    m_PartA_InputIn("PartA_Input", m_PartA_Input),
    m_PartB_InputIn("PartB_Input", m_PartB_Input),
    m_PartC_InputIn("PartC_Input", m_PartC_Input),
    m_PartD_InputIn("PartD_Input", m_PartD_Input),
    m_PartA_OutputOut("PartA_Output", m_PartA_Output),
    m_PartB_OutputOut("PartB_Output", m_PartB_Output),
    m_PartC_OutputOut("PartC_Output", m_PartC_Output),
    m_PartD_OutputOut("PartD_Output", m_PartD_Output)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Music_Control::~Music_Control()
{
}



RTC::ReturnCode_t Music_Control::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("InstrumentControl", m_InstrumentControlIn);
  addInPort("MusicVolume", m_MusicVolumeIn);
  addInPort("PartA_Input", m_PartA_InputIn);
  addInPort("PartB_Input", m_PartB_InputIn);
  addInPort("PartC_Input", m_PartC_InputIn);
  addInPort("PartD_Input", m_PartD_InputIn);
  
  // Set OutPort buffer
  addOutPort("PartA_Output", m_PartA_OutputOut);
  addOutPort("PartB_Output", m_PartB_OutputOut);
  addOutPort("PartC_Output", m_PartC_OutputOut);
  addOutPort("PartD_Output", m_PartD_OutputOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Music_Control::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Music_Control::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Music_Control::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Music_Control::onActivated(RTC::UniqueId ec_id)
{
	//Initial Volume Value
	m_MusicVolume.data = 50;
	//Initial State of Musical Instrument
	m_InstrumentControl.Part1 = true;
	m_InstrumentControl.Part2 = true;
	m_InstrumentControl.Part3 = true;
	m_InstrumentControl.Part4 = true;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Music_Control::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Music_Control::onExecute(RTC::UniqueId ec_id)
{
	//Acquire Volume Information
	if (m_MusicVolumeIn.isNew())
	{
		m_MusicVolumeIn.read();
	}

	//Acquisition of Instrument Information
	if (m_InstrumentControlIn.isNew())
	{
		m_InstrumentControlIn.read();
	}

	//Input of Music Data Information of Each Instrument
	//PartA
	if (m_PartA_InputIn.isNew())
	{
		m_PartA_InputIn.read();

		m_PartA_Output.event = m_PartA_Input.event;
		m_PartA_Output.ch.channel = m_PartA_Input.ch.channel;
		m_PartA_Output.ch.note_number = m_PartA_Input.ch.note_number;
		m_PartA_Output.ch.velocity = static_cast<short>(m_MusicVolume.data);
	}
	//PartB
	if (m_PartB_InputIn.isNew())
	{
		m_PartB_InputIn.read();

		m_PartB_Output.event = m_PartB_Input.event;
		m_PartB_Output.ch.channel = m_PartB_Input.ch.channel;
		m_PartB_Output.ch.note_number = m_PartB_Input.ch.note_number;
		m_PartB_Output.ch.velocity = static_cast<short>(m_MusicVolume.data);
	}
	//PartC
	if (m_PartC_InputIn.isNew())
	{
		m_PartC_InputIn.read();

		m_PartC_Output.event = m_PartC_Input.event;
		m_PartC_Output.ch.channel = m_PartC_Input.ch.channel;
		m_PartC_Output.ch.note_number = m_PartC_Input.ch.note_number;
		m_PartC_Output.ch.velocity = static_cast<short>(m_MusicVolume.data);
	}
	//PartD
	if (m_PartD_InputIn.isNew())
	{
		m_PartD_InputIn.read();

		m_PartD_Output.event = m_PartD_Input.event;
		m_PartD_Output.ch.channel = m_PartD_Input.ch.channel;
		m_PartD_Output.ch.note_number = m_PartD_Input.ch.note_number;
		m_PartD_Output.ch.velocity = static_cast<short>(m_MusicVolume.data);
	}

	//Output of Music Data Information of Each Instrument
	//PartA
	if (m_InstrumentControl.Part1)
	{
		m_PartA_OutputOut.write();
	}
	//PartB
	if (m_InstrumentControl.Part2)
	{
		m_PartB_OutputOut.write();
	}
	//PartC
	if (m_InstrumentControl.Part3)
	{
		m_PartC_OutputOut.write();
	}
	//PartD
	if (m_InstrumentControl.Part4)
	{
		m_PartD_OutputOut.write();
	}

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Music_Control::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Music_Control::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Music_Control::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Music_Control::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Music_Control::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void Music_ControlInit(RTC::Manager* manager)
  {
    coil::Properties profile(music_control_spec);
    manager->registerFactory(profile,
                             RTC::Create<Music_Control>,
                             RTC::Delete<Music_Control>);
  }
  
};


