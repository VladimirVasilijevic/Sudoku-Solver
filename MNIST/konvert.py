import cv2
import numpy as np

def inverte(imagem, name):
    imagem = (255-imagem)
    cv2.imwrite(name, imagem)
	
def inverte2(imagem, name):
    for x in np.nditer(imagem, op_flags=['readwrite']):
        x = abs(x - 255)
    cv2.imwrite(name, imagem)



nome = str('9.png')
image = cv2.imread(nome)
gs_imagem = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
inverte(gs_imagem, "White9.png")
inverte2(gs_imagem, "invertida2.png")