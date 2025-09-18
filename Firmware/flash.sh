#picotool load -f -u build/AXIOM_Remote_Firmware.uf2
#--bus 1 --address $1
#picotool reboot
picotool load build/AXIOM_Remote_Firmware.uf2 -u -x -F
