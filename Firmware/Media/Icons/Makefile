INPUT  := $(wildcard *_icon.svg)
OUTPUT := $(INPUT:.svg=.h)
TEMP_DIR = temp

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
	# TODO: Add temp files for every icon to prevent removing over and over, just at the end of the build
	echo $$DATA >> $(TEMP_DIR)/TempData.txt
	sed -i -e '/%IMAGE_DATA%/r $(TEMP_DIR)/TempData.txt' -i -e 's/%IMAGE_DATA%};//' $(1)
	rm -f $(TEMP_DIR)/TempData.txt
endef

all: build_dir ApertusRingLogo.h ApertusTextLogo.h $(OUTPUT) clean_temp

build_dir:
	@ mkdir -p $(TEMP_DIR)

# TODO: Reduce redundant calls
# TODO: Reduce the need for temporary files to generate multiple files in parallel
ApertusRingLogo.h:
	sed "/#c2bfbc/ s/fill-opacity:1/fill-opacity:0/" ApertusLogo.svg > $(TEMP_DIR)/TempRing.svg
	magick -density 800 $(TEMP_DIR)/TempRing.svg -trim +repage -filter point +negate -negate -threshold 50% -resize 14x14 $(TEMP_DIR)/$@.xbm
	$(call generate_header_file,$@,$(TEMP_DIR)/$@.xbm)

ApertusTextLogo.h:
	sed "/#f47248/ s/fill-opacity:1/fill-opacity:0/" ApertusLogo.svg > $(TEMP_DIR)/TempText.svg
	magick -density 800 $(TEMP_DIR)/TempText.svg -trim +repage -filter point +negate -negate -threshold 50% -resize 214x64 $(TEMP_DIR)/$@.xbm
	$(call generate_header_file,$@,$(TEMP_DIR)/$@.xbm)

$(OUTPUT): %_icon.h: %_icon.svg
	magick -density 800 $< -filter point -threshold 50% -resize 24x24 $(TEMP_DIR)/$@.xbm
	$(call generate_header_file,$@,$(TEMP_DIR)/$@.xbm)

PHONY: clean_temp
clean_temp:
	rm -rf $(TEMP_DIR)

PHONE: clean
clean:
	rm -rf $(TEMP_DIR) *_icon.h *Logo.h
