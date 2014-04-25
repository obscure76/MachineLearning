import sys
import math
import pydot
import pygraphviz as PG

A = PG.AGraph(directed=True, strict=True)
list_of_data = []
list_of_attr = ['serial', 'Manufacturer', 'Power', 'AI', 'Decision']
list_of_used_attr = []
value_of_attr = {}
list_of_nodes = []
count = 0
mydot = open('mydot', 'w')
lcount = 0

class DTNode:
    def __init__(self, data):
        self.data = data
        self.children = {}
        self.attr = ''
    def addChild(self, value, child):
        self.children[value] = child
    def printData(self):
        print 'myData'
        print self.data
    def setattr(self, attr):
        self.attr = attr
    def printChildren(self):
        print 'Number of children for', self.attr, len(self.children)
        print self, self.children
    def printTree(self):
        print self.attr
        for value, child in self.children.items():
            if(len(child.children)):
                child.printTree()
    def drawacgraph(self, parentcount):
        global lcount
        global mydot
        for value, child in self.children.items():
            if(value == 'ML-based'):
                value = 'MLbased'
            if len(child.data) == 0:
                continue
            # add children to the graph
            if len(child.attr):
                lcount += 1
                text = 'attr' + str(lcount) + '[shape="rectangle", label="' + child.attr + '"]\n'
                mydot.write(text)
                edgetext = 'attr' + str(parentcount) + ' -> attr' + str(lcount) + '[label="' + value + '"]\n'
                mydot.write(edgetext)
                child.drawacgraph(lcount)
            else:
                each = child.data[0]
                if each['Decision'] == 'Y':
                    lcount += 1
                    text = 'attr' + str(lcount) + '[shape="plaintext", label="Y"]\n'
                    mydot.write(text)
                    edgetext = 'attr' + str(parentcount) + ' -> attr' + str(lcount) + '[label="' + value + '"]\n'
                    mydot.write(edgetext)
                else:
                    lcount += 1
                    text = 'attr' + str(lcount) + '[shape="plaintext", label="N"]\n'
                    mydot.write(text)
                    edgetext = 'attr' + str(parentcount) + ' -> attr' + str(lcount) + '[label="' + value + '"]\n'
                    mydot.write(edgetext)

def distribute_dataset(attr, data):
   try:
       values = value_of_attr[attr]
   except:
       print attr
       return []
   distriData = {}
   for value in values:
       currData = []
       for edata in data:
           if(edata[attr] == value):
               currData.append(edata)
       distriData[value] = currData
   return distriData


def walk_and_form_data(filename):
    fp = open(filename, 'r')
    for attr in list_of_attr:
        value_of_attr[attr] = set()
    for line in fp:
        if(line == '\n'):
            continue
        curr_data = {}
        instance = line.strip().split(',')
        for index in range(len(instance)):
            curr_data[list_of_attr[index]] = instance[index]
            value_of_attr[list_of_attr[index]].add(instance[index])
        list_of_data.append(curr_data)

def calc_entropy(data):
    total_data_points = 0.0
    total_yes = 0.0
    total_no = 0.0
    for edata in data:
        total_data_points = total_data_points+1
        if(edata['Decision'] == 'Y'):
            total_yes = total_yes+1
        else:
            total_no = total_no+1
    pyes = total_yes/total_data_points
    pno = total_no/total_data_points
    if(pyes):
        ppyes = math.log10(pyes)/math.log10(2)
    else:
        ppyes = 0.0
    if(pno):
        ppno = math.log10(pno)/math.log10(2)
    else:
        ppno = 0.0
    return (-1 * pyes *ppyes - pno * ppno)

def calc_attr_entropy(attr):
    Eattr = 0.0
    for value in value_of_attr[attr]:
        total_data_points = 0.0
        curr_data_points = 0.0
        total_yes = 0.0
        total_no = 0.0
        for edata in list_of_data:
            total_data_points=total_data_points+1
            if(not(edata[attr]==value)):
                continue
            curr_data_points = curr_data_points+1
            if(edata['Decision'] == 'Y'):
                total_yes = total_yes+1
            else:
                total_no = total_no+1
        pyes = total_yes/curr_data_points
        pno = total_no/curr_data_points 
        try:
            ppyes = math.log10(pyes)/math.log10(2)
        except:
            ppyes = 0.0
        try:
            ppno = math.log10(pno)/math.log10(2)
        except:
            ppno = 0.0
        Eattr = Eattr + (curr_data_points/total_data_points)*(-1 * pyes * ppyes - pno * ppno)
    return Eattr
    
def calc_best_attr(data, attrList):
    max_gain = 0
    max_gain_attr = ''
    entropy = calc_entropy(data)
    for attr in attrList:
        if(attr == 'serial' or attr == 'Decision'):
            continue
        sum = 0.0
        if(not(max_gain_attr)):
            max_gain_attr = attr
            gain = entropy - calc_attr_entropy(attr)
        else:
            gain = entropy - calc_attr_entropy(attr)
        if(gain>max_gain):
           max_gain = gain
           max_gain_attr = attr
    return max_gain_attr

def isDataClassified(data):
    if(not(len(data))):
        return True
    county = 0
    countn = 0
    for each in data:
        if(each['Decision'] == 'Y'):
            county = county + 1
        else:
            countn = countn + 1
    if(county and countn):
        return False
    else:
        return True

def createDtree(myroot, initData, attrList):
    best_attr = calc_best_attr(initData, attrList)
    if(best_attr == ''):
        return
    myroot.setattr(best_attr)
    dividedData = distribute_dataset(best_attr, initData)
    remAttr = []
    for each in attrList:
        remAttr.append(each)
    remAttr.remove(best_attr)
    count = 0
    for value in value_of_attr[best_attr]:
        count = count+1
        if(isDataClassified(dividedData[value]) or len(remAttr) ==0):
            child = DTNode(dividedData[value])
            myroot.addChild(value, child)
        else:
            child = DTNode(dividedData[value])
            myroot.addChild(value, child)
            createDtree(child, dividedData[value], remAttr)

def main(argv):
    global mydot
    global lcount
    if(len(sys.argv)==0):
        print 'Usage: python decision-tree.py data'
        sys.exit(-1)
    root = DTNode(list_of_data)
    walk_and_form_data(argv[1])
    if(isDataClassified(list_of_data)):
        print 'Already Classified'
        return
    createDtree(root, list_of_data, list_of_attr)
    lcount += 1
    mydot.write('digraph G\n')
    mydot.write('{\n')
    text = 'attr' + str(lcount) + '[shape="rectangle", label=' + root.attr + ']\n'
    mydot.write(text)
    root.drawacgraph(1)
    mydot.write('\n}')

if(__name__=='__main__'):
    sys.exit(main(sys.argv))
