from blackstripes import crossed
from blackstripes import sketchy
from blackstripes import spiral
from PIL import Image

def levels_by_preview_name(name):
	return (100, 150, 200, 230)

def crop_by_preview_name(name):
	return name

def classic_small(image_path, path, color):

    l1, l2, l3, l4 = levels_by_preview_name(image_path)
    selected_crop = crop_by_preview_name(image_path)

    im = Image.open(selected_crop)
    im = im.resize((500,500), Image.ANTIALIAS)
    im.save(selected_crop)

    crossed.draw(selected_crop,         # input
                path,                   # output
                3,                      # nibsize (line size in output svg)
                color,                  # line color
                1.00,                   # scaling factor
                l1, l2, l3, l4,         # levels
                1,                      # type
                380,500,0.2              # signature transform
            )


def sketchy_small(image_path, path, color):

    im = Image.open(image_path)
    im = im.resize((500,500), Image.ANTIALIAS)
    im.save(image_path)

    sketchy.draw(image_path,            # input
                path,                   # output
                2,                      # nibsize (line size in output svg)
                100,                    # max line length
                color,                  # line color
                1.00,                   # scaling factor
                1,                      # line size (internal line size for calculations)
                380,500,0.2              # signature transform
                )


def spiral_small(image_path, path, color):

    l1, l2, l3, l4 = levels_by_preview_name(image_path)
    selected_crop = crop_by_preview_name(image_path)

    im = Image.open(selected_crop)
    im = im.resize((500,500), Image.ANTIALIAS)
    im.save(selected_crop)

    spiral.draw(selected_crop,             # input
                path,                      # output
                4,                         # nibsize (line size in output svg)
                color,                     # line color
                1.00,                      # scaling factor
                l1, l2, l3, l4,            # levels
                3,                         # line spacing
                380,500,0.2                # signature transform
                )

def classic_large(image_path, path, color):

    l1, l2, l3, l4 = levels_by_preview_name(image_path)
    selected_crop = crop_by_preview_name(image_path)

    im = Image.open(selected_crop)
    im = im.resize((1000,1000), Image.ANTIALIAS)
    im.save(selected_crop)

    crossed.draw(selected_crop,         # input
                path,                   # output
                3,                      # nibsize (line size in output svg)
                color,                  # line color
                1.06,                   # scaling factor
                l1, l2, l3, l4,         # levels
                1,                      # type
                820,975,0.3             # signature transform
            )
    

def sketchy_large(image_path, path, color):

    im = Image.open(image_path)
    im = im.resize((1000,1000), Image.ANTIALIAS)
    im.save(image_path)

    sketchy.draw(image_path,            # input
                path,                   # output
                2,                      # nibsize (line size in output svg)
                100,                    # max line length
                color,                  # line color
                1.06,                   # scaling factor
                1,                      # line size (internal line size for calculations)
                820,975,0.3            # signature transform
                )


def spiral_large(image_path, path, color):

    l1, l2, l3, l4 = levels_by_preview_name(image_path)
    selected_crop = crop_by_preview_name(image_path)

    im = Image.open(selected_crop)
    im = im.resize((1000,1000), Image.ANTIALIAS)
    im.save(selected_crop)

    spiral.draw(selected_crop,             # input
                path,                      # output
                4,                         # nibsize (line size in output svg)
                color,                     # line color
                1.06,                      # scaling factor
                l1, l2, l3, l4,            # levels
                3,                         # line spacing
                820,975,0.3               # signature transform
                )


def classic_extra_large(image_path, path, color):
    l1, l2, l3, l4 = levels_by_preview_name(image_path)
    selected_crop = crop_by_preview_name(image_path)

    im = Image.open(selected_crop)
    im = im.resize((1000,1000), Image.ANTIALIAS)
    im.save(selected_crop)

    sig_scale = 1.0/1.39

    crossed.draw(selected_crop,     # input
            path,                   # output
            3,                      # nibsize (line size in output svg)
            color,                  # line color
            1.39,                   # scaling factor
            l1, l2, l3, l4,         # levels
            2,                      # type
            840, 975, 0.3       # signature transform
            )


