FROM ubuntu:bionic

RUN set -e -x ;\
    export DEBIAN_FRONTEND=noninteractive ;\
    apt-get update -y ;\
    apt-get install -y \
        curl \
        ca-certificates \
        clang-format \
        git \
        python3 \
        python3-pip ;\
    python3 -m pip install black ;\
    rm -rf /var/lib/apt/lists

