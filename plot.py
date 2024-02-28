import matplotlib.pyplot as plt
import pandas as pd

# read data from .cpp output
df = pd.read_csv("misc\\arrays.dat", comment='#', sep=',')

# initialize plot
fig, ax = plt.subplots(figsize=(16, 8))

# plot second column against first column and format
ax.plot(df['wavelengths'], df['spectrum'])
ax.set_title('Resulting Spectrum')
ax.set_xlabel('Wavelength (Angstroms)')
ax.set_ylabel('Power')
ax.grid(True)

# save fig as png, and show on screen
plt.savefig(".\\out\\test.png")
plt.show()
