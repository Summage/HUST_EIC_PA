import numpy as np
from matplotlib import pyplot as plt

def average_cost(t):
    return 9000/t+640+90*(t+1)+np.einsum('i,i->', np.arange(1,t+1), np.arange(1,t+1))*180/t

years = np.arange(1, 15)
plt.plot(years, [average_cost(t) for t in years])
plt.show()