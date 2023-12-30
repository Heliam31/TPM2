def tpsExec(fich):
	with open(fich, "r") as file:
		oui = file.readlines()
		ligneF = oui[-1]
		ligneD = oui[1]
		premEl = ligneD.split(',')[0]
		lastEl = ligneF.split(',')[0]
		timeDeb = premEl.split()[0]
		timefin = lastEl.split()[0]
		file.close()
	return float(timefin)-float(timeDeb)

def ener(fich):
	with open(fich, "r") as file:
		oui = file.readlines()
		ener1 = 0
		ener2 = 0
		for i in oui[1:]:
			splitaj = i.split()
			print(splitaj)
			ener2 = (ener2 + float(splitaj[2]))/2
			ener1 = (ener1+float(splitaj[1]))/2
		file.close()
	return ((ener1+ener2)/2)/1000

def Puissance(fich):
	tps = tpsExec(fich)
	nrg = ener(fich)
	return nrg/tps
file = 'dataJulC.csv'
print("tempsExec: ", tpsExec(file)," Energie: ",ener(file) ,"Puissance: ",Puissance(file))
