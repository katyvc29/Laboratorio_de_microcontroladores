'''
*  Nombre  : Katherine Vargas Castro                             *
*  Nombre  : Susan Jimenez Hernandez                             *
*  Curso   : Laboratorio de Microcontroladores IE-0624         	 *                                                                  *
*  MCU	   : Atmega328P                                          *
*  Fecha   : 2024                                                *                                                                  *
*  Labo3   : Incubadora                                          *
'''

# Se importa biblioteca tiempo y pyserial
import time
import serial

# Se definen variables
SCREEN_DISPLAY=True
SAVE_TO_FILE=True
DELIMITER=','

# Dirección del puerto serial de salida
SERIAL_PORT= 'COM2' 

# Nombre de archivo de salida y escritura del puerto ttyS1
file_name= 'labo3.csv' 
fid= open(file_name,'ab')


scale=serial.Serial(SERIAL_PORT,timeout=20,baudrate=9600)

# Loop para siempre hasta que el usuario termine el programa en terminal
while True:
    str_scale=scale.readline()
    time_now=time.strftime("%Y-%m-%d %H:%M:%S")

    if SCREEN_DISPLAY: print(str.encode(time_now+DELIMITER)+str_scale)
    time.sleep(0.01)  
    if SAVE_TO_FILE: fid.write(str.encode(time_now)+str_scale)

scale.close()
fid.close()
