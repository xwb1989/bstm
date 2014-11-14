#!/usr/bin/python

import os
import sys
import subprocess
import datetime
import logging
import threading

CLEAN = "make clean"
MAKE_TL2 = "make tl2"
MAKE_BOOST = "make boost"

CMD = "./work_load "

max_clients = 8
min_clients = 1

max_sets = 8
min_sets = 1

max_size = 128
min_size = 16
size_step = 4

max_percent = 100
min_percent = 25
percent_step = 3

num_tx = 65536 * 2

reps = 5

TIMEOUT = 30 * 60
MAX_RETRY = 10
class Command:
    def __init__(self, cmd):
        self.cmd = cmd
        self.process = None

    def run(self, timeout):
        def target():
            self.process = subprocess.Popen(self.cmd, shell=True)
            self.process.communicate()

        retry = 0
        while True:
            thread = threading.Thread(target=target) 
            thread.start()
            thread.join(timeout)
            if thread.isAlive():
                logging.info("Timeout, kill cmd: " + self.cmd) 
                self.process.terminate()
                subprocess.call("killall -9 work_load", shell=True) #kill the process
                thread.join()
                retry += 1
                if retry >= MAX_RETRY:
                    sys.exit(1)
            else:
                break
        

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
                        cmd_proc = Command(cmd)
                        cmd_proc.run(TIMEOUT)
                    curr_percent += percent_step
                curr_size += size_step
            curr_sets *= 2
        curr_client *= 2



if __name__ == "__main__":

    if not os.path.isdir("output"):
        os.makedirs("output")

    #setup datetime
    today = datetime.datetime.today().strftime("%Y%m%d%H%M%S")
    #setup logging
    logging.basicConfig(filename="logs/" + today, level=logging.INFO, format="%(asctime)s %(message)s", datefmt='%m/%d/%Y %I:%M:%S %p')
   
    if not os.path.isdir("output/"+ today):
        os.makedirs("output/" + today)
    #option
    opt = sys.argv[1]
    if len(sys.argv) > 3:
        TIMEOUT = float(sys.argv[2])
        MAX_RETRY = float(sys.argv[3])

    if opt == "all" or opt == "boost":
        print("Compiling boost")
        subprocess.call(CLEAN, shell=True)
        subprocess.call(MAKE_BOOST, shell=True)
        output_file = "output/" + today + "/normal-boost"
        run_cmd(output_file)

    if opt == "all" or opt == "tl2":
        print("Compiling TL2")
        subprocess.call(CLEAN, shell=True)
        subprocess.call(MAKE_TL2, shell=True)
        output_file = "output/" + today + "/normal-tl2"
        run_cmd(output_file)
