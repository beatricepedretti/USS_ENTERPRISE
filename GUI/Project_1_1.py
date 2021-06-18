from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.textinput import TextInput
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


class Container(BoxLayout):

    connect_btn = ObjectProperty(None)
    #com_ports_spinner = ObjectProperty(None)
    start_btn = ObjectProperty(None)
    stop_btn = ObjectProperty(None)
    one_btn = ObjectProperty(None)
    five_btn = ObjectProperty(None)
    ten_btn = ObjectProperty(None)
    debug_label = ObjectProperty(None)

    #NON SONO SICURO QUESTA FUNZIONE SERVA
    def __init__(self, **kwargs):
        '''super().__init__(**kwargs)
        self.isPortConnected = False
        self.portConnected = None'''
        self.connected = False
        super(Container, self).__init__(*kwargs)

    #DA VEDERE PERCHè è STATA FATTA
    '''def on_Connect_callback(self):
        connection_thread = threading.Thread(target=self.connectionProcess)
        connection_thread.start()'''
    
    '''def on_com_ports_spinner(self, instance, value):
        ports = [port.device for port in comports()]
        self.com_ports_spinner.values = ports
        if len(ports) > 0:
            self.com_ports_spinner.text = ports[0]
        else:
            self.com_ports_spinner.text = 'No COM port found'
            self.com_ports_spinner.disabled = True'''

    '''def button_pressed_callback(self):
        if (len(self.com_ports_spinner.values) == 0):
            print('No COM port selected')
            return
        #if len(self.com_ports_spinner.values)
        if (not self.connected):
            try:
                self.ser = serial.Serial(
                    port=self.com_ports_spinner.text, baudrate=115200)
                if (self.ser.is_open):
                    self.debug_label.text = "Connection successful"
                    self.connected = True
                    self.connect_btn.text = 'Disconnect'
                    self.com_ports_spinner.disabled = True
            except SerialException:
                self.debug_label.text = "Could not connect to serial port"
                #print("Could not connect to serial port")
        else:
            self.ser.close()
            self.debug_label.text = "Disconnection successful"
            self.connected = False
            self.connect_btn.text = 'Connect'
            self.com_ports_spinner.disabled = False'''


    #QUESTA è DA VEDERE, CREEREBBE LA POSSIBILITà DI CONNESSIONE LEGGENDO QUELLO CHE ARRIVA DA PSOC      
    
    #CONTROLLARE CHE FUNZIONINO

    def threading_connection(self):
        threading.Thread(target=self.connectionProcess).start()

    def threading_start(self):
        threading.Thread(target=self.startProcess).start()
    
    def threading_stop(self):
        threading.Thread(target=self.stopProcess).start()
    
    def threading_one(self):
        threading.Thread(target=self.oneProcess).start()

    def threading_five(self):
        threading.Thread(target=self.fiveProcess).start()

    def threading_ten(self):
        threading.Thread(target=self.tenProcess).start()


    def connectionProcess(self):
        if(self.connect_btn.text == 'Connect'):
            self.debug_label.text = 'Connecting..'
            ports_info = list_ports.comports()
            for element in ports_info:
                if ('Bluetooth' not in element.description):
                    self.ser = serial.Serial(port=element.name, baudrate=115200, timeout=0.5)
                    self.ser.write("v".encode())
                    time.sleep(1)
                    string = 0
                    string = self.ser.read_until("$".encode()).decode()
               

                    if(string == "Device succesfully connected$"):
                        self.isPortConnected = True
                        self.portConnected = element.name
                        #self.gridlayout_btns.disabled = False
                        #self.gridlayout_config_btns.disabled = False
                        '''self.connect_btn.text = 'Disconnect'
                        self.debug_label.text = 'Connected through port '+self.portConnected
                        self.ser.close()'''
                        time.sleep(1)
            self.ser.close()
            self.ser=serial.Serial(port=self.portConnected, baudrate=115200)
            if(self.ser.is_open):
                self.connect_btn.text = 'Disconnect'
                self.debug_label.color = (61/255, 235/255, 52/255, 1)
                self.debug_label.text = 'Connected through port '+self.portConnected


        elif(self.connect_btn.text == 'Disconnect'):
            self.ser.close()
            self.ser = serial.Serial(port=self.portConnected, baudrate=115200)            
            self.isPortConnected = False
            self.portConnected = None
            '''self.gridlayout_btns.disabled = True
            self.gridlayout_config_btns.disabled = True'''
            self.connect_btn.text = 'Connect'
            self.debug_label.color = (250/255, 250/255, 250/255, 1)
            self.debug_label.text = 'Waiting for connection'
            self.ser.close()

    def startProcess(self):
        if(self.start_btn.text == 'Start'):
            self.ser.write("b".encode())
            time.sleep(1)    

    def stopProcess(self):
        if(self.stop_btn.text == 'Stop'):
            self.ser.write('s'.encode())
            time.sleep(1)    
    
    def oneProcess(self):
        if(self.one_btn.text == '1'):
            self.ser.write('1'.encode())
            time.sleep(1)    
    
    def fiveProcess(self):
        if(self.five_btn.text == '5'):
            self.ser.write('5'.encode())
            time.sleep(1)    

    
    def tenProcess(self):
        if(self.ten_btn.text == '10'):
            self.ser.write('10'.encode())
            time.sleep(1)    
        

class GuiApp(App):
    def build(self):
        return Container()


GuiApp().run()






