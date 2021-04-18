FROM ubuntu

RUN apt-get -y update && apt-get install -y
RUN apt-get -y install gcc
RUN apt-get -y install clang
RUN apt-get -y install cmake

WORKDIR /app

COPY cmake-build-debug/web ./web
COPY src ./src
COPY tests ./tests
COPY CMakeLists.txt .
COPY main.cpp .
COPY http_client.cpp .
COPY config.json .

RUN cmake .
RUN cmake --build .

EXPOSE 8001

CMD ["./P8_Web_Server_run"]