FROM ubuntu:18.04
RUN  apt-get -y update
RUN apt install -y build-essential
RUN apt-get -y install manpages-dev
# FROM gcc:4.9
COPY . /ipcApp
WORKDIR /ipcApp/
RUN gcc -o app app.c
CMD ["./app","3","test.txt"]

# docker build -t final .
# docker run -it final