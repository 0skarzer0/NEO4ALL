                     _  __ ____ ____   ____  ___    __    __ 
                    /#|/#//___//####\ / / / / _ |  / /   / / 
                   /####//#_/ /#/_/#//_  _// __ | / /__ / /__
                  /#/|#//___/ \####/  /_/ /_/ |_|/____//____/
                                                             
              Official site: http://chui.dcemu.co.uk/neo4all.shtml

---------------------------------------------------------------------------
NEO4ALL Beta 2 (04/10/2005)                              Chui & Fox68k 2005
---------------------------------------------------------------------------


Neo4all is a NeoGeo CD emulator for the Dreamcast videogame system.

This is a branch emulation development from the Fosters' NeoCD/SDL which is
a port of Fabrice Martinez' NeoCD emulator to the Dreamcast. Some parts of
GnGeo project were used in Neo4all development.


What's New ?
------------

Well, this time we wanted to release a much better version, without graphic
glitches, good game compatibility, memory card support and better sound and
speed.

* Many speed ups :)
* Almost every game supported.
* Emulation speed boosted.
* Improved sound core.
* CDDA fixed in many games.
* Z80 core changed to MAMEZ80.
* Scanline flickering removed.


Features
--------

* Motorola 68000 CPU core:
  - FAME by Fox68k (http://perso.wanadoo.es/orayo/fame/).
* MAME Z80 CPU core:
  - Z80 CPU core used in MAME project (http://www.mame.net/).
* Graphic NeoGeo CD core:
  - Custom PowerVR2 engine by Chui using code from GnGeo project.
* Two players.
* Sound FX.
* CDDA sound.
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

Button A <-> Button A        / Left Ctrl
Button B <-> Button B        / Left Alt
Button C <-> Button X        / Left Shift
Button D <-> Button Y        / Space
Start    <-> Start           / Enter
Select   <-> Right Trigger   / Tab

Main Menu:   Left Trigger    / Backspace


Memory card
-----------

In order to use NeoGeo CD game saving, you will need a Dreamcast VMU 
connected to the port A1.
You will need 18 memory blocks free on your VMU.
Game saving is done automatically during gameplay.


Burning instructions
--------------------

You will find instructions on how to use Selfboot inducer to make a bootable 
disk for the Dreamcast at http://dcevolution.net/DCHelp/Sbindv4_Tut.shtml.


You must include a NeoGeo CD BIOS image file (named neocd.bin as the 
emulator expects this name) in the root directory of the disk.


Credits
-------

* GnGeo developer team for GnGeo source code.

* NeoGeoCD/SDL developer team for NeoGeoCD/SDL source code.

* DarkSlain for Neo4all artwork/beta testing.

* Stephàne Dallongueville for his Z80 core (CZ80) used in Neo4all until 
  beta 1.

* Unai for giving us some classes about Dreamcast PVR hardware ;)

---------------------------------------------------------------------------

That is all for now folks, have fun!

Chui & Fox68k