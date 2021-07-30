#!/bin/sh
SOURCES="FirmwareBinder/FirmwareBinder.cpp \
         ../Firmware/UI/Painter/Painter.cpp \
         ../Firmware/UI/MenuSystem.cpp \
         ../Firmware/UI/Screens/MainPage.cpp \
         ../Firmware/UI/Screens/MainMenu.cpp \
         ../Firmware/UI/Screens/IScreen.cpp \
         ../Firmware/UI/Screens/Menu.cpp \
         ../Firmware/UI/Screens/ParameterListScreen.cpp \
         ../Firmware/UI/Screens/NumericValueScreen.cpp \
         ../Firmware/UI/Screens/SettingsSubMenu1.cpp \
         ../Firmware/UI/Screens/WhiteBalanceScreen.cpp \
         ../Firmware/UI/Widgets/ImageButton.cpp \
         ../Firmware/UI/Widgets/MenuItem.cpp \
         ../Firmware/UI/Widgets/PopUpMenuItem.cpp \
         ../Firmware/UI/Widgets/PopUpParameterMenu.cpp \
         ../Firmware/UI/Widgets/ParameterMenuItem.cpp \
         ../Firmware/UI/Widgets/NumericMenuItem.cpp  \
         ../Firmware/CentralDB/Attribute.cpp \
         ../Firmware/CentralDB/CentralDB.cpp \
         ../Firmware/CentralDB/CentralDBObserver.cpp \
         ../Firmware/GlobalSettings.cpp"

         
emcc -O2 -Wcast-align -Wover-aligned -fsanitize=undefined -s WARN_UNALIGNED=1 -s INITIAL_MEMORY=268435456 --bind -fexceptions -s ENVIRONMENT=web -s WASM=1 -s ASSERTIONS=1 -std=c++11 -s MODULARIZE=1 -s EXPORT_NAME="axiomRemoteFirmware" $SOURCES -o src/js/FW/axiom_remote_firmware.js
