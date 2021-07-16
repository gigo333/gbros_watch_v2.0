from PIL import Image
import numpy as np

ICON_PATH=["watch.jpg", "acc.jpg", "camera.jpg"]
APPNAME=["watch", "acc", "camera"]

ICON_SIZE=(80,80)

icons=[]

n=len(ICON_PATH)

for i in range(n):
    img=Image.open(ICON_PATH[i])
    RGBImg=img.convert("RGB")
    resizedImg=RGBImg.resize(ICON_SIZE)

    imgArray = np.array(resizedImg)

    R5 = (imgArray[:,:,0]>>3).astype(np.uint16) << 11
    G6 = (imgArray[:,:,1]>>2).astype(np.uint16) << 5
    B5 = (imgArray[:,:,2]>>3).astype(np.uint16)

    RGB565 = R5 | G6 | B5
    icons.append(np.array(RGB565, dtype='>u2'))
   
with open("../main/apps/appIcons.h","w") as f:
    for i in range(n):
        j=0
        f.write("short unsigned int "+APPNAME[i]+"Icon["+str(ICON_SIZE[0])+"]["+str(ICON_SIZE[1])+"]=\n{")
        icon=icons[i]
        for row in icon:
            f.write("{")
            for column in row:
                j+=1
                if(j==ICON_SIZE[0]*ICON_SIZE[1]):
                    f.write(str(column)+"}};")
                elif(j%ICON_SIZE[0]==0):
                    f.write(str(column)+"},")
                else:
                    f.write(str(column)+", ")
                
            f.write("\n")

