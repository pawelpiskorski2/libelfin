import subprocess
import sys
import datetime
import os
os.chdir(os.path.dirname(os.path.realpath(__file__)))

if sys.version_info < (3, 0):
    sys.stdout.write("Error, must use python Python 3.x\n")
    sys.exit(1)

with open("to_string.cc", "w") as f:
    f.write("// Automatically generated by make at {0}\n".format(str(datetime.datetime.now())))
    f.write("// DO NOT EDIT\n")
    f.write("\n")
    f.write('#include "internal.hh"\n')
    f.write("\n")
    f.write('DWARFPP_BEGIN_NAMESPACE\n')
    f.write("\n")
    v=[sys.executable,os.path.join(os.getcwd(), *'../elf/enum-print.py'.split('/')),'--input', 'dwarf++.hh']
    print(" ".join(v))
    f.write(subprocess.check_output(v).decode("ascii"))
    v=[sys.executable,os.path.join(os.getcwd(), *'../elf/enum-print.py'.split('/')),'-s', '_', '-u', '--hex', '-x' ,'hi_user', '-x', 'lo_user', '--input', 'data.hh']
    print(" ".join(v))
    f.write(subprocess.check_output(v).decode("ascii"))
    f.write('DWARFPP_END_NAMESPACE\n')