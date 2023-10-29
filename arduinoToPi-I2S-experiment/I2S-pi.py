import serial
import sounddevice as sd


# Open serial port (change 'COM3' to the correct port)
ser = serial.Serial('COM4', 115200)  

# Buffer for storing analog values
analog_values = bytearray()

# Function to play sound from analog values
def play_sound(data, samplerate):
    sd.play(data, samplerate)
    sd.wait()

# Read and process data from serial
while True:
    if ser.in_waiting > 0:
        value = ser.read(2)  
        analog_values.extend(value)
        if len(analog_values) >= 44100:  
            play_sound(analog_values, 44100)  
            analog_values = bytearray()  
