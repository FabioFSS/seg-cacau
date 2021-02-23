import cv2
import numpy as np

# Remove o verde da imagem
def color_filter(image):
    new_image = np.zeros_like(image)
    RED, GREEN, BLUE = (2, 1, 0)
    reds = image[:, :, RED]
    greens = image[:, :, GREEN]
    blues = image[:, :, BLUE]
    mask = (greens < 20) | (reds > greens) | (blues > greens)

    for i in range(3):
        new_image[:, :, i] = np.where(mask, image[:, :, i], 255)

    return new_image

# Carrega imagem original
original = cv2.imread('./imagem.jpg')
original = color_filter(original)

height, width, channels = original.shape

original[0:int(height*0.1), 0:width] = (255, 255, 255)
original[height-int(height*0.05):height, 0:width] = (255, 255, 255)



# Aplica o filtro mediano 30 vezes
for i in range(30):
    cv2.medianBlur(original, 5, original)
# Aplica o filtro Gaussiano 30 vezes
for i in range(30):
    cv2.GaussianBlur(original, (5, 5), 0, original)

# Escreve a nova imagem
cv2.imwrite('./pre_processed.jpg', original)