# https://stackoverflow.com/questions/2563822/how-do-you-composite-an-image-onto-another-image-with-pil-in-python
# NOT OUR CODE; COPY AND PASTE FOR DEBUGGING

from PIL import Image
img = Image.open('visualizer.png', 'r')
img_w, img_h = img.size
img.save("1.png")

w_scale = 1920 / img_w
h_scale = 1080 / img_h 
scale = min(w_scale, h_scale)
img = img.resize((int(img_w * scale), int(img_h * scale)))
img_w, img_h = img.size
img.save("2.png")

background = Image.new('RGBA', (1920, 1080), (255, 255, 255, 255))
bg_w, bg_h = background.size
offset = ((bg_w - img_w) // 2, (bg_h - img_h) // 2)
background.paste(img, offset)
background.save('visualizer.png')
background.save("3.png")
