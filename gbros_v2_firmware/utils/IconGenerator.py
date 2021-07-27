from PIL import Image
import numpy as np

ICON_PATH=["watch.jpg", "timer.jpg", "acc.jpg", "camera.jpg"]
APPNAME=["watch", "timer", "acc", "camera"]

ICON_SIZE=(80,80)
SCREEN_SIZE=(240,240)

icons=[]

n=len(ICON_PATH)


resImg=Image.new("RGB", (240,240), (255,255,255))

for i in range(n):
    img=Image.open(ICON_PATH[i])
    RGBImg=img.convert("RGB")
    resizedImg=RGBImg.resize(ICON_SIZE)
    resImg.paste(resizedImg, (80*(i%3),80*(i//3)))

#resImg.show()
imgArray = np.array(resImg)

R5 = (imgArray[:,:,0]>>3).astype(np.uint16) << 11
G6 = (imgArray[:,:,1]>>2).astype(np.uint16) << 5
B5 = (imgArray[:,:,2]>>3).astype(np.uint16)

RGB565 = R5 | G6 | B5
icons=np.array(RGB565, dtype='<u2')
icons=np.resize(icons,-1)   
with open("../font/Icons.icon","wb") as f:
    f.write(icons.tobytes())
    """j=0
    f.write("short unsigned int Icons["+str(SCREEN_SIZE[0])+"]["+str(SCREEN_SIZE[1])+"]=\n{")
    for row in icons:
        f.write("{")
        for column in row:
            j+=1
            if(j==SCREEN_SIZE[0]*SCREEN_SIZE[1]):
                f.write(str(column)+"}};")
            elif(j%SCREEN_SIZE[0]==0):
                f.write(str(column)+"},")
            else:
                f.write(str(column)+", ")
                
        f.write("\n")"""

