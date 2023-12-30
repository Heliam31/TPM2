import pandas as pd
import matplotlib.pyplot as plt

def _read_csv(filename):
    df = pd.read_csv(filename, sep=' ', skipinitialspace=True)
    if df.columns[-1].startswith('Unnamed'):
        df.drop(columns=df.columns[-1:], axis=1, inplace=True)
    return df

def tpsExec(fich):
	with open(fich, "r") as file:
		ligne = file.readlines()[-1]
		premierElement = ligne.split(',')[0]
	li = premierElement.split()
	timefin =  li[0]
	file.close()
	with open(fich, "r") as file:
		ligne = file.readlines()[1]
		premierEl = ligne.split(',')[0]
	li = premierEl.split()
	timeDeb = li[0]
	file.close()
	return float(timefin)-float(timeDeb)

def ener(fich):
	with open(fich,"r") as file:
		ligne 

print("deb", tpsExec('dataJul.csv') ,"fin")
