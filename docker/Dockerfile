FROM debian:11

LABEL mantainer="Seobryn <ing.jose.joya@gmail.com>"

RUN apt-get update && apt-get install git make cmake gcc g++ clang default-libmysqlclient-dev libssl-dev libbz2-dev libreadline-dev libncurses-dev libboost-all-dev wget default-mysql-client -y

WORKDIR /firelands/Firelands-Core
WORKDIR /firelands
RUN mkdir release
COPY ./ /firelands/Firelands-Core
WORKDIR /firelands/

WORKDIR /firelands/Firelands-Core/build
RUN cmake ../ \
    -DCMAKE_INSTALL_PREFIX=../../release/ \
    -DCMAKE_C_COMPILER=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++ \
    -DWITH_WARNINGS=1 \
    -DSERVERS=1 \
    -DCMAKE_BUILD_TYPE=Debug \
    -DTOOLS=0

RUN make -j 4 && make install
WORKDIR /firelands/release/bin

ENTRYPOINT [ "./worldserver" ]
