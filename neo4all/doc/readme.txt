                     _  __ ____ ____   ____  ___    __    __ 
                    /#|/#//___//####\ / / / / _ |  / /   / / 
                   /####//#_/ /#/_/#//_  _// __ | / /__ / /__
                  /#/|#//___/ \####/  /_/ /_/ |_|/____//____/
                                                             
              Official site: http://chui.dcemu.co.uk/neo4all.html


---------------------------------------------------------------------------
NEO4ALL Beta 3 (10/10/2005)                              Chui & Fox68k 2005
---------------------------------------------------------------------------



Neo4all is a NeoGeo CD emulator for the Dreamcast videogame system.

This is a branch emulation development from the Fosters' NeoCD/SDL which is
a port of Fabrice Martinez' NeoCD emulator to the Dreamcast. Some parts of
GnGeo project were used in Neo4all development.



History
-------

- Beta 3 (10/10/2005)

    * Neo Turf Masters game fixed.
    * CDDA track change will not stall gameplay anymore.
    * New auto-frameskip method, much exact.
    * Load/save from/to VMU will not stall gameplay anymore.
    * Sound render improvements.
    * New loading screen.
    * Compiled with new Chui's SDL driver.
    * More accurate 68000 CPU timming.
    * General improvements.


- Beta 2 (04/11/2005)

    * Many speed ups :)
    * Almost every game supported.
    * Emulation speed boosted.
    * Improved sound core.
    * CDDA fixed in many games.
    * Z80 core changed to MAMEZ80.
    * Scanline flickering removed.


- Beta 1 (02/27/2005)

    * Incorrect tiles bug fixed.
    * Bilinear filter option added (slow with sound).
    * Reset option bug fixed.
    * Sound output improved.
    * FAME and PVR-Blitter speed up.
    * Memcard support added.


- Alpha 2 (02/14/2005)

    * New 640x480 video resolution (respect original ratio).
    * FAME 68000 core fixed (a lot of games runs).
    * Crystalline sound with no lag.
    * White tiles bug fixed.
    * Keyboard input bug fixed.


- Alpha 1 (02/07/2005)

    * First public release.


Features
--------

* Main CPU Motorola 68000:
      - FAME emulation library
        by Fox68k (http://www.m68k.com/fame/).

* Sound CPU Zilog Z80:
      - Z80 CPU emulation core
        from MAME project (http://www.mame.net/).

* NeoGeo CD Video Display Processor:
      - Custom PowerVR2 based engine
        by Chui (sdl_gp32@yahoo.es).
      - Bilinear filtering.
      - Tile caching.
      - SQ data transfer.

* YM2610 FM sound chip.

* IO Ports (controller 1 & 2).

* CDDA playback sound.

* Memory card support.


Main menu
---------

- Region: Allows you to choose game region.

- Frameskip: You can adjust frame skipping with this option.

- Sound: Turns sound output On and Off.

- 68K CLK: Intended to take control of 68000 CPU clock. Overclocking could
           fix some game slowdowns. Normal CLK is recommended.

- Z80 CLK: Intented to adjust Z80 CPU clock. Underclocking can result in a
           general speed increase, but could broke sound output. Under CLK
           is recommended.

- Filter: Graphic filtering. Bilinear filtering produces a smoother image, 
          but could decrease emulator speed dramatically.

- Reset: Hardware NeoGeo CD reset.

- Run: Runs the NeoGeo CD game.

- Exit: Exits the emulator and resets Dreamcast.


Controllers
-----------

NeoGeo CD     Dreamcast Pad    Dreamcast keyboard

Button A <->    Button A         / Left Ctrl
Button B <->    Button B         / Left Alt
Button C <->    Button X         / Left Shift
Button D <->    Button Y         / Space
Start    <->     Start           / Enter
Select   <->  Right Trigger      / Tab

Main Menu:    Left Trigger       / Backspace


Memory card
-----------

In order to use NeoGeo CD game saving, you will need a Dreamcast VMU 
connected to the port A1.
You will need 18 free memory blocks on your VMU.
Game saving is done automatically during gameplay.


Burning instructions
--------------------

You will find instructions on how to use Selfboot inducer to make a bootable 
disk for the Dreamcast at http://dcevolution.net/DCHelp/Sbindv4_Tut.shtml.


You must include a NeoGeo CD BIOS image file (named neocd.bin as the 
emulator expects this name) in the root directory of the disk.


Credits
-------

* NeoGeoCD/SDL developer team for NeoGeoCD/SDL source code.

* GnGeo developer team for GnGeo source code.

* DarkSlain for Neo4all artwork/beta testing.

* Stephàne Dallongeville for his Z80 core (CZ80) used in Neo4all until 
  beta 1.

* Unai for giving us some classes about Dreamcast PVR hardware ;)

---------------------------------------------------------------------------

That is all for now folks, have fun!

Chui & Fox68k
ÿ
