import seaborn as sns 
from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt

dir = Path(".")
sns.set_style("whitegrid", {"grid.linestyle": ":"})
plot_data = list(dir.glob("./plot_data/v3/*plot.txt"))
plot_agg = []

for dat in plot_data:
    df = pd.read_csv(dat, delim_whitespace=True, skiprows=2)
    desc = dat.open().readline().strip()
    df['filename'] = dat.name
    plot_agg.append(df)

plot_agg = pd.concat(plot_agg)
plot_agg["tombstones"] = 1 - plot_agg["freeFraction"].add(plot_agg["n"], axis=0)
plot_agg["sqrt_unsuccessful-search"] = plot_agg["unsuccessful-search"]**(-1/2)

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg,
        x = "K",
        y = "timeavg-insertion",
        hue = "k",
        legend = "full")
sim.set(title = "Time Averaged Insertion Cost", xlabel = "Number of Deletions")
plt.savefig("Time Averaged Insertion Cost")

rows = plot_agg.groupby("k")["K"].transform(max) == plot_agg["K"]

plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg[rows],
        x = "k",
        y = "unsuccessful-search",
        )
sim.set(title = "Unsuccessful Search Cost On Last Iteration Interval", xlabel = "k - Stochastic Weight")
plt.savefig("Unsuccessful Search Cost On Last Iteration Interval")

plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg[rows],
        x = "k",
        y = "successful-search",
        )
sim.set(title = "Successful Search Cost On Last Iteration Interval", xlabel = "k - Stochastic Weight")
plt.savefig("Successful Search Cost On Last Iteration Interval")

plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg[rows],
        x = "k",
        y = "timeavg-insertion")
sim.set(title = "Time Average Insertion Cost On Last Iteration Interval", xlabel = "k - Stochastic Weight")
plt.savefig("Time Average Insertion on Last Iteration Interval vs k")

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg,
        x = "K",
        y = "n",
        hue = "k",
        legend = "brief")
sim.set(title = "Load (n/m) Over time", xlabel = "Number of Deletions")
plt.savefig("Load Over Time")

plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg[rows],
        x = "k",
        y = "timeavg-load")
sim.set(title = "Load (n/m) at Last 10% of Iterationtion", xlabel = "k - Stochastic Weight")
plt.savefig("Time Average Load Last 10pct Iterations")
