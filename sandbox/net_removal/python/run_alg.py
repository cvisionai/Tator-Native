#!/usr/bin/env python

if __name__ == "__main__" :
  from partition import Partition
  import os
  data_dir = os.path.join(os.getcwd(), '../data')
  imgs = [os.path.join(data_dir, f) for f in os.listdir(data_dir) 
    if os.path.isfile(os.path.join(data_dir, f)) and f.endswith('.png')]
  for img in imgs :
    p = Partition(img, 1)
    for i, sigma_g in enumerate(p.sigma_g) :
      plt.figure()
      plt.quiver(p.r, p.c, p.field_r[i], p.field_c[i])
      plt.show()
