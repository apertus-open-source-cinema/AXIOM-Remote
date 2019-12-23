INCLUDE "../Common/procdefs.ld"

SECTIONS {
  .shared_text ORIGIN(user_app):
  {
    . = ALIGN(4);
    KEEP(*Main.o(.text .text*))
    KEEP(*Main.o(.rodata))
    /*KEEP(*(.text .text*))
    KEEP(*(.rodata .rodata*))*/
    KEEP(*(.user_app*))
    /*KEEP(*(.main*))*/
    /*KEEP(*ILI9341Device.o(.rodata))
    KEEP(*ILI9341Device.o(.text .text*))*/
    . = ALIGN(4);
  } >user_app
}
