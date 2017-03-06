#!/usr/bin/python3

import os
import sys
from subprocess import call
#stdout pipe of a crest run

transfer_file_name = sys.argv[1]
src_original_location = sys.argv[2]

src_filename = os.path.basename(src_original_location)

dirname = ""
with open(transfer_file_name, 'r') as f:
    dirname = f.readline().strip() + ".d"
    
    
crestOutput = "output.out"
#funcallsFileTemplate = "iter_%d_%s"

modeCNativeRunScript="/home/symext/testSymExec/scripts/CaR/modeCNativeCompileAndRun.sh"

iter_cnt = 0
os.chdir(dirname)

with open(crestOutput, 'r') as f:
    for line in f.readlines() :
        if "checksum" in line:
            iter_cnt += 1
            var_assignments_name = "input."  + str(iter_cnt)
            os.environ["VARS_FILENAME"] = var_assignments_name
            call([modeCNativeRunScript, src_filename])
#        print(line[:-1])
            

os.chdir("..")
#call(["rm","-r", dirname])
            
    
