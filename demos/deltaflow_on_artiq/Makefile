
.ONESHELL: build/test_deltaflow_kasli
build/test_deltaflow_kasli: demos/deltaflow_on_artiq/deltaflow_emulator.cpp \
	emulator/artiq_emulator.hpp \
	build/or1kISAmodel.o \
	build/verilated/Vtop.h \
	build/verilated/Vtop__ALL.a \
	build/build-deltasimulator \
	build/textrst.bin \
	build/flash_storage.img container
	${DEXEC} python3 /workdir/demos/deltaflow_on_artiq/builder.py


BRUN = docker exec -w /workdir/build \
   $(shell cat container)


.ONESHELL: run-kasli-emulator
run-kasli-emulator: build/test_deltaflow_kasli \
	              build/artiq_kasli/software/bootloader/bootloader.elf \
								build/textrst.bin \
								emulator/or1ksim-kasli.cfg
	${DEXEC} chmod +x $<
	${BRUN} rm -rf logs  
	${BRUN} mkdir logs 
	${BRUN} ./test_deltaflow_kasli --duration 3800
