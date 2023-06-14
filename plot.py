import seaborn as sns 
from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt

dir = Path(".")
data = list(dir.glob("*rec.txt"))

agg = []

for dat in data:
    df = pd.read_csv(dat.name, delim_whitespace=True, skiprows=2)
    desc = dat.open().readline().strip()
    df['description'] = desc
    df['filename'] = dat.name
    agg.append(df)

agg = pd.concat(agg)

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        agg[agg.freeFraction < 0.3],
        x = "freeFraction",
        y = "unsuccessfull",
        hue = "description",
        )
sim.set(title = "Reconcile with paper",
        xlabel = "Free Fraction")
plt.xlim(reversed(plt.xlim()))
plt.savefig("Unsuc_freeFrac_rec")
# plt.show()

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        agg[agg.freeFraction < 0.3],
        x = "freeFraction",
        y = "successfull",
        hue = "description",
        )
sim.set(title = "Reconcile with paper",
        xlabel = "Free Fraction")
plt.xlim(reversed(plt.xlim()))
plt.savefig("Suc_freeFrac_rec")
# plt.show()

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        agg[agg.freeFraction < 0.3],
        x = "K",
        y = "unsuccessfull",
        hue = "description")
sim.set(title = "Reconcile with paper",
        xlabel = "Number of deletions")
plt.savefig("Unsuc_ndelete_rec")
# plt.show()

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        agg[agg.freeFraction < 0.3],
        x = "K",
        y = "successfull",
        hue = "description")
sim.set(title = "Reconcile with paper",
        xlabel = "Number of deletions")
plt.savefig("Suc_ndelete_rec")
# plt.show()

plot_data = list(dir.glob("./plot/*plot.txt"))
plot_agg = []

for dat in plot_data:
    df = pd.read_csv(dat, delim_whitespace=True, skiprows=2)
    desc = dat.open().readline().strip()
    df['description'] = desc
    df['filename'] = dat.name
    plot_agg.append(df)

plot_agg = pd.concat(plot_agg)
plot_agg = plot_agg.sort_values(by="description")
plot_agg["tombstones"] = 1 - plot_agg["freeFraction"].add(plot_agg["n"], axis=0)
plot_agg["sqrt_unsuccessfull"] = plot_agg["unsuccessfull"]**(-1/2)
# print(plot_agg)

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg,
        x = "freeFraction",
        y = "unsuccessfull",
        hue = "description",
        )
sim.set(title = "Stochastic Unsuccessful Search Cost",
        xlabel = "Free slots (not occupied nor tombstone)")
plt.xlim(reversed(plt.xlim()))
plt.savefig("Unsuc_freeFrac_plot")
# plt.show()

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg,
        x = "freeFraction",
        y = "successfull",
        hue = "description",
        )
sim.set(title = "Stochastic Successful Search Cost",
        xlabel = "Free slots (not occupied nor tombstone)")
plt.xlim(reversed(plt.xlim()))
plt.savefig("Suc_freeFrac_plot")
# plt.show()

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg.sort_values(by="description"),
        x = "K",
        y = "unsuccessfull",
        hue = "description")
sim.set(title = "Stochastic Unsuccessful Search Cost",
        xlabel = "Number of deletions")
plt.yscale('log')
plt.savefig("Unsuc_ndelete_plot")
# plt.show()

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg,
        x = "K",
        y = "successfull",
        hue = "description")
sim.set(title = "Stochastic Successful Search Cost",
        xlabel = "Number of deletions")
plt.yscale('log')
plt.savefig("Suc_ndelete_plot")

plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg[plot_agg.K == 16777215],
        x = "k",
        y = "unsuccessfull",
        hue = "description")
sim.set(title = "Stochastic Unsuccessful Search Cost On Last Iteration",
        xlabel = "k - stochastic weight")
plt.yscale('log')
plt.savefig("Unsuc_w_plot")


plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg,
        x = "freeFraction",
        y = "unsuccessfull",
        hue = "description")
sim.set(title = "Stochastic Unsucc. Search vs Fraction Free (excl. Tmb)",
        xlabel = "FreeFraction")
plt.yscale('log')
plt.savefig("Unsuc_freeFrac_plot")

plt.figure(figsize=(13.75, 10))
sim = sns.scatterplot(
        plot_agg,
        x = "freeFraction",
        y = "sqrt_unsuccessfull",
        hue = "description")
sim.set(title = "Stochastic Unsucc. Search vs Fraction Free (excl. Tmb)",
        xlabel = "FreeFraction")
plt.savefig("Unsuc_freeFrac_nonlog_plot")

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg,
        x = "K",
        y = "freeFraction",
        hue = "k")
sim.set(title = "Stochastic Fraction Free (excl Tmb) vs #deletions",
        xlabel = "# of deletions")
plt.yscale('log')
plt.savefig("freeFrac_ndeletes_plot")

plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        plot_agg,
        x = "K",
        y = "n",
        hue = "k")
sim.set(title = "Stochastic Load Over time",
        xlabel = "# of deletions")
plt.savefig("AvgLoad_ndeletes_plot")

# seaborne does this werid thing where it buckets your hue values
hue_plot = plot_agg.sort_values(by="E[Load]")
plt.figure(figsize=(13.75, 10))
sim = sns.lineplot(
        hue_plot,
        x = "K",
        y = "tombstones",
        hue = ["$%s$" % x for x in hue_plot["E[Load]"]])
sim.set(title = "Stochastic tombstones Over Time",
        xlabel = "# of deletions")
plt.savefig("Avgtombstones_ndeletes_plot")
