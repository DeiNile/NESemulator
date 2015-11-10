This is a side project where I will attempt to implement an NES emulator from scratch. This has the additional benefit of me learning some C++.

UPDATE OCT. 23, 2015

So, progress has been slow but steady. At this point I am starting to test the CPU thoroughly. You could argue that I should have implemented the CPU's memory more solidly. Admittedly I have been putting it off because I am not exactly sure how the memory mapping works quite yet. Once the CPU has been tested and I am sure that it works reliably I will probably implement memory. 

UPDATE NOV. 09, 2015

Finally my CPU can handle the Nestest, with the exception of 'undocumented' instructions. It seems these instructions are rarely, if ever, used by games released for the NES, so I have decided to not implement these instructions at the moment. As for which thing to do next, I am kind of leaning towards the PPU. For simple ROMs there is no need for any complex memory mappers (I think), so doing the PPU would be a logical next step.