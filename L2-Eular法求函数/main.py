import numpy as np
import matplotlib.pyplot as plt


def Eular_1(op, x, y, z, h, t):
    log = [[x], [y]]
    for i in range(t):
        x, y = x + h * op[0](x, y, z), y + h * op[1](x, y, z)
        log[0].append(x)
        log[1].append(y)
        z += h
    log[0].append(op[2](z))
    log[1].append(op[3](z))
    return log


operation_list = [[lambda x, y, t: 2 * x + 3 * y, lambda x, y, t: 3 * x + 2 * y,
                   lambda t: 0.5*np.exp(-t)+0.5*np.exp(5*t),
                   lambda t: -0.5*np.exp(-t)+0.5*np.exp(5*t)
                   ],
                  [lambda x, y, t: x + 5 * y, lambda x, y, t: -x - 3 * y,
                   lambda t: 5 * np.exp(-t) * (np.cos(t) + 6 * np.sin(t)),
                   lambda t: np.exp(-t) * (4*np.cos(t) - 13 * np.sin(t))
                   ],
                  [lambda x, y, t: x + 3 * y, lambda x, y, t: x - y + 2 * np.exp(t),
                   lambda t: -np.exp(-2*t)+3*np.exp(2*t)-2*np.exp(t),
                   lambda t: np.exp(-2*t)+np.exp(2*t)
                   ]]

args_list = [[operation_list[0], 1, 0, 0, 0.25, 3],
             [operation_list[0], 1, 0, 0, 0.125, 6],
             [operation_list[1], 5, 4, 0, 0.25, 3],
             [operation_list[1], 5, 4, 0, 0.125, 6],
             [operation_list[0], 0, 2, 0, 0.25, 3],
             [operation_list[0], 0, 2, 0, 0.125, 6]
             ]

log_list = [Eular_1(*args) for args in args_list]
print("Problem 1\n")
for log in log_list:
    print("Eular:\n")
    for i in range(len(log[0])-5, len(log[0])-2):
        print("x: ", log[0][i], "\ty: ", log[1][i], "\n")
    print("Precision Res:\nx: ", log[0][-1], "\ty: ", log[1][-1], "\n")

def Eular_2(x, y, h=0.1, t=7):
    op = [lambda x, y: x*(10-x-y), lambda x, y: y*(15-x-3*y)]
    log = [[x], [y]]
    for i in range(t):
        x, y = x+h*op[0](x,y), y+h*op[1](x,y)
        log[0].append(x)
        log[1].append(y)
    return log

log = Eular_2(5, 2, 0.01, 1000)
t = np.arange(len(log[0]))
plt.plot(t, log[0], color="blue", linewidth=2.5, linestyle="-.", label="(t, B(t))")
plt.plot(t, log[1], color="red", linewidth=2.5, linestyle="-.", label="(t, T(t))")
plt.legend(loc="upper left")
plt.show()
plt.plot(log[0], log[1], color="green", linewidth=2.5, linestyle="-", label="(B(t),T(t))")
plt.legend(loc="upper left")
plt.show()