def spiral_extra_large(image_path, path, color):
    l1, l2, l3, l4 = levels_by_preview_name(image_path)
    selected_crop = crop_by_preview_name(image_path)

    im = Image.open(selected_crop)
    im = im.resize((1000,1000), Image.ANTIALIAS)
    im.save(selected_crop)

    sig_scale = 1.0/1.39

    spiral.draw(selected_crop,         # input
            path,                      # output
            3,                         # nibsize (line size in output svg)
            color,                     # line color
            1.39,                      # scaling factor
            l1, l2, l3, l4,            # levels
            2,                         # line spacing
            840, 975, 0.3        # signature transform
            )


def sketchy_extra_large(image_path, path, color):

    im = Image.open(image_path)
    im = im.resize((1390,1390), Image.ANTIALIAS)
    im.save(image_path)

    sig_scale = 1.0/1.39

    sketchy.draw(image_path,            # input
                path,                   # output
                2,                      # nibsize (line size in output svg)
                100,                    # max line length
                color,                  # line color
                1.00,                   # scaling factor
                1,                      # line size (internal line size for calculations)
                1225, 1370, 0.3   # signature transform
                )

def classic_pen_a3_portrait(image_path, path, color):
    l1, l2, l3, l4 = levels_by_preview_name(image_path)
    selected_crop = crop_by_preview_name(image_path)

    im = Image.open(selected_crop)
    im = im.resize((668,1000), Image.ANTIALIAS)
    im.save(selected_crop)

    crossed.draw(selected_crop,     # input
            path,                   # output
            2,                      # nibsize (line size in output svg)
            color,                  # line color
            0.32,                   # scaling factor
            l1, l2, l3, l4,         # levels
            2,                      # type
            500,975, 0.3            # signature transform
            )

def classic_pen_a3_landscape(image_path, path, color):
    l1, l2, l3, l4 = levels_by_preview_name(image_path)
    selected_crop = crop_by_preview_name(image_path)

    im = Image.open(selected_crop)
    im = im.resize((1000,572), Image.ANTIALIAS)
    im.save(selected_crop)

    crossed.draw(selected_crop,     # input
            path,                   # output
            2,                      # nibsize (line size in output svg)
            color,                  # line color
            0.34,                   # scaling factor
            l1, l2, l3, l4,         # levels
            2,                      # type
            830, 550, 0.3            # signature transform
            )


def spiral_pen_a3_portrait(image_path, path, color):
    l1, l2, l3, l4 = levels_by_preview_name(image_path)
    selected_crop = crop_by_preview_name(image_path)

    im = Image.open(selected_crop)
    im = im.resize((668,1000), Image.ANTIALIAS)
    im.save(selected_crop)

    spiral.draw(selected_crop,         # input
            path,                      # output
            2,                         # nibsize (line size in output svg)
            color,                     # line color
            0.32,                      # scaling factor
            l1, l2, l3, l4,            # levels
            2,                         # line spacing
            500,975, 0.3               # signature transform
            )

def spiral_pen_a3_landscape(image_path, path, color):
    l1, l2, l3, l4 = levels_by_preview_name(image_path)
    selected_crop = crop_by_preview_name(image_path)

    im = Image.open(selected_crop)
    im = im.resize((1000,572), Image.ANTIALIAS)
    im.save(selected_crop)

    spiral.draw(selected_crop,         # input
            path,                      # output
            2,                         # nibsize (line size in output svg)
            color,                     # line color
            0.34,                      # scaling factor
            l1, l2, l3, l4,            # levels
            2,                         # line spacing
            830, 550, 0.3               # signature transform
            )


