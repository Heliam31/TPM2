import pandas as pd
import matplotlib.pyplot as plt
def _read_csv(filename):
	df = pd.read_csv(filename, sep=' ', skipinitialspace=True)
	if df.columns[-1].startswith('Unnamed'):
		df.drop(columns=df.columns[-1:], axis=1, inplace=True)
	return df

def show_csv(filename, norm=False):
	a = _read_csv(filename)
	a['#timestamp'] = a['#timestamp']-a['#timestamp'][0]
	if norm:
		tmp = (a/a.max())
		tmp['#timestamp'] = a['#timestamp']
		a = tmp
	a.plot(x='#timestamp')

show_csv('/tmp/data.csv', norm=True)
plt.show(block=True)
