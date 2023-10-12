import matplotlib.pyplot as plt
import json
from os import path as os_path

PATH = os_path.abspath(os_path.split(__file__)[0])


def get_rssi(file_name):
    file = open(file_name, 'r')
    l_rssi = []
    for line in file:
        y = json.loads(line)
        l_rssi.append(y["all_gw"]["rssi"])
    return l_rssi


def rssi_vs_sf():
    all_rssi = []
    for i in range(0, 6):
        all_rssi.append(get_rssi(PATH + "/traces/trameDr" + str(i) + ".txt"))

    plt.figure("RSSI vs SF")
    plt.boxplot(all_rssi, labels=["SF7", "SF8", "SF9", "SF10", "SF11", "SF12"])
    plt.title("RSSI en fonction du SF")
    plt.xlabel("Spreading Factor")
    plt.ylabel("RSSI (dBm)")


def display_rssi_vs_sf():
    rssi_vs_sf()
    plt.show()


if __name__ == "__main__":
    display_rssi_vs_sf()
