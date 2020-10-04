import subprocess
import os
import time
import numpy as np
import pandas as pd
import csv
import matplotlib.pyplot as plt
from PIL import Image




res_base_impl_flags = pd.read_csv("../benchmark_res/cycle-count-base-imp-all-flags.csv")
res_base_impl = pd.read_csv("../benchmark_res/cycle-count-base-implementation.csv")
res_base_opt = pd.read_csv("../benchmark_res/cycle-count-base-optimization.csv")
res_simd = pd.read_csv("../benchmark_res/cycle-count-vector-balenga.csv")

res_vectorize = pd.read_csv("../benchmark_res/cycle-count-vector-all-flags.csv")
res_v_without_flags = pd.read_csv("../benchmark_res/cycle-count-vectorized.csv")
res_merge_opt = pd.read_csv("../benchmark_res/cycle-count-merge.csv")
res_compute_bound = pd.read_csv("../benchmark_res/cycle-count-compute-bound.csv")


res_after_base_impl = pd.read_csv("../benchmark_res/cycle-count-5(11-15)-base-implementation.csv")
res_after_base_opt = pd.read_csv("../benchmark_res/cycle-count-5(11-15)-base-optimization.csv")
res_after_vector = pd.read_csv("../benchmark_res/cycle-count-5(11-15)-vector.csv")

#sizes = [256,512,768,1024,1280,1536,1792,2048,2304,2560,2816,3072,3328,3584,3840]
sizes = [256,512,768,1024,1280,1536,1792,2048,2304,2560]
flops_cycles_base_impl_flags = []
flops_cycles_base_impl = []
flops_cycles_base_opt = []
flops_cycles_vectorize = []
flops_cycles_vectorize_no_flags = []
flops_cycles_merge = []
flops_cycles_simd = []

#BEST img from 60->70, WORST img from 50->60
count = 0
for i in range(40, 50):    
    flops_cycles_base_impl_flags.append(res_base_impl_flags['TOT_CYCLES'].iloc[i])
    flops_cycles_base_impl.append(res_base_impl['TOT_CYCLES'].iloc[i])
    flops_cycles_base_opt.append(res_base_opt['TOT_CYCLES'].iloc[i])
    flops_cycles_vectorize.append(res_vectorize['TOT_CYCLES'].iloc[i])
    flops_cycles_vectorize_no_flags.append(res_v_without_flags['TOT_CYCLES'].iloc[i])
    #flops_cycles_merge.append(res_merge_opt['TOT_CYCLES'].iloc[i])
    flops_cycles_simd.append(res_simd['TOT_CYCLES'].iloc[i])

'''for i in range(len(res_after_vector['TOT_CYCLES'])):
    flops_cycles_base_impl_flags.append(res_after_base_impl['TOT_CYCLES'].iloc[i])
    flops_cycles_base_opt.append(res_after_base_opt['TOT_CYCLES'].iloc[i])
    flops_cycles_vectorize.append(res_after_vector['TOT_CYCLES'].iloc[i])
'''
plt.title("Runtime plot for test img n.5",y=1.09,x=0.22, fontweight='bold')
plt.grid(linestyle='-.', linewidth=0.5,axis='y')
plt.xlabel("Image size [pixels]", fontsize=11)
plt.ylabel("Runtime [cycles]", fontsize=11, rotation=0)

plt.xticks(sizes)
#plt.yscale('log')
ax = plt.gca()
ax.yaxis.set_label_coords(0.075,1.05)

plt.plot(sizes, flops_cycles_base_impl, 'green', marker='x', label="Naive implementation")
plt.plot(sizes, res_compute_bound['TOT_CYCLES'], 'k', marker='h', label="Compute bound ")
#plt.plot(sizes, flops_cycles_base_impl_flags, 'm', marker='.', label="naive implementation + compiler flags")
#plt.plot(sizes, flops_cycles_base_opt, 'orchid', marker='d', label="Base optimization")
#plt.plot(sizes, flops_cycles_simd, 'royalblue', marker='^', label="SIMD")
plt.plot(sizes, flops_cycles_vectorize_no_flags, 'darkorange', marker='+', label="SIMD + cache")
#plt.plot(sizes, flops_cycles_vectorize, 'g', marker='^', label="SIMD + cache + compiler flags")
#plt.plot(sizes, flops_cycles_merge, 'y', marker='*', label="merge of optimizations")
plt.legend(loc='best')
plt.show()


