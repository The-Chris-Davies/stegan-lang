Syntax
******

Pixel Format
============
Stegan stores data in the 3 least significant bits of a pixel. This means that, in total, it stores 12 bits of data per pixel.

Internally, the data is seen as a nibble and a byte, with the format:

+-----+-----+-----+-------+
|R    |G    |B    |A      |
+-+-+-+-+-+-+-+-+-+-+--+--+
|0|1|2|3|4|5|6|7|8|9|10|11|
+-+-+-+-+-+-+-+-+-+-+--+--+
|nibble |byte             |
+-------+-----------------+

| The nibble is used to store which function is being run, and the byte holds arguments for the function.
| Additionally, some functions will require additional data, and will read the next pixel's data if it is a :doc:`constant value <cmds/10cv>` or a :doc:`variable reference <cmds/9var>`.


Pixel Reference
===============
All the functions available to call.

.. toctree::
   :maxdepth: 1
   :glob:

   cmds/*
