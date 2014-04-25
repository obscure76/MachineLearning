import sys
import matplotlib.pyplot as plt

pxvar = []
pyvar = []
nxvar = []
nyvar = []

def parse(dfile):
    fp = open(dfile, 'r')
    for line in fp:
        if(line == '\n'):
            continue
        words = line.split()
        if(len(words)<3):
            continue
        chars = list(words[2])
        if(chars[0] == '+'):
            pyvar.append(int (words[1]))
            pxvar.append(int (words[0]))
        else:
            nyvar.append(int (words[1]))
            nxvar.append(int (words[0]))

def main(argv):
    if(len(sys.argv) == 1):
        print 'Usage L <>.py <data-file>'
        sys.exit(-1)
    parse(sys.argv[1])
    plt.plot(pxvar, pyvar, 'go')
    plt.plot(nxvar, nyvar, 'bo')
    plt.show()

if(__name__ == '__main__'):
    main(sys.argv)
    sys.exit(0)

