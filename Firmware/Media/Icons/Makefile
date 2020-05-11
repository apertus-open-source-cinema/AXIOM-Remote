INPUT  := $(wildcard *_icon.svg)
OUTPUT := $(INPUT:.svg=.h)

all: ApertusRingLogo.h ApertusTextLogo.h $(OUTPUT)

ApertusRingLogo.h: 
	sed "/#c2bfbc/ s/fill-opacity:1/fill-opacity:0/" ApertusLogo.svg > TempRing.svg
	magick TempRing.svg -trim +repage -density 800 -filter point +negate -negate -threshold 50% -resize 14x14 xbm:$@
	
ApertusTextLogo.h: 
	sed "/#f47248/ s/fill-opacity:1/fill-opacity:0/" ApertusLogo.svg > TempText.svg
	magick TempText.svg -trim +repage -density 800 -filter point +negate -negate -threshold 50% -resize 214x64 xbm:$@

$(OUTPUT): $(INPUT)
	magick -density 800 $< -filter point -threshold 50% -resize 24x24 xbm:$@

clean:
	rm -rf Temp*
