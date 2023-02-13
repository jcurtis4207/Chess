# Chess
## ncurses implementation in C

This is a simple chess game written in C using ncurses for the UI.

The moves are performed by entering pairs of squares. For example `a2 a3{enter}` will move the first white pawn forward one square.

This is the first project I've done using ncurses, and it is by no means perfect. But it was an interesting experience to see what the capabilities are for simple ASCII terminal applications.

This is also the first project I've done using a Makefile, and I will definitely use them often going forward. It makes it very simple to add files to project, to execute tests, and clean up the directory at will.

Unit tests are performed using [acutest](https://github.com/mity/acutest) and can be executed by running `make test`. I looked at several header-only unit test libraries, and this was by far the simplest to use. Again, I will certainly use this going forward.

> **_NOTE_**  It should be noted this does not currently support the concept of "check". It is currently possible to castle through check, castle out of check, move into check, and move while in check. Hopefully in the future I can get this working correctly.
