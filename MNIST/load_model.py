import numpy as np
import os
import urllib
import gzip
import struct
import cv2

import mxnet as mx
import matplotlib.pyplot as plt

def download_data(url, force_download=True): 
    fname = url.split("/")[-1]
    if force_download or not os.path.exists(fname):
        urllib.urlretrieve(url, fname)
    return fname
	
def read_data(label_url, image_url):
    with gzip.open(download_data(label_url)) as flbl:
        magic, num = struct.unpack(">II", flbl.read(8))
        label = np.fromstring(flbl.read(), dtype=np.int8)
    with gzip.open(download_data(image_url), 'rb') as fimg:
        magic, num, rows, cols = struct.unpack(">IIII", fimg.read(16))
        image = np.fromstring(fimg.read(), dtype=np.uint8).reshape(len(label), rows, cols)
    return (label, image)

path='http://yann.lecun.com/exdb/mnist/'
(train_lbl, train_img) = read_data(
    path+'train-labels-idx1-ubyte.gz', path+'train-images-idx3-ubyte.gz')
(val_lbl, val_img) = read_data(
    path+'t10k-labels-idx1-ubyte.gz', path+'t10k-images-idx3-ubyte.gz')
# load model back
prefix = 'mymodel'
iteration = 100
model_loaded = mx.model.FeedForward.load(prefix, iteration)

plt.imshow(val_img[2], cmap='Greys_r')
plt.axis('off')
plt.show()
prob = model_loaded.predict(val_img[2:3].astype(np.float32)/255)[0]
#assert max(prob) > 0.99, "Low prediction accuracy."
print 'Classified as %d with probability %f' % (prob.argmax(), max(prob))

img = cv2.imread('invertida.png', -1)
#img = cv2.imdecode(np.fromstring(img, np.uint8), -1)
img = cv2.resize(img[:,2], (28,28))
img = img.astype(np.float32).reshape((1,1,28,28))/255.0
#plt.imshow(img, cmap='Greys_r')
plt.axis('off')
plt.show()
#prob = model_loaded.predict(img.astype(np.float32)/255)[0]
prob = model_loaded.predict(img)[0]
print 'Classified as %d with probability %f' % (prob.argmax(), max(prob))