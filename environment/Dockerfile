FROM ubuntu:20.04
ENV BUILD_THREADS=4

ENV TZ=Europe/London
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

COPY ./environment/apt-list /opt/apt-list

# install all deps. We use a single apt call to improve first-build speed
# bash line removes lines with comments and newlines.
RUN apt-get update &&\
    cat /opt/apt-list | sed 's/#.*$//g' | sed '/^$/d' | tr '\n' ' ' | xargs apt-get install -y

# SystemC
RUN wget https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz &&\
    tar xvf systemc-2.3.3.tar.gz && cd systemc-2.3.3 && mkdir build && mkdir /usr/local/systemc-2.3.3 &&\
    cd build && ../configure --prefix=/usr/local/systemc-2.3.3 CXXFLAGS="-DSC_CPLUSPLUS=201703L -std=c++17" && \
    make -j ${BUILD_THREADS} && make install
ENV SYSTEMC_INCLUDE=/usr/local/systemc-2.3.3/include SYSTEMC_LIBDIR=/usr/local/systemc-2.3.3/lib-linux64 SYSTEMC_HOME=/usr/local/systemc-2.3.3
RUN echo $SYSTEMC_LIBDIR >> /etc/ld.so.conf && ldconfig
ENV PATH=/root/.local/bin:$PATH

# Verilator
RUN git clone https://git.veripool.org/git/verilator && \
    cd verilator && git checkout v4.026 && \
    autoconf && ./configure && make -j ${BUILD_THREADS} && make install

# MISOC, MIGEN, and DEPS OF SAME
# binutils and GCC for SoC designs. May need more than just this architecture.
RUN wget https://ftp.snt.utwente.nl/pub/software/gnu/binutils/binutils-2.32.tar.xz &&\
    tar xvf binutils-2.32.tar.xz &&\
    cd binutils-2.32 && mkdir build && cd build &&\
    ../configure --target=lm32-elf &&\
    make -j ${BUILD_THREADS} && make install

# get misoc and python deps of the same.
RUN git clone --recurse-submodules https://github.com/m-labs/misoc.git &&\
    git clone --recurse-submodules https://github.com/m-labs/migen.git &&\
    git clone https://github.com/m-labs/asyncserial.git &&\
    cd migen && git checkout 0.9.1 && python3 setup.py install &&\
    python3 -m pip install pyserial && cd /asyncserial && python3 setup.py install

# missing print parens warnings are normal - setuptools looking at too many files
RUN cd misoc && git checkout 0.12 && python3 setup.py install

# OR1K simulator - the CPU portion of ARTIQ can run here
RUN git clone https://github.com/openrisc/or1ksim.git
RUN apt install -y texinfo m4 automake libtool
RUN cd or1ksim && autoreconf -i && mkdir build && cd build && ../configure && make -j ${BUILD_THREADS} && make install

ENV OR1KSIM_HOME=/usr/local/lib

# we also need the toolchain to verify that we can get binaries for or1k
RUN wget https://github.com/openrisc/newlib/releases/download/v2.3.0-1/or1k-elf-multicore_gcc5.2.0_binutils2.26_newlib2.3.0-1_gdb7.11.tgz
RUN tar xvf or1k-elf-multicore_gcc5.2.0_binutils2.26_newlib2.3.0-1_gdb7.11.tgz
ENV PATH=/or1k-elf-multicore/bin:${PATH}
RUN echo $OR1KSIM_HOME >> /etc/ld.so.conf && ldconfig

# math lib - required for the quantum simulator
RUN git clone https://github.com/vsoftco/qpp.git && cd qpp &&\
    git checkout v1.2
ENV QPP_INCLUDE=/qpp/include
RUN wget https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.tar.bz2 && tar xvf eigen-3.3.7.tar.bz2
ENV EIGEN_INCLUDE=/eigen-eigen-323c052e1731

# misc additional includes. Add whever you need as loose headers here.
RUN mkdir misc_inc && cd misc_inc &&\
 wget https://raw.githubusercontent.com/tomparks/path-integral-quantum-simulator/master/src/cpp/sc_vardefs.h
ENV MISC_INC=/misc_inc

RUN wget https://raw.githubusercontent.com/enjoy-digital/litex/master/litex_setup.py
RUN chmod +x litex_setup.py
RUN ./litex_setup.py init install


# Hotfix for installing projectq https://github.com/ProjectQ-Framework/ProjectQ/issues/323
# pip tries to install pybind11 after projectq is installed,
# despite being one of projectq's dependencies
RUN pip3 install --no-cache-dir pybind11

# We are manually installing the package dependencies instead of importing the packages via pip to enable live
# changes.
COPY ./deltalanguage/environment/install_requirements.txt /opt/dl_install_requirements.txt
RUN pip3 install --upgrade pip && \
    pip3 install --no-cache-dir -r /opt/dl_install_requirements.txt
COPY ./deltalanguage/environment/dev_requirements.txt /opt/dl_dev_requirements.txt
RUN pip3 install --upgrade pip && \
    pip3 install --no-cache-dir -r /opt/dl_dev_requirements.txt
COPY ./deltalanguage/environment/visualisation_requirements.txt /opt/dl_visualisation_requirements.txt
RUN pip3 install --upgrade pip && \
    pip3 install --no-cache-dir -r /opt/dl_visualisation_requirements.txt
ENV PYTHONPATH=${PYTHONPATH}:/workdir/deltalanguage

ENV PYTHONPATH=${PYTHONPATH}:/workdir/deltasimulator
COPY ./deltasimulator/environment/install_requirements.txt /opt/ds_install_requirements.txt
# Remove deltalanguage dependencies
RUN sed '/deltalanguage/d' /opt/ds_install_requirements.txt >> /opt/ds_install_requirements_updated.txt  
RUN pip3 install --upgrade pip && \
    pip3 install --no-cache-dir -r /opt/ds_install_requirements_updated.txt
COPY ./deltasimulator/environment/dev_requirements.txt /opt/ds_dev_requirements.txt
RUN pip3 install --upgrade pip && \
    pip3 install --no-cache-dir -r /opt/ds_dev_requirements.txt
ENV PYTHONPATH=${PYTHONPATH}:/workdir/deltasimulator

# For GitHub Pages
COPY ./environment/dev_requirements.txt /opt/
RUN pip3 install --upgrade pip && \
    pip3 install --no-cache-dir -r /opt/dev_requirements.txt

ENV DEBIAN_FRONTEND=noninteractive

ARG USERNAME
ARG USER_UID
ARG USER_GID

# Create the user
RUN (groupadd --gid $USER_GID $USERNAME; exit 0) && \
    useradd --uid $USER_UID --gid $USER_GID -m $USERNAME && \
    mkdir -p /home/$USERNAME/.vscode-server /home/$USERNAME/.vscode-server-insiders && \
    chown ${USER_UID}:${USER_GID} /home/$USERNAME/.vscode-server*

# Add sudo support to install extra software if needed
RUN apt-get install -y sudo && \
    echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME && \
    chmod 0440 /etc/sudoers.d/$USERNAME

# Clean up
RUN apt-get autoremove -y && \
    apt-get clean -y && \
    rm -rf /var/lib/apt/lists/*

# Switch to the user
USER $USERNAME

ENV DEBIAN_FRONTEND=dialog 
ENV PYTHONPATH=/workdir/:${PYTHONPATH}:/workdir
