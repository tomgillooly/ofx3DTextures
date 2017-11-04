#!/usr/bin/env python

import numpy as np

from skimage.color import hsv2rgb, rgb2hsv
import skimage.io as io

for idx, hue in enumerate(np.arange(0, 1, 0.01)):
    colour = hsv2rgb(np.broadcast_to((hue, 1, 0.5), (1, 1, 3)))*255

    patch = np.tile(colour, (200, 200, 1))
    
    io.imsave('colour_texture/colour_text{0:03}.tif'.format(idx), patch.astype(np.uint8))