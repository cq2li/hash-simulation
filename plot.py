import seaborn as sns 
from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt

dir = Path(".")
sns.set_style("whitegrid", {"grid.linestyle": ":"})
plot_data = list(dir.glob("./plot_data/v2/*plot.txt"))
plot_agg = []

for dat in plot_data:
    df = pd.read_csv(dat, delim_whitespace=True, skiprows=2)
    desc = dat.open().readline().strip()
    df['filename'] = dat.name
    plot_agg.append(df)

plot_agg = pd.concat(plot_agg)
plot_agg["tombstones"] = 1 - plot_agg["freeFraction"].add(plot_agg["n"], axis=0)
plot_agg["sqrt_unsuccessful-search"] = plot_agg["unsuccessful-search"]**(-1/2)

# plt.figure(figsize=(13.75, 10))
# sim = sns.lineplot(
#         plot_agg,
#         x = "freeFraction",
#         y = "unsuccessful-search",
#         hue = "k",
#         )
# sim.set(title = "Stochastic Unsuccessful Search Cost",
#         xlabel = "Free slots not occupied nor tombstone)")
# plt.xlim(reversed(plt.xlim()))
# plt.savefig("Unsuc_freeFrac_plot")
# plt.show()
#
# plt.figure(figsize=(13.75, 10))
# sim = sns.lineplot(
#         plot_agg,
#         x = "freeFraction",
#         y = "successful-search",
#         hue = "k",
#         )
# sim.set(title = "Stochastic Successful Search Cost",
#         xlabel = "Free slots (not occupied nor tombstone)")
# plt.xlim(reversed(plt.xlim()))
# plt.savefig("Suc_freeFrac_plot")
# plt.show()

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg.sort_values(by="k"),
        x = "K",
        y = "unsuccessful-search",
        hue = "k",
        legend = "full")
sim.set(title = "Unsuccessful Search Cost Over Time", xlabel = "Number of Deletions")
plt.yscale('log')
plt.savefig("Unsuccessful Search Cost Over Time")

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg,
        x = "K",
        y = "successful-search",
        hue = "k",
        legend = "full")
sim.set(title = "Successful Search Cost Over Time", xlabel = "Number of Deletions")
plt.yscale('log')
plt.savefig("Successful Search Cost Over Time")


plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg,
        x = "K",
        y = "timeavg-insertion",
        hue = "k",
        legend = "full")
sim.set(title = "Time Averaged Insertion Cost", xlabel = "Number of Deletions")
plt.savefig("Time Averaged Inertion Cost")


rows = plot_agg.groupby("k")["K"].transform(max) == plot_agg["K"]

plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg[rows],
        x = "k",
        y = "unsuccessful-search",
        hue = "k",
        legend = "full")
sim.set(title = "Unsuccessful Search Cost On Last Iteration Interval", xlabel = "k - Stochastic Weight")
plt.yscale('log')
plt.savefig("Unsuccssful Search Cost On Last Iteration Interval")

plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg[rows],
        x = "k",
        y = "successful-search",
        hue = "k",
        legend = "full")
sim.set(title = "Successful Search Cost On Last Iteration Interval", xlabel = "k - Stochastic Weight")
plt.savefig("Successful Search Cost On Last Iteration Interval")

plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg[rows],
        x = "k",
        y = "timeavg-insertion",
        legend = "full")
sim.set(title = "Time Average Insertion Cost On Last Iteration Interval", xlabel = "k - Stochastic Weight")
plt.savefig("Time Average Insertion on Last Iteration Interval vs k")

plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg,
        x = "freeFraction",
        y = "unsuccessful-search",
        hue = "k",
        legend = "full")
sim.set(title = "Unsuccessful Search Cost vs Empty Slots", xlabel = "Empty Slots Excluding Tombstone")
plt.yscale('log')
plt.savefig("Unsuccessful Search Cost vs Empty Slots")

plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg,
        x = "freeFraction",
        y = "sqrt_unsuccessful-search",
        hue = "k")
sim.set(title = "Stochastic Unsucc. Search vs Fraction Free (excl. Tmb)",
        xlabel = "FreeFraction")
plt.savefig("Sqrt")

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg,
        x = "K",
        y = "freeFraction",
        hue = "k",
        legend = "full")
sim.set(title = "Empty Slots Over Time", xlabel = "Number of Deletions", ylabel = "Empty Slots")
plt.yscale('log')
plt.savefig("Empty Slots Over Time")

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg,
        x = "K",
        y = "n",
        hue = "k",
        legend = "full")
sim.set(title = "Load (n/m) Over time", xlabel = "Number of Deletions")
plt.savefig("Load Over Time")

# seaborne does this werid thing where it buckets your hue values
plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg[rows],
        x = "k",
        y = "tombstones",
        hue = "k",
        legend = "full")
sim.set(title = "Tombstones On Last Iteration Interval", xlabel = "k - Stochastic Weight")
plt.savefig("Tombstone on Last Iteration Interval vs k")

plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg[rows],
        x = "k",
        y = "timeavg-load",
        legend = "full")
sim.set(title = "Load (n/m) at Last 10% of Iterationtion", xlabel = "k - Stochastic Weight")
plt.savefig("Time Average Load Last 10pct Iterations")


# data = list(dir.glob("*rec.txt"))
#
# agg = []
#
# for dat in data:
#     df = pd.read_csv(dat.name, delim_whitespace=True, skiprows=2)
#     desc = dat.open().readline().strip()
#     df['k'] = desc
#     df['filename'] = dat.name
#     agg.append(df)
#
# agg = pd.concat(agg)
#
# plt.figure(figsize=(13.75, 10))
# sim = sns.lineplot(
#         agg[agg.freeFraction < 0.3],
#         x = "freeFraction",
#         y = "unsuccessful-search",
#         hue = "k",
#         )
# sim.set(title = "Reconcile with paper",
#         xlabel = "Free Fraction")
# plt.xlim(reversed(plt.xlim()))
# plt.savefig("Unsuc_freeFrac_rec")
# # plt.show()
#
# plt.figure(figsize=(13.75, 10))
# sim = sns.lineplot(
#         agg[agg.freeFraction < 0.3],
#         x = "freeFraction",
#         y = "successful-search",
#         hue = "k",
#         )
# sim.set(title = "Reconcile with paper",
#         xlabel = "Free Fraction")
# plt.xlim(reversed(plt.xlim()))
# plt.savefig("Suc_freeFrac_rec")
# # plt.show()
#
# plt.figure(figsize=(13.75, 10))
# sim = sns.lineplot(
#         agg[agg.freeFraction < 0.3],
#         x = "K",
#         y = "unsuccessful-search",
#         hue = "k")
# sim.set(title = "Reconcile with paper",
#         xlabel = "Number of deletions")
# plt.savefig("Unsuc_ndelete_rec")
# # plt.show()
#
# plt.figure(figsize=(13.75, 10))
# sim = sns.lineplot(
#         agg[agg.freeFraction < 0.3],
#         x = "K",
#         y = "successful-search",
#         hue = "k")
# sim.set(title = "Reconcile with paper",
#         xlabel = "Number of deletions")
# plt.savefig("Suc_ndelete_rec")
# # plt.show()
