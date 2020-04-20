convert logo.svg -crop 645x186+1+15 cropText.png 
convert cropText.png -scale 214x64 cropText.png 
convert cropText.png -alpha remove cropText.png 
convert cropText.png -monochrome -threshold 50% monoText.xbm 
convert logo.svg -crop 41x41+649+0 cropRing.png 
convert cropRing.png -scale 14x14 cropRing.png 
convert cropRing.png -alpha remove cropRing.png
convert cropRing.png -monochrome -threshold 50% monoRing.xbm
rm -r cropRing.png cropText.png
