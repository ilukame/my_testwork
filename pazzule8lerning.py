# -*- coding: utf-8 -*-
import numpy as np
import chainer
from chainer import cuda, Function, FunctionSet, gradient_check, Variable, optimizers, serializers, utils
from chainer import Link, Chain, ChainList
import chainer.functions as F
import chainer.links as L
import sys
import pickle

# 確率的勾配降下法で学習させる際の１回分のバッチサイズ
batchsize = 100
# 学習の繰り返し回数
n_epoch   = 20
# 中間層の数
n_units   = 1000

data = np.genfromtxt('test.csv', delimiter = ',', dtype = np.uint8)
target = data[:, 9]
data = data[:, 0:9]

target = target.astype(np.int32)
data = data.astype(np.float32)
print data
print target

N = 100000
x_train, x_test = np.split(data, [N])
y_train, y_test = np.split(target, [N])
N_test = y_test.size

# Prepare multi-layer perceptron model
# 多層パーセプトロンモデルの設定
# 入力 9次元、出力 4次元
model = FunctionSet(l1 = F.Linear(9, n_units),
                    l2 = F.Linear(n_units, n_units),
                    l3 = F.Linear(n_units, n_units),
                    l4 = F.Linear(n_units, 4))

#ニューラルネットワークの構造
def forward(x_data, y_data, train = True):
    x, t = Variable(x_data), Variable(y_data)
    h1 = F.dropout(F.relu(model.l1(x)), train = train)
    h2 = F.dropout(F.relu(model.l2(h1)), train = train)
    h3 = F.dropout(F.relu(model.l3(h2)), train = train)
    y = model.l4(h3)
    # 多クラス分類なので誤差関数としてソフトマックス関数の
    # 交差エントロピー関数を用いて、誤差を導出
    return F.softmax_cross_entropy(y, t), F.accuracy(y, t)

# Setup optimizer
optimizer = optimizers.Adam()
optimizer.setup(model)
    
l1_W = []
l2_W = []
l3_W = []
l4_W = []

# Learning loop
for epoch in xrange(1, n_epoch+1):
    print 'epoch', epoch

    # training
    # N個の順番をランダムに並び替える
    perm = np.append(np.array(range(100)), np.random.permutation(N))

    sum_accuracy = 0
    sum_loss = 0
    # 0〜Nまでのデータをバッチサイズごとに使って学習
    for i in xrange(0, N, batchsize):
        x_batch = x_train[perm[i:i+batchsize]]
        y_batch = y_train[perm[i:i+batchsize]]

        # 勾配を初期化
        optimizer.zero_grads()
        # 順伝播させて誤差と精度を算出
        loss, acc = forward(x_batch, y_batch)
        # 誤差逆伝播で勾配を計算
        loss.backward()
        optimizer.update()

    # 学習したパラメーターを保存
    l1_W.append(model.l1.W)
    l2_W.append(model.l2.W)
    l3_W.append(model.l3.W)
    l4_W.append(model.l4.W)

model.to_cpu()
with open('mymodel916.pkl', 'wb') as o:
    pickle.dump(model, o)









