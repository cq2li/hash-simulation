import seaborn as sns 
from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt

dir = Path(".")
data = list(dir.glob("*.txt"))

agg = []

for dat in data:
    df = pd.read_csv(dat.name, delim_whitespace=True, skiprows=2)
    desc = dat.open().readline().strip()
    df['description'] = desc
    df['filename'] = dat.name
    agg.append(df)

agg = pd.concat(agg)
print(agg)

sim = sns.lineplot(
        agg[agg.freeFraction < 0.3],
        x = "freeFraction",
        y = "unsuccessfull",
        hue = "description",
        )
sim.set(title = "Reconcile with paper",
        xlabel = "Free slots (not occupied nor tombstone)")
plt.xlim(reversed(plt.xlim()))
plt.show()

sim = sns.lineplot(
        agg[agg.freeFraction < 0.3],
        x = "freeFraction",
        y = "successfull",
        hue = "description",
        )
sim.set(title = "Reconcile with paper",
        xlabel = "Free slots (not occupied nor tombstone)")
plt.xlim(reversed(plt.xlim()))
plt.show()

sim = sns.lineplot(
        agg[agg.freeFraction < 0.3],
        x = "K",
        y = "unsuccessfull",
        hue = "description")

plt.show()

