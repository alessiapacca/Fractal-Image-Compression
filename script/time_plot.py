import subprocess
import os
import time
import numpy as np
import pandas as pd
import csv
import matplotlib.pyplot as plt
from PIL import Image


time_base_impl = pd.read_csv("../benchmark_res/time-base-implementation.csv")
time_base_impl_flags = pd.read_csv("../benchmark_res/time-base-imp-all-flags.csv")
time_base_opt = pd.read_csv("../benchmark_res/time-base-optimization.csv")
time_vectorize = pd.read_csv("../benchmark_res/time-vectorized.csv")
time_merge_opt = pd.read_csv("../benchmark_res/time-merge.csv")


plt.title("Time plot for img n.6")
plt.grid(linestyle='-.', linewidth=0.5,axis='y')
plt.xlabel("Image size [pixels]")
plt.ylabel("Time [seconds]")
plt.xticks(time_base_impl['SIZE'].iloc[40:50])

plt.plot(time_base_impl['SIZE'].iloc[40:50], time_base_impl['TIME'].iloc[40:50], 'r', marker='x', label="naive implementation")
plt.plot(time_base_impl_flags['SIZE'].iloc[40:50], time_base_impl_flags['TIME'].iloc[40:50], 'm', marker='.', label="naive implementation + all flags")
plt.plot(time_base_opt['SIZE'].iloc[40:50], time_base_opt['TIME'].iloc[40:50], 'g', marker='d', label="basic optimizations + all flags")
plt.plot(time_vectorize['SIZE'].iloc[40:50], time_vectorize['TIME'].iloc[40:50], 'b', marker='^', label="vectorize + all flags")
plt.plot(time_merge_opt['SIZE'].iloc[40:50], time_merge_opt['TIME'].iloc[40:50], 'y', marker='*', label="vectorize + cache + all flags")
plt.legend(loc='best')
plt.show()


