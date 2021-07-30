#!/bin/bash -ex

mkdir /home/builder2/hasher 
mkdir /home/builder2/.hasher
cp /app/hasher/config /home/builder2/.hasher/config 
hsh --initroot-only --no-wait-lock -vv /home/builder2/hasher 

cd /app/ && gear-hsh
