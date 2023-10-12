import matplotlib.pyplot as plt
import json
from os import path as os_path

PATH = os_path.abspath(os_path.split(__file__)[0])


def get_snr(file_name):
    file = open(file_name, 'r')
    l_snr = []
    for line in file:
        y = json.loads(line)
        l_snr.append(y["best_gw"]["lsnr"])
    return l_snr


def snr_vs_sf():
    all_snr = []
    for i in range(0, 6):
        all_snr.append(get_snr(PATH + "/traces/trameDr" + str(i) + ".txt"))

    plt.figure("SNR vs SF")
    plt.boxplot(all_snr, labels=["SF7", "SF8", "SF9", "SF10", "SF11", "SF12"])
    plt.title("SNR en fonction du SF")
    plt.xlabel("Spreading Factor")
    plt.ylabel("SNR (dB)")


def display_snr_vs_sf():
    snr_vs_sf()
    plt.show()


if __name__ == "__main__":
    display_snr_vs_sf()
