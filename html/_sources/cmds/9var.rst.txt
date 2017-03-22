9: Define a Variable
********************
Description
===========
Defines a variable.

Arguments
=========
Internal
--------
- **Length/type of variable to store**: The length of the variable to store. **Set to 0 to initialize an int.**

External
--------
- **Var**: A :doc:`variable reference <11vr>` object that points to the object to initialize.

- **Var**: A :doc:`variable reference <11vr>` object that points to a variable to set the initialized value to.

***OR***

- **Value**: A :doc:`constant value <10cv>` holding one byte of data to store to the variable.

.. warning::
   
   An additional ``Value`` pixel is required for every length of the variable.
   8 ``Value`` pixels are required to initialize an int.

Returns
=======
- **None**
