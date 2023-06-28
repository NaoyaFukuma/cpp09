FROM debian:bullseye-slim

RUN apt-get update && \
    apt-get install -y g++ make && \
    apt-get clean

WORKDIR /cpp09_project
ENTRYPOINT [ "make" ]

