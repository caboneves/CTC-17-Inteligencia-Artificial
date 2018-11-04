#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Wed Sep 26 18:02:01 2018

@author: bruno
"""
import numpy as np
import operator
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.metrics import confusion_matrix
from sklearn.metrics import cohen_kappa_score, make_scorer
from sklearn.metrics import mean_squared_error
from sklearn.metrics import accuracy_score


from collections import Counter, defaultdict
 
def probabilities(outcome):
    no_of_examples = len(outcome)
    prob = dict(Counter(outcome))
    for key in prob.keys():
        prob[key] = prob[key] / float(no_of_examples)
    return prob
 
    

def calculatePriors(training, outcome):
    features = np.unique(outcome)
    rows, cols = np.shape(training)
    similarities = {}
    for ftres in features:
        similarities[ftres] = defaultdict(list)
  
    class_probabilities = probabilities(outcome)
  
    for ftres in features:
        row_indices = np.where(outcome == ftres)[0]
        subConjunto = training[row_indices, :]
        r, c = np.shape(subConjunto)
        for j in range(0,c):
            similarities[ftres][j] += list(subConjunto[:,j])
  
    for ftres in features:
        for j in range(0,cols):
             similarities[ftres][j] = probabilities(similarities[ftres][j])
  
    return class_probabilities,similarities,features



def predictRating(class_probabilities,similarities,features,newGuy):
    results = {}
    for ftres in features:
         probs = class_probabilities[ftres]
         for i in range(len(newGuy)):
             relative_values = similarities[ftres][i]
             if newGuy[i] in relative_values.keys():
                 probs *= relative_values[newGuy[i]]
             else:
                 probs *= 0
             results[ftres] = probs
    return max(results.iteritems(), key=operator.itemgetter(1))[0]
    
    


 

if __name__ == "__main__":
    
    movie_data = pd.read_csv("movies.dat", sep="::", header=None, names=['MovieID','Title','Genres'], 
                           dtype={'MovieID': np.int32, 'Title': np.str, 'Genres': np.str}, engine='python')
    
    
    users_data = pd.read_csv("users.dat", sep="::", header=None, names=['UserID','Gender','Age','Occupation','Zip-code'],
                             
                           dtype={'UserID': np.int32, 'Gender': np.str, 'Age': np.int32, 'Occupation' : np.int32, 'Zip-code' : np.str}, engine='python')
    
    ratings_data = pd.read_csv("ratings.dat",
                           sep="::", header=None, names=['UserID','MovieID','Rating','Timestamp'], 
                           dtype={'UserID': np.int32, 'MovieID': np.int32, 'Rating': np.int32, 'Timestamp' : np.str}, engine='python')
    
    data = pd.merge(ratings_data, users_data, how='left', left_on=['UserID'], right_on=['UserID'])
    data2 = pd.merge(ratings_data, movie_data, how='left', left_on=['MovieID'], right_on=['MovieID'])

    
    from sklearn.preprocessing import LabelEncoder
    le = LabelEncoder()

    
    le.fit(data['Gender'])
    x_g = le.transform(data['Gender'])
    data['Gender'] = x_g
    
    le.fit(data2['Genres'])
    x_genrer = le.transform(data2['Genres'])
    data2['Genres'] = x_genrer
    
    
    
   # corr = data.corr()
   # corr2 = data2.corr()
   # sns.heatmap(corr,annot = True, center=0, square=True)
   # sns.heatmap(corr2,annot = True,center=0,square=True)
    
    
    
    #features= age , gender , movieID
    
    from sklearn.model_selection import train_test_split
    features = data[['Age','Gender','MovieID']]
    features['Genres'] = data2['Genres']
    outcome = data['Rating']
    x_train, x_test, y_train, y_test = train_test_split(features, outcome, test_size=0.25)
    
    class_probabilities,similarities,features = calculatePriors(x_train.values,y_train.values)
    nTotal = len(x_test)
    y_T = y_test.values
    x_T = x_test.values
    predicteds = [predictRating(class_probabilities,
                                 similarities,features,
                                 x_T[i]) for i in range(nTotal)]
    predicteds = np.asarray(predicteds)
    
    print accuracy_score(y_T, predicteds)
    print confusion_matrix(y_T,predicteds)
    print cohen_kappa_score(y_T, predicteds)
    print mean_squared_error(y_T,predicteds)
    
    
    #classificador a priori
    dic = {}
    l = int(0.75*len(data['MovieID']))
    
    for i in range(l):
        p = int(data['MovieID'][i])
        if p not in dic:
          dic[p] =(int(data['Rating'][i]),1)
        else:
            p1 = int(dic[p][0]) + int(data['Rating'][i])
            p2 = int(dic[p][1]) + 1
            dic[p] = (p1,p2)
            
    
    prioriResult = {}
    for i in range(l):
        p = int(data['MovieID'][i])
        if dic[p][1]!=0:
         prioriResult[p] = dic[p][0]/dic[p][1] #classificacao eh a media truncada
         
    results = []
    for i in range(l,len(data['MovieID'])):
        p = int(data['MovieID'][i])
        if p in prioriResult:
         results.append(prioriResult[p])
        else:
            results.append(3) # se nunca viu , da media 3.
    
    y_test2 = data['Rating'][l:]
    y_test2 = y_test2.values
    results = np.asarray(results)
    
    print results,y_test2
    
    print accuracy_score(y_test2, results)
    print confusion_matrix(y_test2,results)
    print cohen_kappa_score(y_test2, results)
    print mean_squared_error(y_test2,results)

   
    
    
    
    
    


