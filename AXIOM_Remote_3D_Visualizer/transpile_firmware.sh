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
         ../Firmware/CentralDB/CentralDBObserver.cpp"
         
emsdk-run emcc -O2 -Wcast-align -Wover-aligned -fsanitize=undefined -s WARN_UNALIGNED=1 -s INITIAL_MEMORY=268435456 --bind -fexceptions -s ENVIRONMENT=web -s WASM=1 -s ASSERTIONS=1 -std=c++11 -s MODULARIZE=1 -s EXPORT_NAME="axiomRemoteFirmware" $SOURCES -o src/js/FW/axiom_remote_firmware.js
#-fsanitize=address 
#emsdk-run emcc --bind -s ASSERTIONS=1 -std=c++11 -s MODULARIZE=1 -s EXPORT_ES6=1 -s EXPORT_NAME="axiomRemoteFirmware" ../Firmware/TestClass.cpp ../Firmware/Painter/Painter.cpp ../Firmware/MenuSystem.cpp ../Firmware/Screens/MainPage.cpp ../Firmware/Screens/IScreen.cpp ../Firmware/Widgets/ImageButton.cpp -o js/FW/main.js