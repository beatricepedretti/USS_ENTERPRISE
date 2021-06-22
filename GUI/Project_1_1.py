from kivy.app import App
from kivy.lang import Builder
from kivy.uix.widget import Widget
from kivy.core.window import Window
from kivy.uix.actionbar import ActionButton
from kivy.uix.label import Label
from kivy.clock import Clock
from kivy.uix.boxlayout import BoxLayout
from kivy.properties import ObjectProperty
from serial.serialutil import SerialException
from serial.serialwin32 import Serial
from serial.tools.list_ports import comports
import serial
from serial import Serial
from serial.tools import list_ports
import threading
import time
from kivy.uix.textinput import TextInput
from kivy.uix.image import Image


class Container(BoxLayout):

    connect_btn = ObjectProperty(None)
    #com_ports_spinner = ObjectProperty(None)
    start_btn = ObjectProperty(None)
    stop_btn = ObjectProperty(None)
    one_btn = ObjectProperty(None)
    five_btn = ObjectProperty(None)
    ten_btn = ObjectProperty(None)
    profile_image = ObjectProperty(None)
    debug_label = ObjectProperty(None)

    # NON SONO SICURO QUESTA FUNZIONE SERVA
    def __init__(self, **kwargs):
        '''super().__init__(**kwargs)
        self.isPortConnected = False
        self.portConnected = None'''
        self.connected = False
        self.ser = None
        self.do_read = False
        super(Container, self).__init__(*kwargs)

    def threading_connection(self):
        threading.Thread(target=self.connectionProcess).start()

    def threading_start(self):
        threading.Thread(target=self.startProcess).start()

    def threading_stop(self):
        threading.Thread(target=self.stopProcess).start()

    def threading_step(self, step):
        threading.Thread(target=self.step_setter, args=(step, )).start()

    def threading_ten(self):
        threading.Thread(target=self.tenProcess).start()

    def connectionProcess(self):
        if(self.connect_btn.text == 'Connect'):
            self.debug_label.text = 'Connecting...'
            ports_info = list_ports.comports()
            for element in ports_info:
                if ('Bluetooth' not in element.description):
                    try:
                        self.ser = serial.Serial(
                            port=element.name, baudrate=115200, timeout=0.5)
                        self.ser.write("v".encode())
                        time.sleep(1)
                        string = 0
                        string = self.ser.read_until("$".encode()).decode()

                        if(string == "Device succesfully connected$"):
                            self.isPortConnected = True
                            self.portConnected = element.name
                            time.sleep(1)
                            break
                        self.ser = None
                    except:
                        print(f"Error opening port {element.name}")

            if(self.ser and self.ser.is_open):
                self.ser.timeout = 0
                self.connect_btn.text = 'Disconnect'
                self.debug_label.color = (61/255, 235/255, 52/255, 1)
                self.debug_label.text = 'Connected through port '+self.portConnected
                self.do_read = True

                # Enable all buttons after connecting
                self.connect_btn.disabled = False
                self.five_btn.disabled = False
                self.ten_btn.disabled = False
                self.start_btn.disabled = False
                self.stop_btn.disabled = True
            else:
                self.connect_btn.text = 'Connect'
                self.debug_label.color = (250/255, 250/255, 250/255, 1)
                self.debug_label.text = 'Waiting for connection'

        elif(self.connect_btn.text == 'Disconnect'):
            if self.ser != None:
                self.ser.close()
                self.ser = None
            self.isPortConnected = False
            self.portConnected = None
            self.connect_btn.text = 'Connect'
            self.debug_label.color = (250/255, 250/255, 250/255, 1)
            self.debug_label.text = 'Waiting for connection'

            # Disable all buttons after disconnecting
            self.connect_btn.disabled = True
            self.five_btn.disabled = True
            self.ten_btn.disabled = True
            self.start_btn.disabled = True
            self.stop_btn.disabled = True

    def reader(self):
        with open("coordinates.xyz", "w") as output_file:
            while self.do_read:
                # Wait until there is data waiting in the serial buffer
                if self.ser.in_waiting > 0:

                    # Read data out of the buffer until a carraige return / new line is found
                    #serialString = self.ser.read(self.ser.in_waiting)
                    serialString = self.ser.readline()
                    #serialString = self.ser.read_until("\n".encode())
                    
                    # Print the contents of the serial data
                    try:
                        line = serialString.decode("utf-8").rstrip()
                        print(line)
                        output_file.write(line + '\n')
                        self.ser.flushInput()
                        
                    except:
                        pass
                time.sleep(0.001)

    def startProcess(self):
        if(self.start_btn.text == 'Start'):
            self.do_read = True
            self.connect_btn.disabled = True
            self.five_btn.disabled = True
            self.ten_btn.disabled = True
            self.start_btn.disabled = True
            self.stop_btn.disabled = False
            self.ser.write("b".encode())
            #self.profile_image.source = r'images/sonar.jpg'
            #Clock.schedule_interval(lambda dt: self.profile_image.reload(), 0.2)
            threading.Thread(target=self.reader).start()

    def stopProcess(self):
        if(self.stop_btn.text == 'Stop'):
            self.do_read = False
            self.connect_btn.disabled = False
            self.five_btn.disabled = False
            self.ten_btn.disabled = False
            self.start_btn.disabled = False
            self.stop_btn.disabled = True
            self.ser.write('s'.encode())        
            time.sleep(1)

    def step_setter(self, step):
        print(f"Setting step {step}")
        self.ser.write(step.encode())
        time.sleep(1)


class GuiApp(App):
    def build(self):
        return Container()


GuiApp().run()




