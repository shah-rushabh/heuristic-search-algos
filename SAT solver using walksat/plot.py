import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# %matplotlib inline

filename = "output.txt"
df = pd.read_csv(filename)

plt.plot(df['m/n \t'],df['log(Average iterations)'], 'og-')

plt.xlabel("m/n ratio")
plt.ylabel("log(Avg no of iterations)")
plt.title("No of problems for each value of m/n = 100\n (m/n ranges from 0.25 to 8.25 with 0.25 diff between consecutive points)")

plt.savefig('graph.png')
# plt.show()
