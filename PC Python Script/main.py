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
sleep_time = 0.5
send_cpu_load = True
send_cpu_temp = True
send_used_space = True
send_sys_clock_speed = True
send_swap = True
send_memory = True
send_uptime = True
send_gpu_freq = True

cpu_load = cpu_temp = used_space = sys_clock_speed = swap = memory = uptime_hours = gpu_freq = 0


def getSystemPara():
    w = wmi.WMI(namespace="root\OpenHardwareMonitor")
    hardware_infos = w.Sensor()
    global cpu_load, cpu_temp, used_space, sys_clock_speed, swap, memory, uptime_hours, gpu_freq
    max_cpu_temperature = 0
    max_cpu_clock = 0
    for sensor in hardware_infos:
        # print(sensor.SensorType)
        if sensor.SensorType == "Load":
            if sensor.Name == "CPU Total":
                cpu_load = format(sensor.Value, '.1f')
        if sensor.SensorType == "Temperature":
            if "CPU Core" in sensor.Name:
                max_cpu_temperature = max(max_cpu_temperature, sensor.Value)
        if sensor.SensorType == "Load":
            if sensor.Name == "Used Space":
                used_space = format(sensor.Value, '.1f')
        if sensor.SensorType == "Clock":
            if "CPU Core" in sensor.Name:
                max_cpu_clock = max(max_cpu_clock, sensor.Value)
        if sensor.SensorType == "Load":
            if sensor.Name == "Memory":
                memory = format(sensor.Value, '.1f')
    cpu_temp = format(max_cpu_temperature, '.1f')
    sys_clock_speed = int(max_cpu_clock)
    # calling the function and storing the return value
    t = lib.GetTickCount64()
    # since the time is in milliseconds i.e. 1000 * seconds
    # therefore truncating the value
    t = int(str(t)[:-3])
    uptime_hours = int(t / 3600)
    #print(uptime_hours)


def publishToMqtt(cpu_load=0, cpu_temp=0, used_space=0, sys_clock_speed=0, swap=0, memory=0,
                  uptime_hours=0, gpu_freq=0):
    # connect to mqtt server
    client = paho.Client()
    client.username_pw_set(mqtt_user, mqtt_password)
    client.connect(mqtt_host, int(mqtt_port))

    # publish monitored values to MQTT
    if send_cpu_load:
        client.publish(mqtt_topic_prefix + "/cpuload", cpu_load, qos=1)
        time.sleep(sleep_time)
    if send_cpu_temp:
        client.publish(mqtt_topic_prefix + "/cputemp", cpu_temp, qos=1)
        time.sleep(sleep_time)
    if send_used_space:
        client.publish(mqtt_topic_prefix + "/diskusage", used_space, qos=1)
        time.sleep(sleep_time)
    if send_swap:
        client.publish(mqtt_topic_prefix + "/swap", swap, qos=1)
        time.sleep(sleep_time)
    if send_memory:
        client.publish(mqtt_topic_prefix + "/memory", memory, qos=1)
        time.sleep(sleep_time)
    if send_sys_clock_speed:
        client.publish(mqtt_topic_prefix + "/sys_clock_speed", sys_clock_speed, qos=1)
        time.sleep(sleep_time)
    if send_uptime:
        client.publish(mqtt_topic_prefix + "/uptime_hours", uptime_hours, qos=1)
        time.sleep(sleep_time)
    if send_gpu_freq:
        client.publish(mqtt_topic_prefix + "/gpu_freq", gpu_freq, qos=1)
        time.sleep(sleep_time)
        # disconnect from mqtt server
    client.disconnect()


if __name__ == '__main__':
    while (True):
        getSystemPara()
        publishToMqtt(cpu_load, cpu_temp, used_space, sys_clock_speed, swap, memory, uptime_hours, gpu_freq)
        time.sleep(5)