def sketchy_pen_a3_portrait(image_path, path, color):

    im = Image.open(image_path)
    im = im.resize((668,1000), Image.ANTIALIAS)
    im.save(image_path)

    sketchy.draw(image_path,            # input
                path,                   # outputsketchy_pen_a3
                2,                      # nibsize (line size in output svg)
                100,                    # max line length
                color,                  # line color
                0.32,                   # scaling factor
                1,                      # line size (internal line size for calculations)
                500,975, 0.3           # signature transform
                )

def sketchy_pen_a3_landscape(image_path, path, color):

    im = Image.open(image_path)
    im = im.resize((1000,572), Image.ANTIALIAS)
    im.save(image_path)

    sketchy.draw(image_path,            # input
                path,                   # outputsketchy_pen_a3
                2,                      # nibsize (line size in output svg)
                100,                    # max line length
                color,                  # line color
                0.34,                   # scaling factor
                1,                      # line size (internal line size for calculations)
                830, 550, 0.3           # signature transform
                )

def classic_pen_a3(image_path, path, color):
    l1, l2, l3, l4 = levels_by_preview_name(image_path)
    selected_crop = crop_by_preview_name(image_path)

    im = Image.open(selected_crop)
    im = im.resize((1000,1000), Image.ANTIALIAS)
    im.save(selected_crop)

    crossed.draw(selected_crop,     # input
            path,                   # output
            3,                      # nibsize (line size in output svg)
            color,                  # line color
            0.21,                   # scaling factor
            l1, l2, l3, l4,         # levels
            1,                      # type 
            730,970,0.5          # signature transform
            )


def spiral_pen_a3(image_path, path, color):
    l1, l2, l3, l4 = levels_by_preview_name(image_path)
    selected_crop = crop_by_preview_name(image_path)

    im = Image.open(selected_crop)
    im = im.resize((1000,1000), Image.ANTIALIAS)
    im.save(selected_crop)

    spiral.draw(selected_crop,         # input
            path,                      # output
            3,                         # nibsize (line size in output svg)
            color,                     # line color
            0.21,                      # scaling factor
            l1, l2, l3, l4,            # levels
            2,                         # line spacing
            730,970,0.5               # signature transform
            )


def sketchy_pen_a3(image_path, path, color):

    im = Image.open(image_path)
    im = im.resize((1000,1000), Image.ANTIALIAS)
    im.save(image_path)

    sketchy.draw(image_path,            # input
                path,                   # output
                3,                      # nibsize (line size in output svg)
                100,                    # max line length
                color,                  # line color
                0.21,                   # scaling factor
                1,                      # line size (internal line size for calculations)
                730, 970, 0.5          # signature transform
                )


sketchy_pen_a3("image.png", "image1.svg", "#ff0000")
spiral_pen_a3("image.png", "image2.svg", "#ff0000")
classic_pen_a3("image.png", "image3.svg", "#ff0000")

sketchy_pen_a3_landscape("image.png", "image4.svg", "#ff0000")
spiral_pen_a3_landscape("image.png", "image5.svg", "#ff0000")
classic_pen_a3_landscape("image.png", "image6.svg", "#ff0000")

sketchy_pen_a3_portrait("image.png", "image7.svg", "#ff0000")
spiral_pen_a3_portrait("image.png", "image8.svg", "#ff0000")
classic_pen_a3_portrait("image.png", "image9.svg", "#ff0000")

sketchy_extra_large("image.png", "image10.svg", "#ff00ff")
spiral_extra_large("image.png", "image11.svg", "#ff00ff")
classic_extra_large("image.png", "image12.svg", "#ff00ff")

sketchy_large("image.png", "image13.svg", "#ff0000")
spiral_large("image.png", "image14.svg", "#ff0000")
classic_large("image.png", "image15.svg", "#ff0000")

sketchy_small("image.png", "image16.svg", "#ffff00")
spiral_small("image.png", "image17.svg", "#ffff00")
classic_small("image.png", "image18.svg", "#ffff00")
