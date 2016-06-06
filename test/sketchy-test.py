from blackstripes import sketchy

sketchy.draw("../test/ali-medium.png",   # input
             "ali_sketchy.svg",          # output
             1,                          # nibsize (line size in output svg)
             100,                        # max line length
             "#000000",                  # line color
             1.0,                        # scaling factor
             1,                          # line size (internal line size for calculations)
             540,1021,0.7                # signature transform tx, ty, scale
            )
