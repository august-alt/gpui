#!/bin/bash -ex
rpm -ql gear ; rpm -ql hasher
cd /app/ && /usr/bin/gear-rpm -ba
