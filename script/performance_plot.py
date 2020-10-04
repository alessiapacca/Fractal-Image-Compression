import subprocess
import os
import time
import numpy as np
import pandas as pd
import csv
import matplotlib.pyplot as plt
from PIL import Image
from matplotlib.font_manager import FontProperties

flops_base_impl = pd.read_csv("../benchmark_res/op-count-base-implementation.csv")
flops_base_opt = pd.read_csv("../benchmark_res/op-count-base-optimization.csv")
flops_vectorize = pd.read_csv("../benchmark_res/op-count-vector.csv")
flops_merge_opt = pd.read_csv("../benchmark_res/op-count-merge.csv")

res_base_impl_flags = pd.read_csv("../benchmark_res/cycle-count-base-imp-all-flags.csv")
res_base_impl = pd.read_csv("../benchmark_res/cycle-count-base-implementation.csv")
res_base_opt = pd.read_csv("../benchmark_res/cycle-count-base-optimization.csv")
res_simd = pd.read_csv("../benchmark_res/cycle-count-vector-balenga.csv")
res_vectorize = pd.read_csv("../benchmark_res/cycle-count-vector-all-flags.csv")
res_merge_opt = pd.read_csv("../benchmark_res/cycle-count-merge.csv")

res_v_without_flags = pd.read_csv("../benchmark_res/cycle-count-vectorized.csv")
'''res_after_base_impl = pd.read_csv("../benchmark_res/cycle-count-5(11-15)-base-implementation.csv")
res_after_base_opt = pd.read_csv("../benchmark_res/cycle-count-5(11-15)-base-optimization.csv")
res_after_vector = pd.read_csv("../benchmark_res/cycle-count-5(11-15)-vector.csv")
'''
sizes = [256,512,768,1024,1280,1536,1792,2048,2304,2560,2816,3072,3328,3584,3840]
#sizes = [256,512,768,1024,1280,1536,1792,2048,2304,2560]

flops_cycles_base_impl_flags = []
flops_cycles_base_impl = []
flops_cycles_base_opt = []
flops_cycles_vectorize = []
flops_cycles_vectorize_no_f = []
flops_cycles_merge = []
flops_cycles_simd = []

#BEST img from 60->70, WORST img from 50->60
for i in range(40, 55):    
    flops_cycles_base_impl.append(flops_base_impl['TOT_FLOPS'].iloc[i] / res_base_impl['TOT_CYCLES'].iloc[i])
    flops_cycles_base_impl_flags.append(flops_base_impl['TOT_FLOPS'].iloc[i] / res_base_impl_flags['TOT_CYCLES'].iloc[i])
    flops_cycles_base_opt.append(flops_base_opt['TOT_FLOPS'].iloc[i] / res_base_opt['TOT_CYCLES'].iloc[i])
    flops_cycles_vectorize.append(flops_vectorize['TOT_FLOPS'].iloc[i] / res_vectorize['TOT_CYCLES'].iloc[i])
    flops_cycles_vectorize_no_f.append(flops_vectorize['TOT_FLOPS'].iloc[i] / res_v_without_flags['TOT_CYCLES'].iloc[i])
    flops_cycles_merge.append(flops_merge_opt['TOT_FLOPS'].iloc[i] / res_merge_opt['TOT_CYCLES'].iloc[i])
    flops_cycles_simd.append(flops_base_impl['TOT_FLOPS'].iloc[i] / res_simd['TOT_CYCLES'].iloc[i])

'''for i in range(len(res_after_vector['TOT_CYCLES'])):
    flops_cycles_base_impl_flags.append(flops_base_impl['TOT_FLOPS'].iloc[i] / res_after_base_impl['TOT_CYCLES'].iloc[i])
    flops_cycles_base_opt.append(flops_base_impl['TOT_FLOPS'].iloc[i] / res_after_base_opt['TOT_CYCLES'].iloc[i])
    flops_cycles_vectorize.append(flops_base_impl['TOT_FLOPS'].iloc[i] / res_after_vector['TOT_CYCLES'].iloc[i])
'''
plt.title("Performance plot for test img n.5", y=1.065,x=0.28, fontweight='bold')
plt.grid(linestyle='-.', linewidth=0.5,axis='y')
plt.xlabel("Image size [pixels]", fontsize=11)
plt.ylabel("Performance [ops/cycles]", rotation=0, fontsize=11)
ax = plt.gca()
ax.yaxis.set_label_coords(0.16,1.02)
plt.ylim([2, 14])

plt.axvline(512, 0,linestyle='--', color='gray')
ax.text(550, 4, 'L1', fontsize=9)
plt.axvline(1024, 0,linestyle='--', color='gray')
ax.text(1070, 4, 'L2', fontsize=9)
plt.axvline(2896, 0,linestyle='--', color='gray')
ax.text(2930, 4, 'L3', fontsize=9)
plt.xticks([256,768,1280,1792,2304,2816,3328,3840])
#plt.yscale('log',basey=2)
f = [3.0, 4.0, 5.0, 6.0,  7.0,  8.0,  9.0, 10.0, 11.0, 12.0, 13.0]

plt.plot(sizes, flops_cycles_base_impl, 'green', marker='x', label="Naive implementation")
plt.plot(sizes, flops_cycles_base_impl_flags, 'yellowgreen', marker='.', label="Naive implementation + additional flags")
#plt.plot(sizes, flops_cycles_base_opt, 'orchid', marker='d', label="Base optimization")
#plt.plot(sizes, flops_cycles_simd, 'royalblue', marker='^', label="SIMD")
plt.plot(sizes, flops_cycles_vectorize_no_f, 'darkorange', marker='+', label="SIMD + cache")
plt.plot(sizes, flops_cycles_vectorize, 'red', marker='^', label="SIMD + cache + additional flags")
#plt.plot(sizes, flops_cycles_merge, 'y', marker='*', label="Merge of optimizations")

fontP = FontProperties()
fontP.set_size('small')
plt.legend(loc='best', prop=fontP)
plt.show()