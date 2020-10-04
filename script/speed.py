import subprocess
import os
import time
import numpy as np
import pandas as pd
import csv
import matplotlib.pyplot as plt
from PIL import Image

flops_base_impl = pd.read_csv("../benchmark_res/op-count-base-implementation.csv")
flops_base_opt = pd.read_csv("../benchmark_res/op-count-base-optimization.csv")
flops_vectorize = pd.read_csv("../benchmark_res/op-count-vector.csv")

res_base_impl = pd.read_csv("../benchmark_res/cycle-count-base-implementation.csv")
res_base_opt = pd.read_csv("../benchmark_res/cycle-count-base-optimization.csv")
res_simd = pd.read_csv("../benchmark_res/cycle-count-vector-balenga.csv")
res_v_without_flags = pd.read_csv("../benchmark_res/cycle-count-vectorized.csv")
res_comp = pd.read_csv("../benchmark_res/cycle-count-compute-bound.csv")

flop_cycle_base_impl = (flops_base_impl['TOT_FLOPS'] / res_base_impl['TOT_CYCLES'])
speedup_base_opt = (flops_base_opt['TOT_FLOPS'] / res_base_opt['TOT_CYCLES']) / flop_cycle_base_impl
speedup_simd = (flops_base_impl['TOT_FLOPS'] / res_simd['TOT_CYCLES']) / flop_cycle_base_impl
speedup_vectorize = (flops_vectorize['TOT_FLOPS'][40:55] / res_v_without_flags['TOT_CYCLES'][40:55]) / flop_cycle_base_impl[40:55]

print(res_comp['TOT_CYCLES'].mean() /  res_v_without_flags['TOT_CYCLES'][40:50].mean())



runtime_base_opt = [0]*15
runtime_simd = [0]*15
runtime_vectorize = [0]*15
j = 0
for i in range(40, 55):
    runtime_base_opt[j] += res_base_impl['TOT_CYCLES'].iloc[i] / res_base_opt['TOT_CYCLES'].iloc[i]
    runtime_simd[j] += res_base_impl['TOT_CYCLES'].iloc[i] / res_simd['TOT_CYCLES'].iloc[i]
    runtime_vectorize[j] += res_base_impl['TOT_CYCLES'].iloc[i] / res_v_without_flags['TOT_CYCLES'].iloc[i]
    j += 1


#runtime_base_opt = [index / 15 for index in runtime_base_opt]
#runtime_simd = [index / 15 for index in runtime_simd]
#runtime_vectorize = [index / 15 for index in runtime_vectorize]


sizes = [256,512,768,1024,1280,1536,1792,2048,2304,2560,2816,3072,3328,3584,3840]
N = len(sizes)

ind = np.arange(N)    # the x locations for the groups
width = 0.35       # the width of the bars: can also be len(x) sequence

p1 = plt.bar(ind, tuple(runtime_simd), width, color='C0')
p2 = plt.bar(ind, tuple(runtime_base_opt), width,
             bottom=tuple(runtime_simd), color='orchid')
p3 = plt.bar(ind, tuple(runtime_vectorize), width,
             bottom=tuple(runtime_base_opt), color='C1')
plt.ylabel("Speedup [N° times]", fontsize=11, rotation=0)
ax = plt.gca()
ax.yaxis.set_label_coords(0.115,1.045)
plt.grid(linestyle='-.', linewidth=0.5,axis='y')
plt.xlabel('Image size [pixels]', fontsize=11)
plt.title('Runtime speedup plot',y=1.09,x=0.17, fontweight='bold')
plt.xticks(ind, [256,768,1280,1792,2304,2816,3328,3840])
plt.locator_params(axis='x', nbins=8)
plt.yticks(np.arange(0, 15))
plt.legend((p1[0], p2[0], p3[0]), ('SIMD', 'Base optimization', 'SIMD + cache'), loc='best')

plt.show()