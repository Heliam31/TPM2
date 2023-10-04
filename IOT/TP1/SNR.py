import matplotlib.pyplot as plt
import json

def get_snr(file_name):
    file = open(file_name, 'r')
    for line in file:
        a = json.loads(line)
        ligne = a["best_gw"]
        snr = ligne["lsnr"]
        print(snr)

get_snr('Resultats/trameDr0.json')     