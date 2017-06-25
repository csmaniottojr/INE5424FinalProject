import serial, time, sys, traceback
from utils.observable import Observable
from utils.message_validator import MessageValidator

class SerialPortManager(Observable):
    def __init__(self, port, baudrate, write_timeout):
        Observable.__init__(self)
        self.port = port
        self.baudrate = baudrate
        self.write_timeout = write_timeout
        self.ser = None

    def run(self):
        self.ser = serial.Serial()
        self.ser.port = self.port
        self.ser.baudrate = self.baudrate
        self.ser.write_timeout = self.write_timeout

        try:
            self.open_serial_port()
            
            print("Esperando por mensagens...")
            while True:
                line = bytearray()
                while not self.is_end_of_line(line):
                    _byte = self.ser.read()

                    # Ignora caso nao seja o comeco de uma msg valida
                    if not line and not self.is_valid_start_of_message(_byte):
                        print (_byte)
                        continue
                    
                    line += bytearray(_byte)
                else:
                    self.update_observers(line)
        except KeyboardInterrupt:
            pass
        except:
            print(traceback.format_exc())
        finally:
            print("\nTerminando o programa...")
            if self.ser.is_open:
                self.ser.close()
            exit()
    
    def write(self, msg):
        if self.ser.is_open:
            try:
                self.ser.flushOutput()
                n = self.ser.write(msg)
                print("Escritos %d bytes na porta serial..." % n)
            except serial.SerialTimeoutException:
                print("\nWrite Timeout!\n")#TODO tentar de novo? Parar o app?
    
    def is_end_of_line(self, line):
        return MessageValidator.check_if_message_is_complete(line)
    
    def is_valid_start_of_message(self, start):
        return MessageValidator.check_if_is_valid_start(start)

    def open_serial_port(self):
        while not self.ser.is_open:
            try:
                print("Tentando conectar com a porta %s..." % self.port)
                self.ser.open()
            except serial.SerialException as msg:
                time.sleep(1)
