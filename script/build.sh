#!/bin/bash -ex

chown -R builder2:builder2 /app/
mkdir -p /app/RPMS/

mkdir /home/builder2/hasher 
mkdir /home/builder2/.hasher

cp /app/hasher/config /home/builder2/.hasher/config 
hsh --initroot-only --no-wait-lock -vv /home/builder2/hasher 

cd /app/ && gear --hasher -- hsh-rebuild /home/builder2/hasher
cp /home/builder2/hasher/repo/x86_64/RPMS.hasher/*.rpm /app/RPMS/
