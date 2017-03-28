5: String Manipulation
**********************
Description
===========
Various manipulation functions for strings.

Arguments
=========
Internal
--------
- **Function to Perform**: The function type. Set to 1 to get length, 2 to set length, 3 to get a substring, and 4 to set a substring.

External
--------
1: Get Length
^^^^^^^^^^^^^
- **var1**: A :doc:`variable reference <11vr>` object that represents a string to get the length of.
- **var2**: A :doc:`variable reference <11vr>` object that represents an int to be set to the length.

2: Set Length
^^^^^^^^^^^^^
- **var1**: A :doc:`variable reference <11vr>` object that represents a string to set the length of.
- **var2**: A :doc:`variable reference <11vr>` object that represents an int to set the length to.

3: Get Substr
^^^^^^^^^^^^^
- **var1**: A :doc:`variable reference <11vr>` object that represents a string to get a substring from.
- **var2**: A :doc:`variable reference <11vr>` object that represents the index to start reading from.
- **var3**: A :doc:`variable reference <11vr>` object that will be set to the substring. The length of the read substring is the length of this var, or 1 if this is an int.

4: Set Substr
^^^^^^^^^^^^^
- **var1**: A :doc:`variable reference <11vr>` object that represents a string to set the substring to.
- **var2**: A :doc:`variable reference <11vr>` object that represents the index to start writing from.
- **var3**: A :doc:`variable reference <11vr>` object that will be used as the substring. The substring is the length of this var, or 1 if this is an int.

Returns
=======
- **None**
