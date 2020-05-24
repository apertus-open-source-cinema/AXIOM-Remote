INPUT  := $(wildcard *_icon.svg)
OUTPUT := $(INPUT:.svg=.h)

define generate_header_file
	cat sample.h > $(1)
	sed -i -e 's/icon/$(1)/;s/IconName/$(1)/;s/SAMPLE/$(1)/;s/$(1)\_H/\U&/;/$(1)/ s/\.h//;s/\.H//' $(1)

    	WIDTH=$$(sed -n "s/^.*width //p" $(2));	\
    	sed -i "/data\[/ s/%WIDTH%/$$WIDTH/" $(1); \
    	sed -i "/Width\ =\ / s/%WIDTH%/$$WIDTH/" $(1)
    	
    	HEIGHT=$$(sed -n "s/^.*height //p" $(2)); \
    	sed -i "/*\ / s/%HEIGHT%/$$HEIGHT/" $(1); \
    	sed -i "/Height\ =\ / s/%HEIGHT%/$$HEIGHT/" $(1)
    		
    	DATA=$$(sed -n "4,$$ p" $(2)); \
	echo $$DATA >> "TempData.txt"
	sed -i -e '/%IMAGE_DATA%/r TempData.txt' -i -e 's/%IMAGE_DATA%};//' $(1)
	rm -rf TempData.txt
endef

.PHONY: all

all: ApertusRingLogo.h ApertusTextLogo.h $(OUTPUT)

ApertusRingLogo.h: 
	sed "/#c2bfbc/ s/fill-opacity:1/fill-opacity:0/" ApertusLogo.svg > TempRing.svg
	magick -density 800 TempRing.svg -trim +repage -filter point +negate -negate -threshold 50% -resize 14x14 $@.xbm
	$(call generate_header_file,$@,$@.xbm)
	
ApertusTextLogo.h: 
	sed "/#f47248/ s/fill-opacity:1/fill-opacity:0/" ApertusLogo.svg > TempText.svg
	magick -density 800 TempText.svg -trim +repage -filter point +negate -negate -threshold 50% -resize 214x64 $@.xbm
	$(call generate_header_file,$@,$@.xbm)

$(OUTPUT): %_icon.h: %_icon.svg
	magick -density 800 $< -filter point -threshold 50% -resize 24x24 $@.xbm
	$(call generate_header_file,$@,$@.xbm)

clean:
	rm -rf Temp* *.xbm
