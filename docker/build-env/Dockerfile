FROM debian:stretch-backports

RUN set -e -x ;\
    export DEBIAN_FRONTEND=noninteractive ;\
    apt-get update -y ;\
    apt-get install -y \
        git \
        gcc-arm-none-eabi \
        python3-pip ;\
    apt-get install -y -t stretch-backports \
        meson ;\
    pip3 install crc16 ;\
    rm -rf /var/lib/apt/lists

