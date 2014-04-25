import sys

wgt1 = [6,6]
wgt2 = [6,6]

def dele1(w1, w2):
    return 2.0*w1+w2/2.0

def dele2(w1,w2):
    return 2.0*w1+2.0*w2

for i in range(5):
    err1 = wgt1[0]*wgt1[0] + wgt1[1]*wgt1[1]/4.0
    err2 = wgt2[0]*wgt2[0] + wgt2[1]*wgt2[1]

    wgt1[0] = wgt1[0] - 0.1 * dele1(wgt1[0], wgt1[1]) 
    wgt1[1] = wgt1[1] - 0.1 * dele1(wgt1[0], wgt1[1]) 
    wgt2[0] = wgt2[0] - 0.1 * dele2(wgt2[0], wgt2[1]) 
    wgt2[1] = wgt2[1] - 0.1 * dele2(wgt2[0], wgt2[1]) 

    print err1, err2, wgt1, wgt2



