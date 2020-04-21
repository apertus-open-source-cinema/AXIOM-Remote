convert ApertusLogo.svg -crop 645x186+1+15 cropText.png 
convert cropText.png -scale 214x64 cropText.png 
convert cropText.png -alpha remove cropText.png 
convert cropText.png -monochrome -threshold 50% monoText.xbm 
convert ApertusLogo.svg -crop 41x41+649+0 cropRing.png 
convert cropRing.png -scale 14x14 cropRing.png 
convert cropRing.png -alpha remove cropRing.png
convert cropRing.png -monochrome -threshold 50% monoRing.xbm

convert home_icon.svg -scale 24x24 cropHome.png 
convert cropHome.png -alpha remove cropHome.png 
convert cropHome.png -monochrome -threshold 50% monoHome_icon.xbm
convert back_icon.svg -scale 24x24 cropBack.png 
convert cropBack.png -alpha remove cropBack.png 
convert cropBack.png -monochrome -threshold 50% monoBack_icon.xbm 
convert delete_icon.svg -scale 24x24 cropDelete.png 
convert cropDelete.png -alpha remove cropDelete.png 
convert cropDelete.png -monochrome -threshold 50% monoDelete_icon.xbm 
convert help_icon.svg -scale 24x24 cropHelp.png 
convert cropHelp.png -alpha remove cropHelp.png 
convert cropHelp.png -monochrome -threshold 50% monoHelp_icon.xbm 
convert up_icon.svg -scale 24x24 cropUp.png 
convert cropUp.png -alpha remove cropUp.png 
convert cropUp.png -monochrome -threshold 50% monoUp_icon.xbm 
convert down_icon.svg -scale 24x24 cropDown.png 
convert cropDown.png -alpha remove cropDown.png 
convert cropDown.png -monochrome -threshold 50% monoDown_icon.xbm
rm -r crop*
