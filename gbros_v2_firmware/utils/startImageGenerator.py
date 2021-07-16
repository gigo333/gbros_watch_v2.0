from PIL import Image
import numpy as np

IMAGE_PATH="startImage.jpg"

IMAGE_SIZE=(240,240)


img=Image.open(IMAGE_PATH)
RGBImg=img.convert("RGB")
resizedImg=RGBImg.resize(IMAGE_SIZE)

imgArray = np.array(resizedImg)

R5 = (imgArray[:,:,0]>>3).astype(np.uint16) << 11
G6 = (imgArray[:,:,1]>>2).astype(np.uint16) << 5
B5 = (imgArray[:,:,2]>>3).astype(np.uint16)

RGB565 = R5 | G6 | B5
image=np.array(RGB565, dtype='>u2')
   
with open("../main/startImage.h","w") as f:
    f.write("short unsigned int startImg["+str(IMAGE_SIZE[0])+"]["+str(IMAGE_SIZE[1])+"]=\n{")
    j=0;
    for row in image:
        f.write("{")
        for column in row:
            j+=1
            if(j==IMAGE_SIZE[0]*IMAGE_SIZE[1]):
                f.write(str(column)+"}};")
            elif(j%IMAGE_SIZE[0]==0):
                f.write(str(column)+"},")
            else:
                f.write(str(column)+", ")
                
        f.write("\n")

