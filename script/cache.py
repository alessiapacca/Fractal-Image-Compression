import subprocess
import os
import time
import numpy as np
import pandas as pd
import csv
import matplotlib.pyplot as plt
from PIL import Image
from matplotlib.font_manager import FontProperties

cache_miss_simd = pd.read_csv("../benchmark_res/cache-simd-perf.csv")
cache_miss_vector = pd.read_csv("../benchmark_res/cache-vector-perf.csv")

#plt.ylim([2, 15])
#plt.xticks(cache_miss_simd['SIZE'])
#f = [3.0, 4.0, 5.0, 6.0,  7.0,  8.0,  9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0]
#plt.yticks(f)

data = [[cache_miss_simd['MISS'], cache_miss_vector['MISS']]]

ax = plt.gca()
ax.set_yscale('log', basey=10)
ax.yaxis.set_label_coords(0.045,1.02)

labels = cache_miss_simd['SIZE']

x = np.arange(len(labels))  # the label locations
width = 0.35  # the width of the bars

rects1 = plt.bar(x - width/2, cache_miss_simd['MISS'], width, label='SIMD')
rects2 = plt.bar(x + width/2, cache_miss_vector['MISS'], width, label='SIMD + cache')

# Add some text for labels, title and custom x-axis tick labels, etc.
plt.title("Cache miss plot for test img n.5",y=1.065,x=0.24, fontweight='bold')
plt.grid(linestyle='-.', linewidth=0.5,axis='y')
ax.set_ylabel('Cache misses', rotation=0, fontsize=11)
ax.set_xlabel('Image size [pixels]', fontsize=11)
plt.xticks(x, [256,768,1280,1792,2304,2816,3328,3840])
plt.locator_params(axis='x', nbins=8)
ax.legend()

plt.show()