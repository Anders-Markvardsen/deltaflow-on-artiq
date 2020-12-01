BRUN = docker exec -w /workdir/build \
   $(shell cat container)

ONESHELL: build/build-runtime
build/build-deltasimulator: container
	mkdir -p build
	${DEXEC} python3 demos/deltasimulator/builder.py
	touch build/build-deltasimulator

ONESHELL: run-deltasimulator 
run-deltasimulator: build/build-deltasimulator
	${BRUN} chmod +x ./main 
	${BRUN} ./main

