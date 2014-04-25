import sys
import matplotlib.pyplot as plt

xvar = []
errvar = []

def getvalue(w):
    return (4.0*w*w*w-57.0*w*w+250.0*w-317.0)

def main(argv):
    x = 0.01
    while(x<7.0):
        xvar.append(x)
        errvar.append(getvalue(x))
        x = x + .01


if(__name__ == '__main__'):
    sys.exit(main(sys.argv))


