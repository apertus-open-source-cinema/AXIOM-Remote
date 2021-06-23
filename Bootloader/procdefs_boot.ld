/*--- Bootloader linker script ---*/
OUTPUT_FORMAT("elf32-tradlittlemips")
OUTPUT_ARCH(pic32mx)
ENTRY(_reset)

/*
 * Provide for a minimum stack and heap size
 * - _min_stack_size - represents the minimum space that must be made
 *                     available for the stack.  Can be overridden from
 *                     the command line using the linker's --defsym option.
 * - _min_heap_size  - represents the minimum space that must be made
 *                     available for the heap.  Must be specified on
 *                     the command line using the linker's --defsym option.
 */
EXTERN (_min_stack_size _min_heap_size)
PROVIDE(_min_stack_size = 0x400) ;

/*************************************************************************
 * Legacy processor-specific object file.  Contains SFR definitions.
 * The SFR definitions are now provided in a processor-specific *.S
 * assembly source file rather than the processor.o file. Use the new
 * .S file rather than this processor.o file for new projects. MPLAB XC32
 * v2.10 and later will automatically link the new .S file. When using * this linker script with an older MPLAB XC32 version, remove the
 * OPTIONAL() line below and add the pic32mx/lib/proc/<device>.S file
 * to your project.
 *************************************************************************/
OPTIONAL("processor.o")


/*************************************************************************
 * Vector-offset initialization
 *************************************************************************/
OPTIONAL("vector_offset_init.o")

/*************************************************************************
 * Symbols used for interrupt-vector table generation
 * To override the defaults, define the _ebase_address symbol using
 * the --defsym linker opt as shown in this example:
 *   xc32-gcc src.c -Wl,--defsym=_ebase_address=0x9D001000
 *************************************************************************/
PROVIDE(_vector_spacing = 0x0001);
/*PROVIDE(_ebase_address = 0x9FC01000);*/

/*************************************************************************
 * Memory Address Equates
 * _RESET_ADDR                    -- Reset Vector or entry point
 * _BEV_EXCPT_ADDR                -- Boot exception Vector
 * _DBG_EXCPT_ADDR                -- In-circuit Debugging Exception Vector
 * _SIMPLE_TLB_REFILL_EXCPT_ADDR  -- Simple TLB-Refill Exception Vector
 * _CACHE_ERR_EXCPT_ADDR          -- Cache-error Exception Vector
 * _GEN_EXCPT_ADDR                -- General Exception Vector
 *************************************************************************/
_RESET_ADDR                    = 0xBFC00000;
_BEV_EXCPT_ADDR                = 0xBFC00380;
_DBG_EXCPT_ADDR                = 0xBFC00480;
_SIMPLE_TLB_REFILL_EXCPT_ADDR  = _ebase_address + 0;
_CACHE_ERR_EXCPT_ADDR          = _ebase_address + 0x100;
_GEN_EXCPT_ADDR                = _ebase_address + 0x180;

/*************************************************************************
 * Memory Regions
 *
 * Memory regions without attributes cannot be used for orphaned sections.
 * Only sections specifically assigned to these regions can be allocated
 * into these regions.
 *
 * The Debug exception vector is located at 0x9FC00480.
 *
 * The config_<address> sections are used to locate the config words at
 * their absolute addresses.
 *************************************************************************/


MEMORY
{
  /* Interrupt vector table */
  exception_mem               : ORIGIN = 0x9FC00000, LENGTH = 0x1000

  /* Bootloader C program is in the Boot_Flash */
  /* adjust length, as the boot program grows. (0x80000 for now) */
  kseg0_program_mem     (rx)  : ORIGIN = 0x9FC01000, LENGTH = 128K
  /*shared_lib_mem (rx)         : ORIGIN = 0x9FC01000 + 16K, LENGTH = 0x4000 /* Shared between bootloader and app */
/*kseg1_program_mem      (rx)  : ORIGIN = 0x9D000000, LENGTH = 16K*/
  user_app              (rx)  : ORIGIN = 0x9D002000, LENGTH = 512K

  /* first half of boot flash */
  kseg0_boot_mem              : ORIGIN = 0x9FC004b0, LENGTH = 0x0
  /* other half of boot flash <*/
  kseg1_boot_mem              : ORIGIN = 0xBFC00000, LENGTH = 0x480
  kseg1_boot_mem_4B0 : ORIGIN = 0xBFC004B0, LENGTH = (0x1000 - 0x4B0)
  /*kseg1_boot_mem              : ORIGIN = 0xBFC00000, LENGTH = 0x480
  kseg1_boot_mem_4B0          : ORIGIN = 0xBFC004B0, LENGTH = 0xFA50*/
  framebuffer_mem        (w!x)  : ORIGIN = 0x80000000, LENGTH = 150K        /* 150K -> 153600 -> 0x25800 */
  /*fifo_queue_mem         (w!x)  : ORIGIN = 0x80000000 + 150K, LENGTH = 40   /* 2 bytes for counters, 38 bytes for 2 x fifo queues */
  kseg0_data_mem       (w!x)  : ORIGIN = 0x80000000 + 150K + 40, LENGTH = 16K
  /*kseg1_data_mem       (w!x)  : ORIGIN = 0xA0000000 + 200K, LENGTH = 300K - 200K /*0x25800*/
  /*kseg1_data_mem       (w!x)  : ORIGIN = 0xA0000000 + 150K + 4, LENGTH = 0x80000 - 150K*/
  /*kseg0_data_mem       (w!x)  : ORIGIN = 0x80000000 + 150K + 32K, LENGTH = 0x80000 - 150K - 32K*/
}

SECTIONS 
{
  /* LCD */
  framebuffer = ORIGIN(framebuffer_mem);

  /* FIFO queue */
  /*readCounterUA = ORIGIN(fifo_queue_mem);       /* 1 byte */
  /*writeCounterUA = ORIGIN(fifo_queue_mem) + 1;  /* 1 byte */
  /*queueUA = ORIGIN(fifo_queue_mem) + 2;         /* 19 bytes */
  /*queueBL = ORIGIN(fifo_queue_mem) + 21;        /* 19 bytes */

  /*.framebuffer_section ORIGIN(framebuffer_mem) (NOLOAD) :
  {
    . = ALIGN(4);
    KEEP(*(framebuffer))
    /*KEEP(*ILI9341Device.o(.rodata))*/
    /*KEEP(*ILI9341Device.o(.text .text*))
    . = ALIGN(4);
  } >shared_lib_mem*/

  /*.shared_text ORIGIN(shared_lib_mem):
  {
    . = ALIGN(4);
    /*KEEP(*ILI9341Device.o(.rodata))*/
    /*KEEP(*ILI9341Device.o(.text .text*))
    . = ALIGN(4);
  } >shared_lib_mem*/

  /*.user_app ORIGIN(user_app) : 
  {
    __USER_APP_ADDRESS = .;
    KEEP(*(.user_app*))
  } >user_app*/
}

INCLUDE "../Common/procdefs_common.ld"
