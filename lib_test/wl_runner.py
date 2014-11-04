#!/usr/bin/python

import os
import subprocess
import datetime
import logging

CLEAN = "make clean"
MAKE_TL2 = "make tl2"
MAKE_BOOST = "make boost"

CMD = "./work_load "

max_clients = 8
min_clients = 1

max_sets = 8
min_sets = 1

max_size = 64
min_size = 8

max_percent = 100
min_percent = 25
percent_step = 25

num_tx = 4194304 

reps = 5

def run_cmd(output):
    print("Creating file: " + output_file) 
    subprocess.call("touch " + output_file, shell=True)
    curr_client = min_clients
    while curr_client <= max_clients:
        curr_sets = min_sets
        while curr_sets <= max_sets:
            curr_size = min_size
            while curr_size <= max_size:
                curr_percent = min_percent
                while curr_percent <= max_percent:
                    args = " -c%i -n%i -r%i -p%i -t%i" %(curr_client, curr_sets, curr_size, curr_percent, num_tx)
                    cmd = CMD + args + " >> " + output
                    logging.info("Runing with cmd: " + cmd) 
                    for i in range(reps):
                        subprocess.call(cmd, shell=True)
                    curr_percent += percent_step
                curr_size *= 2
            curr_sets *= 2
        curr_client *= 2



if __name__ == "__main__":

    if not os.path.isdir("output"):
        os.makedirs("output")

    #setup datetime
    today = datetime.datetime.today().strftime("%Y%m%d%H%M%S")
    #setup logging
    logging.basicConfig(filename="logs/" + today, level=logging.INFO, format="%(asctime)s %(message)s", datefmt='%m/%d/%Y %I:%M:%S %p')

    print("Compiling TL2")
    subprocess.call(CLEAN, shell=True)
    subprocess.call(MAKE_TL2, shell=True)
    output_file = "output/" + today + "-tl2"
    run_cmd(output_file)

    
    print("Compiling boost")
    subprocess.call(CLEAN, shell=True)
    subprocess.call(MAKE_BOOST, shell=True)
    output_file = "output/" + today + "-boost"
    run_cmd(output_file)
