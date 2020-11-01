INPUT  := $(wildcard *_icon.svg)
OUTPUT := $(INPUT:.svg=.h)
TEMP_DIR = temp

all: build_dir ApertusRingLogo.h ApertusTextLogo.h $(OUTPUT) clean_temp

build_dir:
	@ mkdir -p $(TEMP_DIR)

# TODO: Reduce redundant calls
ApertusRingLogo.h:
	sed "/#c2bfbc/ s/fill-opacity:1/fill-opacity:0/" ApertusLogo.svg > $(TEMP_DIR)/TempRing.svg
	magick -density 200 $(TEMP_DIR)/TempRing.svg -trim +repage -filter point -monochrome -resize 14x14 xbm:- | ./convert_xbm_to_h.sh $@ -

ApertusTextLogo.h:
	sed "/#f47248/ s/fill-opacity:1/fill-opacity:0/" ApertusLogo.svg > $(TEMP_DIR)/TempText.svg
	magick -density 200 $(TEMP_DIR)/TempText.svg -trim +repage -filter point -monochrome -resize 214x64 xbm:- | ./convert_xbm_to_h.sh $@ -

$(OUTPUT): %_icon.h: %_icon.svg
	magick -density 200 $< -filter point -monochrome -resize 24x24 xbm:- | ./convert_xbm_to_h.sh $@ -

PHONY: clean_temp
clean_temp:
	rm -rf $(TEMP_DIR)

PHONY: clean
clean:
	rm -rf $(TEMP_DIR) *_icon.h *Logo.h
