import matplotlib.pyplot as plt
import matplotlib.lines as mlines
import numpy as np
import matplotlib.ticker as ticker


ax = plt.gca()


plt.xlim([0, 15])
plt.ylim([0, 80])
plt.plot([0, 4/5.89], [0, 4], color='black')
plt.plot([4/5.89, 66/5.89], [4, 66], color='black')
plt.axhline(4, color='blue', label='Scalar roofline')
plt.axhline(66, color='green', label='Vector roofline')

plt.plot((3, 3), (0, 17.67), color='gray',linestyle='dashed')
plt.plot((4/5.89, 4/5.89), (0, 4), color='blue',linestyle='dashed')
plt.plot((66/5.89, 66/5.89), (0, 66), color='green',linestyle='dashed')
plt.scatter(3, 9.5, color='orange', label="Max performance achieved")
plt.scatter(3, 2.8, color='green', label="Min performance achieved")
plt.title("Roofline plot", fontsize=11, fontweight='bold', y=1.065,x=0.095)



ax.yaxis.set_major_formatter(ticker.FuncFormatter(lambda y, _: '{:g}'.format(y)))
ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: '{:g}'.format(x)))
box = ax.get_position()
ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
ax.legend(loc='best', fontsize=11)
ax.text(7, 66.7, 'π vector = 66', fontsize=10)
ax.text(7, 4.3, 'π scalar = 4', fontsize=10)
ax.text(3.2, 13, 'lower bound I(n)', fontsize=10)
plt.ylim(ymax=100)
plt.grid(axis='y')
plt.xlabel("I(n) [int ops/bytes]", fontsize=11)
plt.ylabel("P(n) [int ops/cycles]", fontsize=11, rotation=0)
ax = plt.gca()
ax.yaxis.set_label_coords(0.15,1.02)
plt.show()