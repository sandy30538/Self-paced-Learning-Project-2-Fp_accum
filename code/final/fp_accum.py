
# coding: utf-8

# In[ ]:


from __future__ import print_function
from random import random
from random import seed

import sys

sys.path.append('/home/xilinx')
from pynq import Overlay

if __name__ == "__main__":
    seed(1)
    print("Entry:", sys.argv[0])
    print("System argument(s):", len(sys.argv))

    print("Start of \"" + sys.argv[0] + "\"")

    ol = Overlay("/home/xilinx/IPBitFile/FP_Accum.bit")
    regIP = ol.hls_fp_accumulator_0

    window = []

    # Assign Value
    for i in range(128):
        floating_point_value = random()
        window.append(floating_point_value)
        print("value " + str(i) + " is " + str(floating_point_value))
    
    for i in range(128):
        regIP.write(0x10 + i * 4, window[i])

    print("============================")
    regIP.write(0x00, 0x01)
       
    while (regIP.read(0x00) & 0x4) == 0x0:
        Res = regIP.read(0x20)
        continue
    Res = regIP.read(0x20)    
    
    print("The final answer is " + str(Res))
    print("============================")
    print("Exit process")

