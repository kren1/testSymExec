#!/usr/bin/python
import sys

checksum = False
limiter = sys.argv[1]
for line in sys.stdin:
    if checksum and limiter in line:
        exit(0)
    if limiter in line:
        checksum = True
    sys.stdout.write(line)
