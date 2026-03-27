import serial
import openpyxl
import time
import math
from itertools import zip_longest

# Configura el puerto serial
ser = serial.Serial('COM8', 115200)  # Reemplaza COM3 por tu puerto
time.sleep(2)  # Espera a que Arduino reinicie

# Crear archivo Excel
wb = openpyxl.Workbook()
ws = wb.active
ws.title = "Bits Interrumpidos"
#ws.append(["ON", "OFF","Decrease","Increase","White","Blue","Green","Red"])

# Inicializa variables
promedio = 0
contador = -1
Secuencia = [[],[],[],[],[],[],[],[],[],[],[]]
Bits = [[],[],[],[],[],[],[],[],[],[]]

def cantidad_de_bits(n):
    if 8 < n :
        return 0
    elif 1.68 > n:
        return 1
    else:
        return 2
        
    

try:
    for i in range(300):  # Número de cambios a registrar

        line = ser.readline().decode('utf-8').strip()

        if i == 0: # Tiempo en milisegundos
            start_time = time.perf_counter()
            time_anterior_ms = start_time
            
        time_actual_ms = (time.perf_counter() - start_time) * 1000

        diferencia_tmp = round(abs(time_actual_ms - time_anterior_ms),2)
        
        if diferencia_tmp > 500:
            contador = contador + 1
            diferencia_tmp = 0
        elif diferencia_tmp > 5000:
            break

        time_anterior_ms = time_actual_ms

        Secuencia[contador].append(diferencia_tmp)

    #print(Secuencia)
                        
    print(Secuencia)
    
    for k in range(10):
        Bits[k].append(1)
        for i in range (0, len(Secuencia[k])):
            
            if cantidad_de_bits(Secuencia[k][i]) == 0:
                for j in range(0,16):
                    Bits[k].append(0)
                for j in range(0,8):
                    Bits[k].append(1)

            if cantidad_de_bits(Secuencia[k][i]) == 1:
                Bits[k].append(0)
                Bits[k].append(1)

            if cantidad_de_bits(Secuencia[k][i]) == 2:
                Bits[k].append(0)
                for j in range(0,3):
                    Bits[k].append(1)


    print(Bits[1])
    matriz_transpuesta = list(zip_longest(*Bits))

    for fila in matriz_transpuesta:
        ws.append(fila)
    #for i in range (0, 50):
    #        ws.append(Bits[1])


         
    print(len(Bits[0]))
    wb.save("Muetreo de secuencias NEC.xlsx")
    print("Archivo guardado: secuencia de bits.xlsx")       

except KeyboardInterrupt:
    print("Cancelado por el usuario.")

finally:
    ser.close()
