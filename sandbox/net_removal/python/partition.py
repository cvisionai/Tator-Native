from skimage.io import imread
from scipy.sparse import bsr_matrix
import numpy as np

class DistanceMap :
  def __init__(self, nrows, ncols) :
    dist_r_vec = np.arange(-nrows + 1, nrows)
    dist_c_vec = np.arange(-ncols + 1, ncols)
    self.dist_r, self.dist_c = np.meshgrid(dist_r_vec, dist_c_vec)
    self.dist_r = self.dist_r.transpose()
    self.dist_c = self.dist_c.transpose()
    self.r_center = nrows - 1
    self.c_center = ncols - 1
    self.r = self.dist_r + self.r_center
    self.c = self.dist_c + self.c_center
    dist = np.sqrt(
        np.multiply(self.dist_r, self.dist_r) + 
        np.multiply(self.dist_c, self.dist_c))
    self.dist = np.copy(dist)
    dist[self.r_center, self.c_center] = 1.0 # avoid divide by zero
    self.norm_r = np.divide(self.dist_r, dist)
    self.norm_c = np.divide(self.dist_c, dist)
    self.norm_r[self.r_center, self.c_center] = 0.0
    self.norm_c[self.r_center, self.c_center] = 0.0

  def to_map(self, r_origin, c_origin, r_img, c_img) :
    r_map = r_img + self.r_center - r_origin
    c_map = c_img + self.c_center - c_origin
    return (r_map, c_map)

  def from_map(self, r_origin, c_origin, r_map, c_map) :
    r_img = r_map - self.r_center + r_origin
    c_img = c_map - self.c_center + c_origin
    return (r_img, c_img)

  def compute_field(self, r, c, over_r, over_c) :
    r_map, c_map = self.to_map(r, c, over_r, over_c)
    dist_over = self.dist[r_map, c_map]
    nearest = np.argmin(dist_over)
    field_r = -self.norm_r[r_map[nearest], c_map[nearest]]
    field_c = -self.norm_c[r_map[nearest], c_map[nearest]]
    return (field_r, field_c)

class Partition :
  """ Partitions an image via the transform described in :
      N. Anuja.  A transform for multiscale image segmentation by 
      integrated edge and region detection, 1996.
  """

  def __init__(self, img, num_p) :
    """ Constructor.
        Inputs:
            img - Path to an image.
            num_p - Number of partitions.
    """
    ## The image.
    self.img = imread(img, as_grey=True)
    nrows, ncols = self.img.shape
    ## Row vector.
    self.r_vec = np.arange(0, nrows)
    ## Column vector.
    self.c_vec = np.arange(0, ncols)
    col, row = np.meshgrid(self.c_vec, self.r_vec)
    ## Image row coordinates.
    self.r = row
    ## Image col coordinates.
    self.c = col
    img_min = np.min(self.img)
    img_max = np.max(self.img)
    ## Distance map.
    self.dmap = DistanceMap(nrows, ncols)
    ## Photometric scale parameter vector.
    sigma_g = np.linspace(0.0, img_max - img_min, num_p + 2)
    self.sigma_g = sigma_g[1:-1]
    ## Multiscale field.
    self.field_r, self.field_c = [self.compute_field(g) for g in self.sigma_g]

  def compute_field(self, sigma_g) :
    """ Computes affinity field for a given photometric scale.
        Inputs:
            sigma_g - The photometric scale.
    """
    field_r = np.zeros_like(self.img)
    field_c = np.zeros_like(self.img)
    for r in self.r_vec :
      for c in self.c_vec :
        # Find pixels with contrast > sigma_g
        over = np.greater(np.abs(self.img - self.img[r, c]), sigma_g)
        over_r = self.r[over]
        over_c = self.c[over]
        # Use distance map to compute the field
        f_r, f_c = self.dmap.compute_field(r, c, over_r, over_c)
        field_r[r, c] = f_r
        field_c[r, c] = f_c
      print "On row {0} of {1}...".format(r, self.r_vec[-1])
    return (field_r, field_c)
        

    

