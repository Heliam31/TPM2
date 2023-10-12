import matplotlib.pyplot as plt
import json
from os import path as os_path

PATH = os_path.abspath(os_path.split(__file__)[0])


def get_(file_name):
    file = open(file_name, 'r')
    l_ = []
    for line in file:
        y = json.loads(line)
        l_.append(y["best_gw"])
    return l_


def der_gw_sf():
    all_ = []
    for i in range(0, 6):
        all_.append(get_(PATH + "/traces/trameDr" + str(i) + ".txt"))
    

    plt.figure("DER vs Gateway vs SF")
    plt.boxplot(all_, labels=["SF7", "SF8", "SF9", "SF10", "SF11", "SF12"])
    plt.title("DER vs Gateway vs SF")
    plt.xlabel("Spreading Factor")
    plt.ylabel("DER")
    plt.zlabel("Gateway")


def display_der_gw_sf():
    der_gw_sf()
    plt.show()


if __name__ == "__main__":
    display_der_gw_sf()
