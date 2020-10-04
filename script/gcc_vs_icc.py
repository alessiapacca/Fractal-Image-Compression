import subprocess
import os
import time
import numpy as np
import pandas as pd
import csv
import matplotlib.pyplot as plt
from PIL import Image

res_base_opt = pd.read_csv("../benchmark_res/cycle-count-base-optimization.csv")
res_vectorize = pd.read_csv("../benchmark_res/cycle-count-vectorized.csv")

icc_base_opt = pd.read_csv("../benchmark_res/icc-cycle-count-base-optimization.csv")
icc_vectorize = pd.read_csv("../benchmark_res/icc-cycle-count-vectorized.csv")

sizes = []

flops_cycles_base_opt = []
flops_cycles_vectorize = []

icc_flops_cycles_base_opt = []
icc_flops_cycles_vectorize = []

#BEST img from 60->70, WORST img from 50->60
for i in range(40, 50):
    im = Image.open('../' + res_base_opt['IMG_NAME'].iloc[i])
    width, height = im.size
    sizes.append(width)
    
    flops_cycles_base_opt.append(res_base_opt['TOT_CYCLES'].iloc[i])
    flops_cycles_vectorize.append(res_vectorize['TOT_CYCLES'].iloc[i])
    icc_flops_cycles_base_opt.append(icc_base_opt['TOT_CYCLES'].iloc[i])
    icc_flops_cycles_vectorize.append(icc_vectorize['TOT_CYCLES'].iloc[i])  


plt.title("Runtime plot for test img n.5")
plt.grid(linestyle='-.', linewidth=0.5,axis='y')
plt.xlabel("Image size [pixels]")
plt.ylabel("Runtime [cycles]")
ax = plt.gca()
ax.yaxis.set_label_coords(0,1.02)
plt.xticks(sizes)

plt.plot(sizes, flops_cycles_base_opt, 'g', marker='d', label="gcc - basic optimization")
plt.plot(sizes, flops_cycles_vectorize, 'b', marker='^', label="gcc - SIMD + cache")

plt.plot(sizes, flops_cycles_base_opt, 'g', marker='d', label="icc - basic optimization")
plt.plot(sizes, flops_cycles_vectorize, 'b', marker='^', label="icc - SIMD + cache")


plt.legend(loc='best')
plt.show()