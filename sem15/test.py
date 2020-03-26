#!/usr/bin/python
import sys
import os


print("input =", input())

print("argv =", sys.argv)
print("env =", os.environ)

exit(23123)
# os.kill(os.getpid(), 3)
