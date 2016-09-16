# -*- coding: utf-8 -*-
import numpy as np
import chainer
from chainer import cuda, Function, FunctionSet, gradient_check, Variable, optimizers, serializers, utils
from chainer import Link, Chain, ChainList
import chainer.functions as F
import chainer.links as L
import sys
import pickle
import random

def predict(p):
    h1 = F.dropout(F.relu(model.l1(Variable(p))), train=False)
    h2 = F.dropout(F.relu(model.l2(h1)), train=False)
    h3 = F.dropout(F.relu(model.l3(h2)), train=False)
    y  = model.l4(h3)

    y_trimed = y.data.argmax(axis=1)
    return np.array(y_trimed, dtype=np.int32)

def change(p, a, b):
    tmp = p[0][a]
    p[0][a] = p[0][b]
    p[0][b] = tmp

p = np.array([[1,0,2,3,4,5,6,7,8]])
p = np.array([[3,1,2,0,4,5,6,7,8]])
p = p.astype(np.float32)

with open('mymodel916.pkl', 'rb')as i:
    model = pickle.load(i)

print "0 is [left], 1 is [up], 2 is [right], 3 is [down]"
print p
direct_ago = -3
j = 0
while ((p[0][0] != 0) or (p[0][1] != 1) or (p[0][2] != 2) or (p[0][3] != 3) or (p[0][4] != 4) or (p[0][5] != 5) or (p[0][6] != 6) or (p[0][7] != 7) or (p[0][8] != 8)):
    direct = predict(p)
    j = j+ 1
    #0(空)の場所を調べる
    i = 0
    while(p[0][i] != 0): i += 1

    #一個前の位置に戻らないように
    if (direct_ago+2 == direct or direct_ago-2 == direct):
        if(i == 0):
            if(direct == 2): direct = 3
            else: direct = 2
        elif(i == 1):
            ran = random.randint(0, 1)
            if(direct == 0):
                if(ran == 0): direct = 2
                else: direct = 3
            elif(direct == 2):
                if(ran == 0): direct = 0
                else: direct = 3
            else:
                if(ran == 0): direct = 0
                else: direct = 2
        elif(i == 2):
            if(direct == 0): direct = 3
            else: direct = 0
        elif(i == 3):
            ran = random.randint(0, 1)
            if(direct == 1):
                if(ran == 0): direct = 2
                else: direct = 3
            elif(direct == 2):
                if(ran == 0): direct = 1
                else: direct = 3
            else:
                if(ran == 0): direct = 1
                else: direct = 2
        elif(i == 4):
            ran = random.randint(0, 2)
            if(direct == 0):
                if(ran == 0): direct = 1
                elif(ran == 1): direct = 2
                else: direct = 3
            elif(direct == 1):
                if(ran == 0): direct = 0
                elif(ran == 1): direct = 2
                else: direct = 3
            elif(direct == 2):
                if(ran == 0): direct = 0
                elif(ran == 1): direct = 1
                else: direct = 3
            else:
                if(ran == 0): direct = 0
                elif(ran == 1): direct = 1
                else: direct = 2
        elif(i == 5):
            ran = random.randint(0, 1)
            if(direct == 0):
                if(ran == 0): direct = 1
                else: direct = 3
            elif(direct == 1):
                if(ran == 0): direct = 0
                else: direct = 3
            else:
                if(ran == 0): direct = 0
                else: direct = 1
        elif(i == 6):
            if(direct == 1): direct = 2
            else: direct = 1
        elif(i == 7):
            ran = random.randint(0, 1)
            if(direct == 0):
                if(ran == 0): direct = 1
                else: direct = 2
            elif(direct == 1):
                if(ran == 0): direct = 0
                else: direct = 2
            else:
                if(ran == 0): direct = 0
                else: direct = 1
        else:
            if(direct == 0): direct = 1
            else: direct = 0

    #予測が外れてインデックスエラーにならないように
    if(i == 0 and (direct == 0 or direct == 1)):
        ran = random.randint(0, 1)
        if(ran == 0): direct = 2
        else: direct = 3
    elif(i == 1 and direct == 1):
        ran = random.randint(0, 2)
        if(ran == 0): direct = 0
        elif(ran == 1): direct = 2
        else: direct = 3
    elif(i == 2 and (direct == 1 or direct == 2)):
        ran = random.randint(0, 1)
        if(ran == 0): direct = 0
        else: direct = 3
    elif(i == 3 and direct == 0):
        ran = random.randint(0, 2)
        if(ran == 0): direct = 1
        elif(ran == 1): direct = 2
        else: direct = 3
    elif(i == 5 and direct == 2):
        ran = random.randint(0, 2)
        if(ran == 0): direct = 0
        elif(ran == 1): direct = 1
        else: direct = 3
    elif(i == 6 and (direct == 0 or direct == 3)):
        ran = random.randint(0, 1)
        if(ran == 0): direct = 1
        else: direct = 2
    elif(i == 7 and direct == 3):
        ran = random.randint(0, 2)
        if(ran == 0): direct = 0
        elif(ran == 1): direct = 1
        else: direct = 2
    elif(i == 8 and (direct == 2 or direct == 3)):
        ran = random.randint(0, 1)
        if(ran == 0): direct = 0
        else: direct = 1

    print direct
    
    if(direct == 0): change(p, i, i-1)
    elif(direct == 1): change(p, i, i-3)
    elif(direct == 2): change(p, i, i+1)
    else: change(p, i, i+3)
    print p
    direct_ago = direct
print j
