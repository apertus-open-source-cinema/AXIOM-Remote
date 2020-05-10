all: RingLogo.xbm TextLogo.xbm Home_icon.xbm Back_icon.xbm Up_icon.xbm Down_icon.xbm Delete_icon.xbm Help_icon.xbm

RingLogo.xbm: 
	sed -i "/#c2bfbc/ s/fill-opacity:1/fill-opacity:0/" ApertusLogo.svg
	magick ApertusLogo.svg -trim +repage -density 800 -filter point +negate -negate -threshold 50% -resize 14x14 $@
	
TextLogo.xbm: 
	sed -i "/#c2bfbc/ s/fill-opacity:0/fill-opacity:1/" ApertusLogo.svg
	sed -i "/#f47248/ s/fill-opacity:1/fill-opacity:0/" ApertusLogo.svg
	magick ApertusLogo.svg -trim +repage -density 800 -filter point +negate -negate -threshold 50% -resize 214x64 $@

Home_icon.xbm: home_icon.svg
	magick -density 800 $< -filter point -threshold 50% -resize 24x24 $@

Back_icon.xbm: back_icon.svg
	magick -density 800 $< -filter point -threshold 50% -resize 24x24 $@

Up_icon.xbm: up_icon.svg
	magick -density 800 $< -filter point -threshold 50% -resize 24x24 $@

Down_icon.xbm: down_icon.svg
	magick -density 800 $< -filter point -threshold 50% -resize 24x24 $@
	
Delete_icon.xbm: delete_icon.svg
	magick -density 800 $< -filter point -threshold 50% -resize 24x24 $@

Help_icon.xbm: help_icon.svg
	magick -density 800 $< -filter point -threshold 50% -resize 24x24 $@

clean:
	sed -i "s/fill-opacity:0/fill-opacity:1/" ApertusLogo.svg
