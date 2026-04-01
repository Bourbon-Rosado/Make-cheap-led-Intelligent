import serial
import openpyxl
import time
import math
from itertools import zip_longest

# Configura el puerto serial
ser = serial.Serial('COM12', 9600)  
time.sleep(2)  

# Crear archivo Excel
wb = openpyxl.Workbook()
ws = wb.active
ws.title = "Bits Interrumpidos"

# Inicializa variables
CANTIDAD_LECTURAS = 300
promedio = 0
contador = -1
Secuencia = [[],[],[],[],[],[],[],[],[],[],[]]
Bits = [[],[],[],[],[],[],[],[],[],[]]

# Deside en funcion del tiempo si es un 0 o un 1 usando el protocolo NEC

def cantidad_de_bits(n):
    if 8 < n : # Frame inicial 9ms y 4.5ms
        return 2
    elif 1.68 > n: # 0 logico 
        return 0
    else:
        return 1  # 1  logico

try:
    for i in range(CANTIDAD_LECTURAS):  # Número de cambios a registrar

        line = ser.readline().decode('utf-8').strip()

        if i == 0: 
            start_time = time.perf_counter()
            time_anterior_ms = 0
            
        time_actual_ms = (time.perf_counter() - start_time) * 1000

        diferencia_tmp = round(abs(time_actual_ms - time_anterior_ms),3)
        
        if diferencia_tmp > 500:
            contador = contador + 1
            diferencia_tmp = 0
        elif diferencia_tmp > 5000:
            break

        time_anterior_ms = time_actual_ms
        Secuencia[contador].append(diferencia_tmp)

    for i in range(10):
        print(f'{Secuencia[i]}\n')
    
        
    for k in range(10):
        Bits[k].append(1)
        for i in range (0, len(Secuencia[k])):

            if cantidad_de_bits(Secuencia[k][i]) == 0:
                Bits[k].append(0)
                Bits[k].append(1)

            elif cantidad_de_bits(Secuencia[k][i]) == 1:
                Bits[k].append(0)
                for j in range(3):
                    Bits[k].append(1)
            else:
                for j in range(16):
                    Bits[k].append(0)
                for j in range(8):
                    Bits[k].append(1)


    matriz_transpuesta = list(zip_longest(*Bits))

    for fila in matriz_transpuesta:
        ws.append(fila)
         
    print(len(Bits[0]))
    wb.save("Muetreo de secuencias NEC.xlsx")
    print("Archivo guardado: Muetreo de secuencias NEC.xlsx")       

except KeyboardInterrupt:
    print("Cancelado por el usuario.")

finally:
    ser.close()
