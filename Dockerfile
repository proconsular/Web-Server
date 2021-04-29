FROM ubuntu

RUN apt-get -y update && apt-get install -y
RUN apt-get -y install gcc
RUN apt-get -y install clang
RUN apt-get -y install cmake
RUN apt-get -y install build-essential checkinstall zlib1g-dev libtemplate-perl
RUN apt-get -y install wget
RUN wget -c "https://www.openssl.org/source/openssl-1.1.1k.tar.gz"
RUN tar -xzvf "openssl-1.1.1k.tar.gz"
WORKDIR "/openssl-1.1.1k/"
RUN ["./config", "--prefix=/usr/local/ssl", "--openssldir=/usr/local/ssl shared zl"]
RUN make
RUN make test
RUN make install
RUN export PATH="/usr/local/ssl/bin:${PATH}"

WORKDIR /app

COPY cmake-build-debug/web ./web
COPY src ./src
COPY tests ./tests
COPY lib ./lib
COPY CMakeLists.txt .
COPY main.cpp .
COPY http_client.cpp .
COPY config.json .

RUN cmake .
RUN cmake --build .

EXPOSE 8001

CMD ["./P8_Web_Server_run"]