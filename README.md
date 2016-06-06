#sketchy-python-extension
###Your drawbot will like it!

![example](sketchy-example.png)

This is a python module written in c. It turns an png image into a svg line drawing. The output is very suitable for vsg capable drawbots as the drawings do not require pen lifting.

##How to use

This module exposes one function named sketch. It takes 4 arguments.

	import sketchy
	sketchy.sketch("ali.png","ali_sketch.svg",1 ,90)

1. The name of the input image.
2. The name for the exported SVG
3. The simulated nib or pen size. (pixels)
4. The max length of a line segment in pixels 

	
##How to build

(This module is tested with python 2.7 on mac-osx, but it will probably work on most platforms.)

1. Clone this repo
2. navigate to the src folder
3. `python setup.py build`
4. `python setup.py install`
5. navigate to the test folder
6. `python sketchy-test.py`

##How does it work

1. Calculate the brightness of the inpute image.
2. Place the pen at pixel 0,0.
3. Get a random line length between 10 and the max segment length.
4. Evaluate 360 lines by making a 360 degree sweep with the selected line length.
5. Get the line covering the largest average darkness in the input image.
6. Draw this line white in the input image.
7. Draw this line black in the ouput image.
8. Calculate the brightness of the output image.
9. Repeat from step 2 until the brightness of the output image is equal to the inpute image.

 






