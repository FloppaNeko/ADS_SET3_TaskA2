import matplotlib.pyplot as plt
import numpy as np


LIMITS = [1, 10, 50, 100]
COLORS = ["tab:blue", "tab:green", "tab:orange", "tab:red"]


def get_title(n):
    if n == 1:
        return "PLAIN MERGE SORT"
    else:
        return "MERGE+INSERTION SORT " + str(n)
    

def build_graph(n, arr, title=""):
    for i, (lim, color) in enumerate(zip(LIMITS, COLORS)):
        axes[n].plot(
            x,
            arr[:, i],
            lw=1,
            label=get_title(lim),
            color=color
        )
    axes[n].set_title(title)
    axes[n].set_xlabel("Размер массива")
    axes[n].set_ylabel("Время выполнения $(\mu s)$")
    axes[n].grid()
    axes[n].legend()


data = np.genfromtxt("data.csv", delimiter=',')
x = data[:, 0]
rev_arr = data[:, 1::3]
rand_arr = data[:, 2::3]
as_arr = data[:, 3::3]


fig, axes = plt.subplots(1, 3, figsize=(15, 4))

build_graph(0, rev_arr, "Отсортированные в обратном порядке массивы")
build_graph(1, rand_arr, "Случайные массивы")
build_graph(2, as_arr, "Почти отсортированнные массивы")

# Adjust spacing between plots
plt.tight_layout()

# Show the plots
plt.show()

