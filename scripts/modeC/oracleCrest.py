#!/usr/bin/python3

import os
from subprocess import call
#stdout pipe of a crest run
crestOutput = "output.out"
funcallsFileTemplate = "iter_%d_%s"

modeCNativeRunScript="/home/tim/projects/testSymExec/scripts/CaR/modeCNativeCompileAndRun.sh"

iter_cnt = 0


with open(crestOutput, 'r') as f:
    for line in f.readlines() :
        if "checksum" in line:
            iter_cnt += 1
            var_assignments_name = "input"
            if iter_cnt > 1:
                var_assignments_name += "." + str(iter_cnt)
            os.environ["VARS_FILENAME"] = var_assignments_name
            call([modeCNativeRunScript, "test1.c"])
        print(line)
            
            
    
