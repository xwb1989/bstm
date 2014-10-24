#!/usr/bin/python

import os
import subprocess

CLEAN = "make clean"
MAKE_TL2 = "make tl2"
MAKE_BOOST = "make boost"

CMD = "./work_load "
HIGH_CONTENTION_DISJOINT = "-c4 -n8 -r128 -p25 -t65536"
HIGH_CONTENTION_OVERLAPPED = "-c4 -n8 -r128 -p50 -t65536"




if __name__ == "__main__":
    subprocess.call(CLEAN, shell=True)

    print("Compiling TL2")
    subprocess.call(MAKE_TL2, shell=True)

    subprocess.call(CMD + HIGH_CONTENTION_OVERLAPPED, shell=True)
    subprocess.call(CMD + HIGH_CONTENTION_DISJOINT, shell=True)
    

    subprocess.call(CLEAN, shell=True)
    subprocess.call(MAKE_BOOST, shell=True)

    subprocess.call(CMD + HIGH_CONTENTION_OVERLAPPED, shell=True)
    subprocess.call(CMD + HIGH_CONTENTION_DISJOINT, shell=True)
