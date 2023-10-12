import matplotlib.pyplot as plt
import json
from os import path as os_path

PATH = os_path.abspath(os_path.split(__file__)[0])


def get_gw(file_name):
    file = open(file_name, 'r')
    l_der = []
    nbBuSante=0 #"MTCDT-210A / LoRa gateway @ BU santé"
    nbEcolab = 0 #"MTCDTIP-266A / LoRa gateway @ Ecolab"
    nbBuSciences = 0 #"MTCDT-210A / LoRa gateway @ BU sciences"
    for line in file:
        y = json.loads(line)
        y=y["all_gw"]
        for i in y:
            print(i)
            if (i["desc"] == "MTCDT-210A / LoRa gateway @ BU santé"):
                nbBuSante = nbBuSante+1
            if (i["desc"] == "MTCDTIP-266A / LoRa gateway @ Ecolab"):
                nbEcolab = nbEcolab +1
            if (i["desc"] == "MTCDT-210A / LoRa gateway @ BU sciences"):
                nbBuSciences = nbBuSciences+1
    print([(nbBuSante*100)/30, (nbEcolab*100)/30, (nbBuSciences*100)/30])
    return [(nbBuSante*100)/30, (nbEcolab*100)/30, (nbBuSciences*100)/30]


def der_vs_sf():
    all_buSante = []
    all_Ecolab = []
    all_buScience = []
    all_rssi = []
    for i in range(0, 6):
        all_rssi.append(get_gw(PATH + "/traces/trameDr" + str(i) + ".txt"))

    for i in all_rssi:
        all_buSante.append(i[0])
        all_Ecolab.append(i[1])
        all_buScience.append(i[2])

    labels=["SF7", "SF8", "SF9", "SF10", "SF11", "SF12"]
    plt.figure("DER BuSante vs SF")
    plt.bar(labels, all_buSante ,width = 0.1)
    plt.title("DER de la BuSante en fonction du SF")
    plt.xlabel("Spreading Factor")
    plt.ylabel("DER (%)")

    plt.figure("DER Ecolab vs SF")
    plt.bar(labels, all_Ecolab ,width = 0.1)
    plt.title("DER de l'Ecolab  en fonction du SF")
    plt.xlabel("Spreading Factor")
    plt.ylabel("DER (%)")

    plt.figure("DER BuSciences vs SF")
    plt.bar(labels, all_buScience ,width = 0.1)
    plt.title("DER de la BuSciences en fonction du SF")
    plt.xlabel("Spreading Factor")
    plt.ylabel("DER (%)")

def display_der_vs_sf():
    der_vs_sf()
    plt.show()


if __name__ == "__main__":
    display_der_vs_sf()
