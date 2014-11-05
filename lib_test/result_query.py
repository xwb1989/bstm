#!/usr/bin/python

#File_name: result_query.py
#Author: Wenbin Xiao

import sys
import os
import re

class DataSet:
    def __init__(self, entries):
        self.entries = entries
    def size(self):
        return len(self.entries)
    def search(self, key, val):
        return DataSet([e for e in self.entries if e.properties[key] == val ])
    def average(self, key):
        if key == "version":
            print "Error: please input a numeric key"
        else:
            res = [ e.properties[key] for e in self.entries ]
            if len(res )!= 0:
                return sum(res) / float(len(res))
            else:
                return 0
            
    def __str__(self):
        s = ""
        for e in self.entries:
            s += e.__str__()
        return s
class DataEntry:
    #Constructor
    def __init__(self, version, properties):
        self.properties = { "version" : version}
        self.properties.update(properties)
        self.properties["abort_ratio"] = properties["Aborts"] / float(properties["Starts"])

    @staticmethod
    def FileToEntries(file_path):
        """
            This method is based on the format of entry:
                --------Following is the entry-------
            Configurations: 
                Transactions        = 65536
                Clients             = 1
                Transactions/client = 65536
                Sets/transaction    = 1
                Relations           = 8
                Query percent       = 25
                Query range         = 2
            Query Range:
                Client 0   = 1 - 2
            TL2 system ready: GV=GV4
            Running clients... done.
            Time = 0.310766
            Deallocating memory... done.
            TL2 system shutdown:
              GCLOCK=0x0 Starts=65536 Aborts=0
              Overflows: R=0 W=0 L=0
              -------------Entry finishes------------
        """
        res = []
        version = "boost"
        if "boost" in file_path:
            version = "boost"
        else:
            version = "tl2"
        try:
            f = open(file_path)
            properties = {}
            size = 13
            for line in f:
                #skip useless lines
                if "Configuration" in line and len(properties) != 0 and len(properties) != size:
                    print "wrong data:"
                    print properties, version
                    print line
                if "Configuration" in line and len(properties) == size:
                    res.append(DataEntry(version, properties))
                    properties = {}
                elif "-" in line:
                    continue
                elif "=" in line:
                    values = multiple_equations_to_key_value(line)
                    properties.update(values)
        except IOError:
            sys.stderr.write("Failed to open file: %s" % (file_path))
        return res
    def __str__(self):
        s = "DataEntry:\n"
        for key, val in self.properties.iteritems():
            s += ("\t%s = %s\n" %(key, val))
        return s

def multiple_equations_to_key_value(line):
    pattern = re.compile("\w+\s*=\s*\d*.?\d*")
    l = re.findall(pattern, line)
    res = {}
    for e in l:
        ret = single_equation_to_key_value(e)
        if ret != None:
            res.update(ret)
    return res


    
def single_equation_to_key_value(line):
    splited = [word.strip() for word in line.split('=')]
    ret = None
    try: 
        if ("Time" in splited[0]):
            ret = { splited[0] : float(splited[1]) }
        else:
            ret = { splited[0] : int(float(splited[1])) }
        return  ret
    except ValueError:
        #print "unable to convert %s : %s" %(splited[0], splited[1])
        return None
def printf(line):
    sys.stdout.write(line.__str__())
def get_file_list(target_dir):
    return [ os.path.join(target_dir, file) for file in os.listdir(target_dir) if os.path.isfile(os.path.join(target_dir, file))] 
    


if __name__ == "__main__":
    #argument check
    if len(sys.argv) != 2:
        sys.stderr.write("Invalid argument. Usage: result_query.py target_dir")
        sys.exit()
    
    target_dir = sys.argv[1]
    print "Target Directory: %s" %(target_dir)

    #file list
    entries = []
    for file in get_file_list(target_dir):
        entries += (DataEntry.FileToEntries(file))
    data_set= DataSet(entries)
    default = data_set.search("version", "tl2")
    boost = data_set.search("version", "boost")
    
    i = 1
    while i <= 16:
        print "%d clients: " %(i)
        print "\tdefault ninstances: %i" %(default.size())
        print "\tdefault average time: %.2f" %(default.search("Clients", i)).average("Time")
        print "\tdefault average abort ratio: %.2f" %(default.search("Clients", i)).average("abort_ratio")
        print "\tboost ninstances: %i" %(boost.size())
        print "\tboost average time: %.2f" %(boost.search("Clients", i)).average("Time")
        print "\tboost average abort ratio: %.2f" %(boost.search("Clients", i)).average("abort_ratio")
        print ""
        i *= 2
