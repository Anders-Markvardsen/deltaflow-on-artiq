from artiq.experiment import *


class IdleKernel(EnvExperiment):
    def build(self):
        self.setattr_device("core")

    @kernel
    def run(self):
        print("Running idle kernel")
