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

flop_cycle_base_impl = (flops_base_impl['TOT_FLOPS'] / res_base_impl['TOT_CYCLES']).mean() 
speedup_base_opt = (flops_base_opt['TOT_FLOPS'] / res_base_opt['TOT_CYCLES']).mean() / flop_cycle_base_impl
speedup_simd = (flops_base_impl['TOT_FLOPS'] / res_simd['TOT_CYCLES']).mean() / flop_cycle_base_impl
speedup_vectorize = (flops_vectorize['TOT_FLOPS'] / res_v_without_flags['TOT_CYCLES']).mean() / flop_cycle_base_impl


runtime_base_opt = (res_base_impl['TOT_CYCLES'] / res_base_opt['TOT_CYCLES']).mean() 
runtime_simd = (res_base_impl['TOT_CYCLES'] / res_simd['TOT_CYCLES']).mean()
runtime_vectorize = (res_base_impl['TOT_CYCLES'] / res_v_without_flags['TOT_CYCLES']).mean()
print('speedup performance base optimization: ' + str(speedup_base_opt))
print('speedup perfomance simd: ' + str(speedup_simd))
print('speedup performance vectorize: ' + str(speedup_vectorize))

print('speedup runtime base optimization: ' + str(runtime_base_opt))
print('speedup runtime simd: ' + str(runtime_simd))
print('speedup runtime vectorize: ' + str(runtime_vectorize))