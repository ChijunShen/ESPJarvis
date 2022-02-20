import psutil
import time
import socket
import paho.mqtt.client as paho
import wmi
import ctypes

# getting the library in which GetTickCount64() resides
lib = ctypes.windll.kernel32
# MQTT server configuration
mqtt_host = "192.168.0.17"
mqtt_user = "test"
mqtt_password = "2022"
mqtt_port = "1883"
mqtt_topic_prefix = "pc"
# This is the time  between sending the individual messages
sleep_time = 0.1
send_cpu_load = False
send_cpu_temp = True
send_used_space = False
send_sys_clock_speed = True
send_swap = False
send_memory = False
send_uptime = False
send_gpu_freq = False
cpu_core_number = 6
used_space = sys_clock_speed = swap = memory = uptime_hours = gpu_freq = 0
cpu_clock = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
cpu_load = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
cpu_temp = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]


def getSystemPara():
    w = wmi.WMI(namespace="root\OpenHardwareMonitor")
    hardware_infos = w.Sensor()
    global cpu_temp, used_space, sys_clock_speed, swap, memory, uptime_hours, gpu_freq
    max_cpu_temperature = 0
    max_cpu_clock = 0
    for sensor in hardware_infos:
        # print(sensor.SensorType)
        if sensor.SensorType == "Temperature":
            if "CPU Core #1" in sensor.Name:
                cpu_temp[1] = int(sensor.Value)
            elif "CPU Core #2" in sensor.Name:
                cpu_temp[2] = int(sensor.Value)
            elif "CPU Core #3" in sensor.Name:
                cpu_temp[3] = int(sensor.Value)
            elif "CPU Core #4" in sensor.Name:
                cpu_temp[4] = int(sensor.Value)
            elif "CPU Core #5" in sensor.Name:
                cpu_temp[5] = int(sensor.Value)
            elif "CPU Core #6" in sensor.Name:
                cpu_temp[6] = int(sensor.Value)
        if sensor.SensorType == "Load":
            if "CPU Core #1" in sensor.Name:
                cpu_load[1] = int(sensor.Value)
            elif "CPU Core #2" in sensor.Name:
                cpu_load[2] = int(sensor.Value)
            elif "CPU Core #3" in sensor.Name:
                cpu_load[3] = int(sensor.Value)
            elif "CPU Core #4" in sensor.Name:
                cpu_load[4] = int(sensor.Value)
            elif "CPU Core #5" in sensor.Name:
                cpu_load[5] = int(sensor.Value)
            elif "CPU Core #6" in sensor.Name:
                cpu_load[6] = int(sensor.Value)
        if sensor.SensorType == "Clock":
            if "CPU Core" in sensor.Name:
                max_cpu_clock = max(max_cpu_clock, sensor.Value)
            if "CPU Core #1" in sensor.Name:
                cpu_clock[1] = int(sensor.Value)
            elif "CPU Core #2" in sensor.Name:
                cpu_clock[2] = int(sensor.Value)
            elif "CPU Core #3" in sensor.Name:
                cpu_clock[3] = int(sensor.Value)
            elif "CPU Core #4" in sensor.Name:
                cpu_clock[4] = int(sensor.Value)
            elif "CPU Core #5" in sensor.Name:
                cpu_clock[5] = int(sensor.Value)
            elif "CPU Core #6" in sensor.Name:
                cpu_clock[6] = int(sensor.Value)
        if sensor.SensorType == "Load":
            if sensor.Name == "Memory":
                memory = format(sensor.Value, '.1f')
    sys_clock_speed = int(max_cpu_clock)
    # calling the function and storing the return value
    t = lib.GetTickCount64()
    #print(t)
    # since the time is in milliseconds i.e. 1000 * seconds
    # therefore truncating the value
    #t = int(str(t)[:-3])
    uptime_hours = int(t / 3600)
    #print(uptime_hours)


def publishToMqtt(cpu_load=0, cpu_temp=0, used_space=0, sys_clock_speed=0, swap=0, memory=0,
                  uptime_hours=0, gpu_freq=0):
    global cpu_core_number, client

    # publish monitored values to MQTT
    if send_cpu_load:
        for i in range(1, cpu_core_number + 1):
            if cpu_load[i] != 0:
                client.publish(mqtt_topic_prefix + "/cpuload_" + str(i), cpu_load[i], qos=0)
                time.sleep(sleep_time)
                # print("CPU" + str(i) + " Load: " + str(cpu_load[i]))
        time.sleep(sleep_time)
    if send_cpu_temp:
        for i in range(1, cpu_core_number + 1):
            if cpu_temp[i] != 0:
                client.publish(mqtt_topic_prefix + "/cputemp_" + str(i), cpu_temp[i], qos=0)
                time.sleep(sleep_time)
                # print("CPU" + str(i) + " Temp: " + str(cpu_temp[i]))
    if send_used_space:
        client.publish(mqtt_topic_prefix + "/diskusage", used_space, qos=0)
        time.sleep(sleep_time)
    if send_swap:
        client.publish(mqtt_topic_prefix + "/swap", swap, qos=0)
        time.sleep(sleep_time)
    if send_memory:
        client.publish(mqtt_topic_prefix + "/memory", memory, qos=0)
        time.sleep(sleep_time)
    if send_sys_clock_speed:
        # client.publish(mqtt_topic_prefix + "/sys_clock_speed", sys_clock_speed, qos=0)
        # time.sleep(sleep_time)
        for i in range(1, cpu_core_number+1):
            if cpu_clock[i] != 0:
                client.publish(mqtt_topic_prefix + "/cpuclock_" + str(i), cpu_clock[i], qos=0)
                # print(mqtt_topic_prefix + "/cpuclock_" + str(i)+" "+str(cpu_clock[i]))
                time.sleep(sleep_time)
    if send_uptime:
        client.publish(mqtt_topic_prefix + "/uptime_hours", uptime_hours, qos=0)
        time.sleep(sleep_time)
    if send_gpu_freq:
        client.publish(mqtt_topic_prefix + "/gpu_freq", gpu_freq, qos=0)
        time.sleep(sleep_time)


if __name__ == '__main__':
    client = paho.Client()
    client.username_pw_set(mqtt_user, mqtt_password)
    client.connect(mqtt_host, int(mqtt_port))
    while (True):
        # connect to mqtt server
        getSystemPara()
        publishToMqtt(cpu_load, cpu_temp, used_space, sys_clock_speed, swap, memory, uptime_hours, gpu_freq)
        time.sleep(3)
