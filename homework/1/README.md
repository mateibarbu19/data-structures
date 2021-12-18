# Homework 1 - Music Playlist

Description: <https://ocw.cs.pub.ro/courses/sd-ca/teme/tema1-2020>.

The solution is based on Laboratory 3. I created a DoublyLinkedList library
based on the lab model.

The player is thus simulated with a double chained list. I changed
the list structure so that it also contains the position on which the cursor is.
Although it would have been better to create a different data structure
from a cursor and a double chained list, I thought the overhead was too
big.

In the main.c file I have created a command interpreter, which will include the
main the functions in Player.h.

The cursor, being of type int and not pointer, is moved each time if it is
way.

The functions in Player.h are independent and can be called individually from
interpreter. They respect the principle of structured programming.

I've always used dynamic memory allocation to create data
in a function and they should not be deleted after the assignment of the
function on hold.

The FIRST, LAST, CURR modes have been encoded with numbers to be able to be
transmitted more easily as a parameter to the functions

- player_add
- player_del
- player_move
- player_show

With an extra SPECIAL mode depending on the function.

The list is made using generic pointers, so the solution is adaptable.
how to define the metadata structure and its display functions.
