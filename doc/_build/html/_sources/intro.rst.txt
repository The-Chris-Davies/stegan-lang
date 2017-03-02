Introduction to Stegan
**********************

What is Stegan?
===============
Stegan is a programming language that can be stored in any lossless 4-channel image. It takes 3 least significant bits of every pixel to store its code.

How do I start using Stegan?
======================================
Stegan comes with an editor written in `Python 2.7 <https://www.python.org/>`_, and uses the `pyFLTK module <http://pyfltk.sourceforge.net/>`_ for its GUI. Both of these must be installed for it to work.

How does Stegan work?
=====================
Stegan navigates an image using a 'cursor', reading the data under the cursor, performing the data's function, then moving in a direction, specified by using the :doc:`change direction <cmds/3cd>` command.

A link to the documentation of all the available commands can be found :doc:`here <syntax>`, and a link to several examples can be found :doc:`here <examples>`.

.. warning::
   
   Stegan takes 3 least significant bits of every pixel to store its code. This means that, while relatively unnoticeable in busy images, artifacts appear in more simple pictures.


Why would I use Stegan?
=======================
There's not really any reason to use Stegan, other than for laughs. Sorry.

.. warning::
   
   Never, ever use this language!