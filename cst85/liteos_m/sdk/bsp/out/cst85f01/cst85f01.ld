/* Linker script for cs8800 */

/* Linker script to configure memory regions. */
MEMORY
{
    IRAM (rwx)      : ORIGIN = 0x08000000, LENGTH = 896K
    DRAM (rwx)      : ORIGIN = 0x00100000, LENGTH = 384K
    LPIRAM (rwx)    : ORIGIN = 0x001A0000, LENGTH = 0x100
    ipc_shared_memory (!rx) : ORIGIN = 0x00190000, len = 2K
    host_rxbuf_memory (!rx) : ORIGIN = 0x00198000, len = 32K
    SHARED_HOST (w)   : ORIGIN = 0x001E5000, LENGTH = 2K
    PRIVATE_HOST (w)  : ORIGIN = 0x001E5800, LENGTH = 2K
    PRIVATE_PATCH (w) : ORIGIN = 0x001E6000, LENGTH = 8K
}

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 */
ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        /*Chipsea add _stext begin*/
        _stext = .;
        /*Chipsea add _stext end*/
        __code_start__ = .;
        /* the address 0 must contain the boot vectors */
        KEEP(*(.isr_vector))
        __isr_vector_end__ = .;
        *boot_startup.o(.text)
        *(.text*)

        *(.rodata*)
        __code_end__ = .;
        /*Chipsea add _etext begin*/
        _etext = .;
        /*Chipsea add _etext end*/
    } > IRAM
    .zInit (.) :
     {
        __zinitcall_bsp_start = .;
        KEEP (*(.zinitcall.bsp0.init))
        KEEP (*(.zinitcall.bsp1.init))
        KEEP (*(.zinitcall.bsp2.init))
        KEEP (*(.zinitcall.bsp3.init))
        KEEP (*(.zinitcall.bsp4.init))
        __zinitcall_bsp_end = .;
        . = ALIGN(4);
        __zinitcall_device_start = .;
        KEEP (*(.zinitcall.device0.init))
        KEEP (*(.zinitcall.device1.init))
        KEEP (*(.zinitcall.device2.init))
        KEEP (*(.zinitcall.device3.init))
        KEEP (*(.zinitcall.device4.init))
        __zinitcall_device_end = .;
        . = ALIGN(4);
        __zinitcall_core_start = .;
        KEEP (*(.zinitcall.core0.init))
        KEEP (*(.zinitcall.core1.init))
        KEEP (*(.zinitcall.core2.init))
        KEEP (*(.zinitcall.core3.init))
        KEEP (*(.zinitcall.core4.init))
        __zinitcall_core_end = .;
        . = ALIGN(4);
        __zinitcall_sys_service_start = .;
        KEEP (*(.zinitcall.sys.service0.init))
        KEEP (*(.zinitcall.sys.service1.init))
        KEEP (*(.zinitcall.sys.service2.init))
        KEEP (*(.zinitcall.sys.service3.init))
        KEEP (*(.zinitcall.sys.service4.init))
        __zinitcall_sys_service_end = .;
        . = ALIGN(4);
        __zinitcall_sys_feature_start = .;
        KEEP (*(.zinitcall.sys.feature0.init))
        KEEP (*(.zinitcall.sys.feature1.init))
        KEEP (*(.zinitcall.sys.feature2.init))
        KEEP (*(.zinitcall.sys.feature3.init))
        KEEP (*(.zinitcall.sys.feature4.init))
        __zinitcall_sys_feature_end = .;
        . = ALIGN(4);
        __zinitcall_run_start = .;
        KEEP (*(.zinitcall.run0.init))
        KEEP (*(.zinitcall.run1.init))
        KEEP (*(.zinitcall.run2.init))
        KEEP (*(.zinitcall.run3.init))
        KEEP (*(.zinitcall.run4.init))
        __zinitcall_run_end = .;
        . = ALIGN(4);
        __zinitcall_app_service_start = .;
        KEEP (*(.zinitcall.app.service0.init))
        KEEP (*(.zinitcall.app.service1.init))
        KEEP (*(.zinitcall.app.service2.init))
        KEEP (*(.zinitcall.app.service3.init))
        KEEP (*(.zinitcall.app.service4.init))
        __zinitcall_app_service_end = .;
        . = ALIGN(4);
        __zinitcall_app_feature_start = .;
        KEEP (*(.zinitcall.app.feature0.init))
        KEEP (*(.zinitcall.app.feature1.init))
        KEEP (*(.zinitcall.app.feature2.init))
        KEEP (*(.zinitcall.app.feature3.init))
        KEEP (*(.zinitcall.app.feature4.init))
        __zinitcall_app_feature_end = .;
        . = ALIGN(4);
        __zinitcall_test_start = .;
        KEEP (*(.zinitcall.test0.init))
        KEEP (*(.zinitcall.test1.init))
        KEEP (*(.zinitcall.test2.init))
        KEEP (*(.zinitcall.test3.init))
        KEEP (*(.zinitcall.test4.init))
        __zinitcall_test_end = .;
        . = ALIGN(4);
        __zinitcall_exit_start = .;
        KEEP (*(.zinitcall.exit0.init))
        KEEP (*(.zinitcall.exit1.init))
        KEEP (*(.zinitcall.exit2.init))
        KEEP (*(.zinitcall.exit3.init))
        KEEP (*(.zinitcall.exit4.init))
        __zinitcall_exit_end = .;
        . = ALIGN(4);
    } > IRAM

    .ARM.exidx :
    {
       __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
       __exidx_end = .;
    } > IRAM

    __etext = .;

    .vectors (ORIGIN(DRAM)) (NOLOAD):
    {
        __vectors_start__ = .;
        . = (__isr_vector_end__ - __code_start__);
        __vectors_end__ = .;
    } > DRAM

    .data : AT (__etext)
    {
        __data_start__ = .;
        Image$$RW_IRAM1$$Base = .;
        *(vtable)
        *(.data*)

        . = ALIGN(4);
        /*Chipsea add hdf_drivers begin*/
		_hdf_drivers_start = .;
        KEEP(*(.hdf.driver))
        _hdf_drivers_end = .;
        /*Chipsea add hdf_drivers end*/
        /* All data end */
        __data_end__ = .;

    } > DRAM

    __image_end__ = __etext + SIZEOF(.data);

    .bss (NOLOAD):
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
        *(SHAREDRAM)
        Image$$RW_IRAM1$$ZI$$Limit = . ;
    } > DRAM

    .heap :
    {
        __end__ = .;
        /*Chipsea add end/_end/__end begin*/
		end = .;
		_end = end;
        __end = end;
        /*Chipsea add end/_end/__end end*/
        *(.heap*)
        __HeapLimit = .;
    } > DRAM
    /*Chipsea add __HEAP_START begin*/
    PROVIDE(__HEAP_START = ADDR(.heap));
    /*Chipsea add __HEAP_START end*/
    PROVIDE(__sbrk_start = ADDR(.heap));
    PROVIDE(__krbs_start = ADDR(.heap) + SIZEOF(.heap));

    /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy :
    {
        *(.stack)
    } > DRAM

    /* Set stack top to end of IRAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(DRAM) + LENGTH(DRAM);
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);
    /*Chipsea add _sstack/_estack begin*/
    _sstack = __StackLimit;
    _estack = __stack;
    /*Chipsea add _sstack/_estack end*/

    /* Check if data + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region DRAM overflowed with stack")

    /* IPC shared RAM */
    IPC_SHARED (NOLOAD):
    {
        *(SHAREDRAMIPC)
    } > ipc_shared_memory

    HOST_RXBUF (NOLOAD):
    {
        _shrbram = . ;
        *(HOST_RXBUF)
        _ehrbram = . ;
    } > host_rxbuf_memory

    .shared_host (NOLOAD):
    {
        _start_shared_host = .;
        *(SORT_BY_NAME(SHAREDMEM_HOST_*))
        _end_shared_host = .;
    } > SHARED_HOST

    .private_host (NOLOAD):
    {
        _start_private_host = .;
        KEEP(*(SORT_BY_NAME(PRIVATEMEM_HOST_*)))
        _end_private_host = .;
    } > PRIVATE_HOST
}
