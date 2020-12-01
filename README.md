# Deltaflow-on-ARTIQ

[Deltaflow-on-ARTIQ repo](https://github.com/riverlane/deltaflow-on-artiq) -
[![Build Status](http://jenkins-riverlane.northeurope.cloudapp.azure.com/buildStatus/icon?job=deltaflow-on-artiq_multibranch_build%2Fdev)](http://jenkins-riverlane.northeurope.cloudapp.azure.com/job/deltaflow-on-artiq_multibranch_build/job/dev/) -
[docs](https://riverlane.github.io/deltaflow-on-artiq):

- [Deltalanguage repo](https://github.com/riverlane/deltalanguage) -
[![Build Status](http://jenkins-riverlane.northeurope.cloudapp.azure.com/buildStatus/icon?job=deltalanguage_multibranch_build%2Fdev)](http://jenkins-riverlane.northeurope.cloudapp.azure.com/job/deltalanguage_multibranch_build/job/dev/) -
[docs](https://riverlane.github.io/deltalanguage)

- [Deltasimulator repo](https://github.com/riverlane/deltasimulator) -
[![Build Status](http://jenkins-riverlane.northeurope.cloudapp.azure.com/buildStatus/icon?job=deltasimulator_multibranch_build%2Fdev)](http://jenkins-riverlane.northeurope.cloudapp.azure.com/job/deltasimulator_multibranch_build/job/dev/) - 
[docs](https://riverlane.github.io/deltasimulator)

- [Deltamodels repo](https://github.com/riverlane/deltamodels) -
[![Build Status](http://jenkins-riverlane.northeurope.cloudapp.azure.com/buildStatus/icon?job=deltamodels_multibranch_build%2Fdev)](http://jenkins-riverlane.northeurope.cloudapp.azure.com/job/deltamodels_multibranch_build/job/dev/) -
[docs](https://riverlane.github.io/deltamodels)


## Intro

ARTIQ Emulator is an [M-Labs](https://m-labs.hk) -
[ARTIQ](https://github.com/m-labs/artiq/tree/master/artiq)
emulator integrated to our Deltaflow framework.

Deltaflow let's you design heterogeneous applications in a modular fashion.
It let you squeeze the most out of your hardware using a simple language and
it gives you the flexibility needed to integrate to your existing control stack.

## Prerequisite

The `Artiq Emulator` environment has been tested on Linux and Mac. Windows is currently not supported. 
`Artiq Emulator` relies almost exclusively on Docker (Docker Desktop on MAC) to maximise portability. 
Said that, we recommend a relatively recent machine to go through the various compilation steps.

## Installing ARTIQ Emulator

As a first step you might need to init/update all the submodules.

```console
river@lane:~$ cd deltaflow-on-artiq
river@lane:~/deltaflow-on-artiq$ git submodule update --init --recursive
```

To create all the docker images you can run:

```console
river@lane:~$ cd deltaflow-on-artiq
river@lane:~/deltaflow-on-artiq$ make run-kasli-emulator
```


The creation of the docker containers currently takes about 2-3 hrs on standard machines.

## Using ARTIQ Emulator

The user can execute three primary [demos](demos/README.md) that show a typical Deltaflow
development process

## License

Please refer to the respective licenses:

- [Artiq Emulator](LICENSE.rst)

- [Deltalanguage](https://github.com/riverlane/deltalanguage/blob/main/LICENSE.rst)

- [Deltasimulator](https://github.com/riverlane/deltasimulator/blob/main/LICENSE.rst)

- [Deltamodels](https://github.com/riverlane/deltamodels/blob/main/LICENSE.rst)
