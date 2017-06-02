import subprocess
import sys
import datetime
import os
os.chdir(os.path.dirname(os.path.realpath(__file__)))

with open("to_string.cc", "w") as f:
    f.write("// Automatically generated by make at {0}\n".format(str(datetime.datetime.now())))
    f.write("// DO NOT EDIT\n")
    f.write("\n")
    f.write('#include "data.hh"\n')
    f.write('#include "to_hex.hh"\n')
    f.write("\n")
    f.write('ELFPP_BEGIN_NAMESPACE\n')
    f.write("\n")
    v=[sys.executable,'enum-print.py',"-u", "--hex", "--no-type", "--mask", "shf", "--mask", "pf", "-x", "loos", "-x", "hios", "-x", "loproc", "-x", "hiproc", "--input", "data.hh"]
    print(" ".join(v))
    f.write(subprocess.check_output(v).decode("ascii"))
    f.write('ELFPP_END_NAMESPACE\n')