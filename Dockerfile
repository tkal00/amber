FROM ubuntu:24.04 AS gcc14
RUN apt-get update && \
        apt-get install -y g++-14 gcc-14 && \
        mv /usr/bin/g++-14 /usr/bin/g++ && \
        mv /usr/bin/gcc-14 /usr/bin/gcc
RUN apt-get install make
