FROM ubuntu:bionic

RUN set -e -x ;\
    export DEBIAN_FRONTEND=noninteractive ;\
    apt-get update -y ;\
    apt-get install -y \
        clang \
        git \
        libclang-dev \
        llvm \
        python3-pip ;\
    pip3 install \
        clang \
        sphinx \
        sphinx_rtd_theme ;\
    rm -rf /var/lib/apt/lists

