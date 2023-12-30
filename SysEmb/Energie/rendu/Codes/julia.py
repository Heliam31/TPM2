import matplotlib.pyplot as plt
import numpy as np

taille = 900
xmin, xmax = -1, 1
ymin, ymax = -1, 1
iterationmax = 100

a = -0.8
b = 0.156

pixels = np.zeros((taille, taille, 3), dtype='i')

for line in range(taille):
    for col in range(taille):
        i = 1
        x = xmin+col*(xmax-xmin)/taille
        y = ymax-line*(ymax-ymin)/taille
        while i<=iterationmax and (x**2+y**2)<=4:

            x, y = x**2-y**2+a, 2*x*y+b
            i += 1
            if i>iterationmax and (x**2+y**2)<=4:
                pixels[col,line] = (0,0,0)
            else:
                pixels[col,line] = ((4*i)%256,2*i,(6*i)%256)

plt.imshow(pixels)
plt.savefig("/home/mpoitelea/python/juliadata/dataJ.png")
