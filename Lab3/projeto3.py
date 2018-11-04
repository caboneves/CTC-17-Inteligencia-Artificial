#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Tue Oct  2 19:54:05 2018

@author: bruno
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import networkx as nx
import random
from copy import deepcopy
import sys



   


ROWS = 4
COLUMNS = 8

WUMPUS = 'w'
GOLD = 'g'
PIT = 'p'
EMPTY = '*'
S = [ [EMPTY for j in range(COLUMNS)] for i in range(ROWS)]
S[0][1] = S[0][6] = S[1][2] = S[1][6] = S[3][2] = S[3][6] = PIT
S[1][0] = S[2][4] = WUMPUS
S[1][1] = S[2][5] = GOLD

UP = 0
DOWN = 1
LEFT = 2
RIGHT = 3

actionStr = {
     UP:'↑',
     DOWN:'↓',
     LEFT:'←',   
     RIGHT: '→'
     }

ACTIONS = [UP,DOWN,LEFT,RIGHT]

done_states = [8,9,20,21]


number_states = ROWS*COLUMNS

   
R = np.array([[-10,-100,-10,-50],[-10,+100,0,0],[-10,-50,-50,0],[-10,0,0,0],
     [-10,0,0,0],[10,0,0,-50],[-10,-50,0,0],[-10,0,-50,-10],
     [0,0,-10,+100],[-50,0,-100,-50],[0,0,+100,0],[0,0,-50,0],[0,-100,0,0],[0,+100,0,-50],
     [-50,0,0,0],[0,0,-50,-10],[-100,0,-10,0],[+100,0,0,0],[-50,-50,0,0],[0,0,0,-100],[0,0,0,+100],
     [0,0,-100,0],[-50,-50,+100,0],[0,0,0,-10],[0,-10,-10,0],[0,-10,0,-50],[0,-10,0,0],[0,-10,-50,0],[-100,-10,0,0],
     [+100,-10,0,-50],[0,-10,0,0],[0,-10,-50,-10]])
    
R = R - 0.1 #cada moviemento tem desconto de -0.1

Q = np.zeros((32,4))

trans_matrix = np.array( [
                            [0,8,0,1],[1,9,0,2],[2,10,1,3],[3,11,2,4],[4,12,3,5],[5,13,4,6],[6,14,5,7],[7,15,6,7],
                            [0,16,8,9],[1,17,8,10],[2,18,9,11],[3,19,10,12],[4,20,11,13],[5,21,12,14],[6,22,13,15],[7,23,14,15],
                            [8,24,8,17],[9,25,16,18],[10,26,17,19],[11,27,18,20],[12,28,19,21],[13,29,20,22],[14,30,21,23],[15,31,22,23],
                            [16,24,24,25],[17,25,24,26],[18,26,25,27],[19,27,26,28],[20,28,27,29],[21,29,28,30],[22,30,29,31],[23,31,30,31]
                        ])
   

    
def future_value(state):
   v1 = 0.6*Q[state][UP] +0.4*Q[state][RIGHT]
   v2 = 0.6*Q[state][DOWN] + 0.4*Q[state][LEFT]
   v3 = 0.6*Q[state][LEFT] + 0.4*Q[state][UP]
   v4 = 0.6*Q[state][RIGHT] + 0.4*Q[state][DOWN]
   return max([v1,v2,v3,v4])



def getaction(a):
    r = random.random()
    if a == UP:
        if r<=0.6:
            return UP
        else:
            return RIGHT
    if a == DOWN:
        if r<=0.6:
            return DOWN
        else:
            return LEFT
    if a == LEFT:
        if r<=0.6:
            return LEFT
        else:
            return UP
    if a == RIGHT:
        if r<=0.6:
            return RIGHT
        else:
            return DOWN

def getMatrix(i,j):
    S1 = deepcopy(S)
    S1[i][j] = 'a'
    print('\n')
    for i in range(ROWS):
        
        for j in range(COLUMNS):
            sys.stdout.write(' ')
            sys.stdout.write(S1[i][j])
        print('\n')
        sys.stdout.flush()
   
    


        
        
#treinamento
############################################
episodes = 1000
scores = []
        
gamma = 0.8     
alpha = 0.8
for e in range(episodes):
    start_state = random.randint(0,number_states-1)
    current_state = start_state
    #steps = 0
    while current_state not in done_states :
        #steps += 1
        action = getaction(random.choice(ACTIONS))
        next_state = trans_matrix[current_state][action]
        future_rewards = []
        for act in ACTIONS:
            future_rewards.append(future_value(next_state))
        #bellman equation
        Q[current_state][action] = Q[current_state][action] + \
        alpha*(R[current_state][action] + (gamma)*max(future_rewards) - Q[current_state][action])
    
        current_state = next_state
    if np.max(Q) == 0:
        scores.append(0)
    else:
        scores.append(np.sum(Q/np.max(Q)*100))
       

##########################################
        
#vetor politica , retorna acao dado um estado
pi = [ []  for _ in range(number_states)]


for i in range(number_states):
    qi = Q[i]
    ma = np.max(qi)
    for j in range(len(qi)):
        if np.abs(qi[j]- ma) < 0.1:
            pi[i].append(j)
            

maze = [ ['' for j in range(COLUMNS)] for i in range(ROWS) ]

for i in range(ROWS):
    for j in range(COLUMNS):
        tmp = pi[i*COLUMNS+j]
        for a in tmp:
            maze[i][j] += actionStr[a]

            
print 
print            
for i in range(ROWS):
    sys.stdout.write('\t')
    for j in range(COLUMNS):
        if S[i][j] == 'g' or S[i][j] == 'w':
            sys.stdout.write(' ')
            sys.stdout.write(S[i][j])
            sys.stdout.write('|\t')
        else:
            sys.stdout.write(maze[i][j])
            sys.stdout.write(' ')
            sys.stdout.write(S[i][j])
            sys.stdout.write('|\t')
    sys.stdout.flush()
    print('')

        
plt.xlim(10,episodes/2)
plt.plot(scores)




state = random.randint(0,31)

while state not in done_states:
    nextAct = getaction(pi[state][0])
    state = trans_matrix[state][nextAct]
    getMatrix(state/COLUMNS,state%COLUMNS)
    

    
    
        
        


