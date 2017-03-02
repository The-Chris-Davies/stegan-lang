6: If Statement
***************
Description
===========
Compares two variables, and sets the direction of the cursor to one of two values, depending on the result.

Arguments
=========
Internal
--------
- **Directions**: The sum of the directions. 

===== ============== ===============
Dir   if var1 > var2 if var2 >= var1
===== ============== ===============
up    16             1
down  32             2
left  64             4
right 128            8
===== ============== ===============

External
--------
- **var1**: A :doc:`variable reference <9var>` that represents the first variable to compare.
- **var2**: A :doc:`variable reference <9var>` that represents the second variable to compare.

Returns
=======
- **None**