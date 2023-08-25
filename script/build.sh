#!/bin/bash -ex

chown -R builder2 /app/
cd /app/ && gear-rpm -ba
