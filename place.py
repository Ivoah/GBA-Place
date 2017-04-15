from PIL import Image

RGB = lambda r, g, b: ((b>>3) << 10) | ((g>>3) << 5) | (r>>3)

place = Image.open('place.png')
placec = open('source/place.c', 'w')

placec.write('#include <gba.h>\n\nconst u16 place[1000000] = {\n    ')
for i, p in enumerate(place.getdata()):
    if i > 0 and i%10 == 0: placec.write('\n    ')
    placec.write(f'{RGB(*p[:3]):#06x},')

placec.write('\n};')
placec.close()

placeh = open('source/place.h', 'w')
placeh.write('u16 place[1000000];')
placeh.close()
