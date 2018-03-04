FROM ubuntu:14.04

RUN apt-get update && apt-get install -y \
  vim \
  postgresql

EXPOSE 5432

ENV PWN3=/opt/pwn3

RUN useradd -ms /bin/bash pwn3

ADD --chown=pwn3 server/MasterServer/initdb.sql $PWN3/initdb.sql
ADD --chown=pwn3 setup $PWN3/setup

