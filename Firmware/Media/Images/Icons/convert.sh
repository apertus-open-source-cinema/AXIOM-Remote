convert home.svg -scale 24x24 cropHome.png 
convert cropHome.png -alpha remove cropHome.png 
convert cropHome.png -monochrome -threshold 50% monoHome.xbm
convert back.svg -scale 24x24 cropBack.png 
convert cropBack.png -alpha remove cropBack.png 
convert cropBack.png -monochrome -threshold 50% monoBack.xbm 
convert delete.svg -scale 24x24 cropDelete.png 
convert cropDelete.png -alpha remove cropDelete.png 
convert cropDelete.png -monochrome -threshold 50% monoDelete.xbm 
convert help.svg -scale 24x24 cropHelp.png 
convert cropHelp.png -alpha remove cropHelp.png 
convert cropHelp.png -monochrome -threshold 50% monoHelp.xbm 
convert up.svg -scale 24x24 cropUp.png 
convert cropUp.png -alpha remove cropUp.png 
convert cropUp.png -monochrome -threshold 50% monoUp.xbm 
convert down.svg -scale 24x24 cropDown.png 
convert cropDown.png -alpha remove cropDown.png 
convert cropDown.png -monochrome -threshold 50% monoDown.xbm
rm -r crop*
