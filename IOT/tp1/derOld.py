import matplotlib.pyplot as plt
import json
from os import path as os_path

PATH = os_path.abspath(os_path.split(__file__)[0])


def get_gw(file_name):
    file = open(file_name, 'r')
    l_der = []
    for line in file:
        y = json.loads(line)
        y=y["all_gw"]
        t=0
        for i in y:
            t=t+1
        l_der.append((t/3)*100)
    res = 0
    for i in l_der:
        res = res+i
    return res/len(l_der)


def der_vs_sf():
    all_rssi = []
    for i in range(0, 6):
        all_rssi.append(get_gw(PATH + "/traces/trameDr" + str(i) + ".txt"))
    labels=["SF7", "SF8", "SF9", "SF10", "SF11", "SF12"]
    plt.figure("DER vs SF")
    plt.bar(labels, all_rssi ,width = 0.1)
    plt.title("DER en fonction du SF")
    plt.xlabel("Spreading Factor")
    plt.ylabel("DER (%)")


def display_der_vs_sf():
    der_vs_sf()
    plt.show()


if __name__ == "__main__":
    display_der_vs_sf()
