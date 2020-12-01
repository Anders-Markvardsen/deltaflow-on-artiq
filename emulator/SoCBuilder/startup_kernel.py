from artiq.experiment import *
from artiq.coredevice.hal import *

class StartupKernel(EnvExperiment):
    def build(self):
        self.setattr_device("core")
        self.setattr_device("quantum_cmd")
        self.setattr_device("quantum_valid")

    @kernel
    def send_cmd(self, cmd):
        for i in range(32):
            with parallel:
                if (cmd >> i) & 0x1:
                    self.quantum_cmd.on()
                else:
                    self.quantum_cmd.off()
                self.quantum_valid.on()
                delay(0.1*us)
            self.quantum_valid.off()
            delay(10*us)

    @kernel
    def run(self):
        print ("Retrieving 10 Rabi commands and sending them to the I/Os")
        cnt=0
        hal_write(1)
        val=hal_read()
        self.core.reset()
        delay(150*us)
        while ((val != 0) and (cnt < 10)):
            self.send_cmd(val)
            hal_write(1)
            val=hal_read()
            cnt=cnt+1
        print("Startup kernel completed")
