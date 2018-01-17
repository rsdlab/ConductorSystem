#coding:utf-8
import smbus
import time
import math
import numpy as np
from numpy import linalg

bus = smbus.SMBus(1)
address = 0x6B
x_adr = 0x28
y_adr = 0x2A
z_adr = 0x2C
currentOrientation= [0 for i in range(3)]
currentAcceleration=[0 for i in range(3)]

inv_A = []

## i2c address set
def init_ADXL345():
    bus.write_byte_data(address,0x10,0x48) 

## senser read
def measure_acc(adr):
    acc0 = bus.read_byte_data(address, adr)
    acc1 = bus.read_byte_data(address, adr + 1)
    acc = (acc1 << 8) + acc0
    
    if acc > 0x7fff:
        acc=(65536-acc)*-1
        
    acc=acc*0.12/1000
    
    return acc

## Correction of X, Y and Z data in the direction of gravity
def roM(px,py,pz):
    Rx=np.array([[1,0,0],
                 [0,math.cos(px),-math.sin(px)],
                 [0,math.sin(px),math.cos(px)]])
    Ry=np.array([[math.cos(py),0,math.sin(py)],
                 [0,1,0],
                 [-math.sin(py),0,math.cos(py)]])
    Rz=np.array([[math.cos(pz),-math.sin(pz),0],
                 [math.sin(pz),math.cos(pz),0],
                 [0,0,1]])
    R=np.array([[math.cos(py)*math.cos(pz)-math.sin(px)*math.sin(py)*math.sin(pz),-math.cos(px)*math.sin(pz),math.sin(py)*math.cos(pz)+math.sin(px)*math.cos(py)*math.sin(pz)],
               [math.cos(py)*math.sin(pz)+math.sin(px)*math.sin(py)*math.cos(pz),math.cos(px)*math.cos(pz),math.sin(pz)*math.sin(py)-math.sin(pz)*math.cos(py)*math.cos(pz)],
               [-math.cos(px)*math.sin(py),math.sin(px),math.cos(px)*math.cos(py)]])

    R=Rz.dot(Rx).dot(Ry).dot(Rz)
    return R

def init_ACC():
    #set i2c
    global inv_A
    init_ADXL345()

    xi=0
    yj=0
    zk=0

    x_acc=measure_acc(x_adr)
    y_acc=measure_acc(y_adr)
    z_acc=measure_acc(z_adr)
    xi=xi+x_acc
    yj=yj+y_acc
    zk=zk+z_acc

    offsetx=xi/10.0
    offsety=yj/10.0
    offsetz=zk/10.0
    offset=(offsetx**2+offsety**2+offsetz**2)**0.5

    xx=(offsetx/offset)/-1.0
    yy=(offsety/offset)/-1.0
    zz=(offsetz/offset)/-1.0

    xthetaR=math.asin(xx)
    xtheta=xthetaR/math.pi*180
    yphiR=math.asin(yy)
    yphi=yphiR/math.pi*180
    zpsiR=math.acos(zz)
    zpsi=zpsiR/math.pi*180

    """
    print'xtheta=%3.2f'%xtheta
    print'yphi=%3.2f'%yphi
    print'zpsi=%3.2f'%zpsi
    """

    R=roM(xthetaR,yphi,zpsiR)
    
    ## Calculation of inverse matrix
    inv_A = linalg.inv(R)

    
def acc_Elements():
    global inv_A
    X_ACCMS=[]
    Y_ACCMS=[]
    Z_ACCMS=[]
    
    ## Acquisition of acceleration data
    x_acc=measure_acc(x_adr)
    y_acc=measure_acc(y_adr)
    z_acc=measure_acc(z_adr)
    
    ## Arranging acceleration data
    x_acc_B=np.array([x_acc,0,0])
    X_ACC=np.dot(inv_A,x_acc_B)
    y_acc_B=np.array([0,y_acc,0])
    Y_ACC=np.dot(inv_A,y_acc_B)
    z_acc_B=np.array([0,0,z_acc])
    Z_ACC=np.dot(inv_A,z_acc_B)

    currentOrientation[0]=X_ACC[0]*0.1+currentOrientation[0]*0.9
    currentOrientation[1]=Y_ACC[0]*0.1+currentOrientation[1]*0.9
    currentOrientation[2]=Z_ACC[0]*0.1+currentOrientation[2]*0.9

    currentAcceleration[0]=X_ACC[0]-currentOrientation[0]
    currentAcceleration[1]=Y_ACC[0]-currentOrientation[1]
    currentAcceleration[2]=Z_ACC[0]-currentOrientation[2]


    if currentAcceleration[0]<=0.08 and currentAcceleration[0]>=-0.08:
        currentAcceleration[0]=0
    if currentAcceleration[1]<=0.08 and currentAcceleration[0]>=-0.08:
        currentAcceleration[1]=0
    if currentAcceleration[2]<=0.08 and currentAcceleration[0]>=-0.08:
        currentAcceleration[2]=0

    X_ACCMS=currentAcceleration[0]*1.02*9.81
    Y_ACCMS=currentAcceleration[1]*1.02*9.81
    Z_ACCMS=currentAcceleration[2]*1.02*9.81

    elements = [X_ACCMS, Y_ACCMS, Z_ACCMS]
 
    return elements

def readData():
    accelements = acc_Elements()

    ACCMS_X = accelements[0]
    ACCMS_Y = accelements[1]
    ACCMS_Z = accelements[2]
    ACCMS=(ACCMS_X**2+ACCMS_Y**2+ACCMS_Z**2)**0.5
    #print 'AC=',ACCMS,'[m/s^2]'
    return ACCMS

try:
    init_ACC()
except ZeroDivisionError:
    print("ZeroDivisonError")

    
if __name__ == '__main__':
    try:
        readData()
    except KeyboardInterrupt:
        pass
    
   

