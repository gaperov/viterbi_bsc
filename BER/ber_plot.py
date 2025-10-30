import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# считываем дату из файла
data = pd.read_csv("results/ber.csv")

p = data["Probability"]
ber = data["ber"]

#линейная регрессия для тренда гкрафика
b, a = np.polyfit(p, ber, 1)
x_line = np.linspace(p.min(), p.max(), len(p))
y_line = a + b * x_line

plt.figure(figsize=(8,5), dpi=150)
plt.plot(p, ber, linestyle='', marker='o', label="BER(p)")
plt.plot(x_line, y_line, linestyle='--', label=f"trend: {a:.3f} + {b:.3f} * p")
plt.title("BSC chanel Bit Error probability(p)")
plt.xlabel("Probability of error in BSC chanel")
plt.ylabel("Number of errors in bits/number of all bits (BER)")
plt.grid(True)
plt.legend()
plt.tight_layout()

plt.savefig("results/ber_plot.png")
#plt.show()