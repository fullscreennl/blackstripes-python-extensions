from distutils.core import setup, Extension

module1 = Extension('blackstripes.sketchy', sources = 
	['blackstripes/sketchy.c',
	 'lib/lodepng/lodepng.c',
	 'lib/sketchy/SketchyImage.c',
	 'lib/sketchy/FSObject.c',
	 'lib/sketchy/Point.c',
	 'lib/sketchy/FSArray.c',
	 'lib/sketchy/FSNumber.c'])

module2 = Extension('blackstripes.crossed', sources = 
	['blackstripes/crossed.c',
	 'lib/lodepng/lodepng.c',
	 'lib/sketchy/SketchyImage.c',
	 'lib/sketchy/FSObject.c',
	 'lib/sketchy/Point.c',
	 'lib/sketchy/FSArray.c',
	 'lib/sketchy/FSNumber.c'])

module3 = Extension('blackstripes.spiral', sources = 
	['blackstripes/spiral.c',
	 'lib/lodepng/lodepng.c',
	 'lib/sketchy/SketchyImage.c',
	 'lib/sketchy/FSObject.c',
	 'lib/sketchy/Point.c',
	 'lib/sketchy/FSArray.c',
	 'lib/sketchy/FSNumber.c'])

setup (name = 'blackstripes',
       version = '1.0',
       description = 'This is the blackstripes package',
       ext_modules = [module1, module2, module3],
       packages=['blackstripes'],
       package_dir={'blackstripes': 'blackstripes'},
       )
