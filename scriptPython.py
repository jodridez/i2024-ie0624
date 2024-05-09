import serial  # Para la comunicación serial con Arduino
import csv  # Para manejar archivos CSV
from datetime import datetime  # Para obtener el tiempo actual
import matplotlib.pyplot as plt  # Para crear gráficos
from matplotlib.animation import FuncAnimation  # Para actualizar los gráficos en tiempo real

# Para la configuración de la conexión serial
ser = serial.Serial('COM3', 9600, timeout=1)  # Abre el puerto serial COM3 a 9600 baudios, ajusta según tu configuración
data_file = 'egg_incubator_data.csv'  # Nombre del archivo CSV para guardar los datos

# Para la configuración inicial de la gráfica

fig, ax = plt.subplots()  # Crea una figura y un eje para graficar
x_data, y_setpoint, y_pid_output, y_temp = [], [], [], []  # Listas para almacenar los datos del gráfico

# Función par aanimar y procesar datos
def animate(i):
    # Se ejecuta esta función en intervalos para leer datos del puerto serial y actualizar el gráfico
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()  # Lee una línea desde el puerto serial
        if line:
            data = line.split(',')  # Separa los datos en base a comas
            if len(data) == 3:
                temp, setpoint, pid_output = map(float, data)  # Asigna los datos a variables y convierte a float
                timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')  # Obtiene el tiempo actual
                
                # Guarda los datos en un archivo CSV
                with open(data_file, 'a', newline='') as file:
                    writer = csv.writer(file)
                    writer.writerow([timestamp, temp, setpoint, pid_output])
                
                # Agrega los nuevos datos a las listas para graficar
                x_data.append(timestamp)
                y_temp.append(temp)
                y_setpoint.append(setpoint)
                y_pid_output.append(pid_output)
                
                # Limpia el gráfico y dibuja las nuevas líneas
                ax.clear()
                ax.plot(x_data, y_temp, label='Temperatura')
                ax.plot(x_data, y_setpoint, label='Setpoint')
                ax.plot(x_data, y_pid_output, label='Salida PID')
                ax.legend(loc='upper right')
                ax.set_title('Monitoreo de Temperatura de Incubadora de Huevos')
                ax.set_xlabel('Tiempo')
                ax.set_ylabel('Temperatura (°C)')
                plt.xticks(rotation=45, ha='right')  # Rota las etiquetas del eje x para mejor legibilidad


# Prepara el archivo CSV escribiendo los encabezados
with open(data_file, 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['Timestamp', 'Temperature', 'Setpoint', 'PID Output'])

# Crea la animación, llamando a la función animate cada 1000 milisegundos
ani = FuncAnimation(fig, animate, interval=1000)
plt.show()  # Muestra la ventana de gráficos

