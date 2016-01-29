@echo off
@Title Generating .srec file

Echo Generating .srec file :

..\..\..\..\fnet_tools\fromelf.exe --m32 --output Flash\shell_boot_intflash.elf.S19 Flash\shell_boot_intflash.axf

Echo Generating "Flash\shell_boot_intflash.elf.S19" Finished. 

