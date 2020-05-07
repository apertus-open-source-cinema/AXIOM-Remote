convert ApertusLogo.svg -crop 645x186+1+15 cropText.png 
convert cropText.png -scale 214x64 cropText.png 
convert cropText.png -alpha remove cropText.png 
convert cropText.png -threshold 50% TextLogo.xbm 
convert ApertusLogo.svg -crop 41x41+649+0 cropRing.png 
convert cropRing.png -scale 14x14 cropRing.png 
convert cropRing.png -alpha remove cropRing.png
convert cropRing.png -threshold 50% RingLogo.xbm

convert home_icon.svg -scale 24x24 cropHome.png 
convert cropHome.png -alpha remove cropHome.png 
convert cropHome.png -threshold 50% Home_icon.xbm
convert back_icon.svg -scale 24x24 cropBack.png 
convert cropBack.png -alpha remove cropBack.png 
convert cropBack.png -threshold 50% Back_icon.xbm 
convert delete_icon.svg -scale 24x24 cropDelete.png 
convert cropDelete.png -alpha remove cropDelete.png 
convert cropDelete.png -threshold 50% Delete_icon.xbm 
convert help_icon.svg -scale 24x24 cropHelp.png 
convert cropHelp.png -alpha remove cropHelp.png 
convert cropHelp.png -threshold 50% Help_icon.xbm 
convert up_icon.svg -scale 24x24 cropUp.png 
convert cropUp.png -alpha remove cropUp.png 
convert cropUp.png -threshold 50% Up_icon.xbm 
convert down_icon.svg -scale 24x24 cropDown.png 
convert cropDown.png -alpha remove cropDown.png 
convert cropDown.png -threshold 50% Down_icon.xbm
rm -r crop*
