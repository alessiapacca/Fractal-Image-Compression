import subprocess
import os
import time
import numpy as np
import pandas as pd
import csv
import matplotlib.pyplot as plt
from PIL import Image

time_naive = pd.read_csv("../benchmark_res/cycle-count-base-implementation1-10.csv")
time_merge_opt = pd.read_csv("../benchmark_res/cycle-count-vectorized1-10.csv")

plt.title("Runtime plot for all images",y=1.09,x=0.204, fontweight='bold')
plt.grid(linestyle='-.', linewidth=0.5,axis='y')

plt.xlabel("Image size [pixels]", fontsize=11)
plt.ylabel("Runtime [cycles]", fontsize=11, rotation=0)

ax = plt.gca()
ax.yaxis.set_label_coords(0.075,1.045)
plt.ylim(-0.1*1e12, 3.0*1e12)
plt.yticks([0.0*1e12,0.5*1e12,1.0*1e12, 1.5*1e12, 2.0*1e12, 2.5*1e12, 3.0*1e12])
plt.xticks([256,512,768,1024,1280,1536,1792,2048,2304,2560])
#plt.plot(sizes, flops_cycles_baseline, 'r', marker='.', label="naive implementation")

plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_naive['TOT_CYCLES'].iloc[0:10], 'r', marker='x', label="Test img1 - naive implementation", linestyle='--')
plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_naive['TOT_CYCLES'].iloc[10:20], 'g', marker='d', label="Test img2 - naive implementation", linestyle='--')
plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_naive['TOT_CYCLES'].iloc[20:30], 'b', marker='^', label="Test img3 - naive implementation", linestyle='--')
plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_naive['TOT_CYCLES'].iloc[30:40], 'y', marker='*', label="Test img4 - naive implementation", linestyle='--')
plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_naive['TOT_CYCLES'].iloc[40:50], 'c', marker='v', label="Test img5 - naive implementation", linestyle='--')
plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_naive['TOT_CYCLES'].iloc[50:60], 'm', marker='h', label="Test img6 - naive implementation", linestyle='--')
plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_naive['TOT_CYCLES'].iloc[60:70], 'k', marker='+', label="Test img7 - naive implementation", linestyle='--')

plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_merge_opt['TOT_CYCLES'].iloc[0:10], 'r', marker='x', label="Test img1 - SIMD + cache")
plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_merge_opt['TOT_CYCLES'].iloc[10:20], 'g', marker='d', label="Test img2 - SIMD + cache")
plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_merge_opt['TOT_CYCLES'].iloc[20:30], 'b', marker='^', label="Test img3 - SIMD + cache")
plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_merge_opt['TOT_CYCLES'].iloc[30:40], 'y', marker='*', label="Test img4 - SIMD + cache")
plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_merge_opt['TOT_CYCLES'].iloc[40:50], 'c', marker='v', label="Test img5 - SIMD + cache")
plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_merge_opt['TOT_CYCLES'].iloc[50:60], 'm', marker='h', label="Test img6 - SIMD + cache")
plt.plot([256,512,768,1024,1280,1536,1792,2048,2304,2560], time_merge_opt['TOT_CYCLES'].iloc[60:70], 'k', marker='+', label="Test img7 - SIMD + cache")


plt.legend(loc='best')
plt.show()


