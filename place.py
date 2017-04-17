#!/usr/local/bin/python3
from PIL import Image

RGB = lambda r, g, b: ((b>>3) << 10) | ((g>>3) << 5) | (r>>3)

place = Image.open('place.png')
placec = open('source/place.c', 'w')

size = place.width*place.height

placec.write(f'''#include <gba.h>
const u16 place_width = {place.width};
const u16 place_height = {place.height};

const u16 place[{size}] = {{
    ''')
for i, p in enumerate(place.getdata()):
    if i > 0 and i%10 == 0: placec.write('\n    ')
    placec.write(f'{RGB(*p[:3]):#06x},')

placec.write('\n};')
placec.close()

placeh = open('source/place.h', 'w')
placeh.write(f'''const u16 place_width;
const u16 place_height;
const u16 place[{size}];
''')
placeh.close()
