#!/usr/bin/env python
# -*- coding: utf-8 -*-
# -*- Python -*-

"""
 @file Acceleration.py
 @brief ModuleDescription
 @date $Date$


"""
import sys
import time
sys.path.append(".")

# Import RTM module
import RTC
import OpenRTM_aist

import RPi.GPIO as GPIO
import accget

global pin
global running
pin = 17
running = False

# Import Service implementation class
# <rtc-template block="service_impl">

# </rtc-template>

# Import Service stub modules
# <rtc-template block="consumer_import">
# </rtc-template>


# This module's spesification
# <rtc-template block="module_spec">
acceleration_spec = ["implementation_id", "Acceleration", 
		 "type_name",         "Acceleration", 
		 "description",       "ModuleDescription", 
		 "version",           "1.0.0", 
		 "vendor",            "VenderName", 
		 "category",          "Category", 
		 "activity_type",     "STATIC", 
		 "max_instance",      "1", 
		 "language",          "Python", 
		 "lang_type",         "SCRIPT",
		 ""]
# </rtc-template>

##
# @class Acceleration
# @brief ModuleDescription
# 
# 
class Acceleration(OpenRTM_aist.DataFlowComponentBase):
	
	##
	# @brief constructor
	# @param manager Maneger Object
	# 
	def __init__(self, manager):
		OpenRTM_aist.DataFlowComponentBase.__init__(self, manager)

		Acc_out_arg = [None] * ((len(RTC._d_TimedDouble) - 4) / 2)
		self._d_Acc_out = RTC.TimedDouble(*Acc_out_arg)
		"""
		"""
		self._Acc_outOut = OpenRTM_aist.OutPort("Acc_out", self._d_Acc_out)
		switch_arg = [None] * ((len(RTC._d_TimedBoolean) - 4) / 2)
		self._d_switch = RTC.TimedBoolean(*switch_arg)
		"""
		"""
		self._switchOut = OpenRTM_aist.OutPort("switch", self._d_switch)


		


		# initialize of configuration-data.
		# <rtc-template block="init_conf_param">
		
		# </rtc-template>


		 
	##
	#
	# The initialize action (on CREATED->ALIVE transition)
	# formaer rtc_init_entry() 
	# 
	# @return RTC::ReturnCode_t
	# 
	#
	def onInitialize(self):
                self._accdata = RTC.TimedDouble(RTC.Time(0,0),0)
                self._Acc_outOut = OpenRTM_aist.OutPort("Acc_out", self._accdata)
                
                self._switch = RTC.TimedBoolean(RTC.Time(0,0),0)
                self._switchOut = OpenRTM_aist.OutPort("switch", self._switch)
		# Bind variables and configuration variable
		
		# Set InPort buffers
		
		# Set OutPort buffers
		self.addOutPort("Acc_out",self._Acc_outOut)
		self.addOutPort("switch",self._switchOut)
		
		# Set service provider to Ports
		
		# Set service consumers to Ports
		
		# Set CORBA Service Ports
		
		return RTC.RTC_OK
	
	#	##
	#	# 
	#	# The finalize action (on ALIVE->END transition)
	#	# formaer rtc_exiting_entry()
	#	# 
	#	# @return RTC::ReturnCode_t
	#
	#	# 
	#def onFinalize(self):
	#
	#	return RTC.RTC_OK
	
	#	##
	#	#
	#	# The startup action when ExecutionContext startup
	#	# former rtc_starting_entry()
	#	# 
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onStartup(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	#	##
	#	#
	#	# The shutdown action when ExecutionContext stop
	#	# former rtc_stopping_entry()
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onShutdown(self, ec_id):
	#
	#	return RTC.RTC_OK
	
		##
		#
		# The activated action (Active state entry action)
		# former rtc_active_entry()
		#
		# @param ec_id target ExecutionContext Id
		# 
		# @return RTC::ReturnCode_t
		#
		#
	def onActivated(self, ec_id):
                global pin
                GPIO.setmode(GPIO.BCM)
                GPIO.setup(pin, GPIO.IN,GPIO.PUD_DOWN)
                
		return RTC.RTC_OK
	
		##
		#
		# The deactivated action (Active state exit action)
		# former rtc_active_exit()
		#
		# @param ec_id target ExecutionContext Id
		#
		# @return RTC::ReturnCode_t
		#
		#
	def onDeactivated(self, ec_id):
                GPIO.cleanup()
		return RTC.RTC_OK
	
		##
		#
		# The execution action that is invoked periodically
		# former rtc_active_do()
		#
		# @param ec_id target ExecutionContext Id
		#
		# @return RTC::ReturnCode_t
		#
		#
	def onExecute(self, ec_id):
                global pin
                global running

                print(running)

                #write when switch change 
                if(GPIO.input(pin) != running):
                        print("switch change")
                        self._switchOut.write()

                #switch judg
                if(GPIO.input(pin)):
                       print("=====config start========")
                       self._switch.data = True
                       OpenRTM_aist.setTimestamp(self._switch)
                       running = True
                else:
                        print("=====config stop========")
                        self._switch.data = False
                        OpenRTM_aist.setTimestamp(self._switch)
                        running = False
                        
                retval = accget.readData()
                
                print("ACC: {}".format(retval))
                self._accdata.data = retval
                
                OpenRTM_aist.setTimestamp(self._accdata)
                self._Acc_outOut.write()
                        
		return RTC.RTC_OK
	
	#	##
	#	#
	#	# The aborting action when main logic error occurred.
	#	# former rtc_aborting_entry()
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onAborting(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	#	##
	#	#
	#	# The error action in ERROR state
	#	# former rtc_error_do()
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onError(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	#	##
	#	#
	#	# The reset action that is invoked resetting
	#	# This is same but different the former rtc_init_entry()
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onReset(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	#	##
	#	#
	#	# The state update action that is invoked after onExecute() action
	#	# no corresponding operation exists in OpenRTm-aist-0.2.0
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#

	#	#
	#def onStateUpdate(self, ec_id):
	#
	#	return RTC.RTC_OK
	
	#	##
	#	#
	#	# The action that is invoked when execution context's rate is changed
	#	# no corresponding operation exists in OpenRTm-aist-0.2.0
	#	#
	#	# @param ec_id target ExecutionContext Id
	#	#
	#	# @return RTC::ReturnCode_t
	#	#
	#	#
	#def onRateChanged(self, ec_id):
	#
	#	return RTC.RTC_OK
	



def AccelerationInit(manager):
    profile = OpenRTM_aist.Properties(defaults_str=acceleration_spec)
    manager.registerFactory(profile,
                            Acceleration,
                            OpenRTM_aist.Delete)

def MyModuleInit(manager):
    AccelerationInit(manager)

    # Create a component
    comp = manager.createComponent("Acceleration")

def main():
	mgr = OpenRTM_aist.Manager.init(sys.argv)
	mgr.setModuleInitProc(MyModuleInit)
	mgr.activateManager()
	mgr.runManager()

if __name__ == "__main__":
	main()

