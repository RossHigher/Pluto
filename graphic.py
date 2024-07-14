import matplotlib as mpl
import matplotlib.pyplot as plt 
import pandas as pd
from helper import sortedListDF
import os
import sys
mpl.use('agg')

blackList = [
        'Crone_Mod', 
        'Critical_Path_Greater', 
        'Critical_Path_Less', 
        'Critical_Path', 
        'Pashkeev', 
        'Pashkeev_Greater', 
        'Pashkeev_Less']

def makeDirectory(pathway):
    if pathway[len(pathway)-1] == '/' or pathway[len(pathway)-1] == '//' or pathway[len(pathway)-1] == '\\':
        pathway = pathway[0: len(pathway)-1]
    splitPath = 'exp/' + pathway
    print(splitPath)
    Images ='Images'
    Bars = 'Bars'
    Lines = 'Lines'
    try:
        os.mkdir(f'{splitPath}/{Images}')
    except OSError as error:
        print(f"Directory {Images} already exist")
    try:
        os.mkdir(f'{splitPath}/{Images}/{Bars}')
    except OSError as error:
        print(f"Directory {Bars} already exist")
    try:
        os.mkdir(f'{splitPath}/{Images}/{Lines}')
    except OSError as error:
        print(f"Directory {Lines} already exist")
    return splitPath

def BestNames(mainDict, blacklist, uniqueN):
    print(mainDict)
    bestNames = []
    for i in uniqueN:
        index = i - uniqueN[0]
        bestName = ''
        minimal = float('inf')
        for name in list(mainDict.keys()):
            if (mainDict[name])[index] < minimal and name not in blacklist:
                minimal = mainDict[name][index]
                bestName = name
        bestNames.append(f'{bestName} {i}')
    return bestNames

def GraphicBar(dataPath, blackList):
    mainPath = makeDirectory(dataPath)
    # mainPath = mainPath.split('/')[0]
    print(mainPath)
    df = pd.read_csv(mainPath + '/Data.csv')
    uniqueN = sortedListDF(df['N'])
    uniqueM = sortedListDF(df['M'])
    uniqueNames = sortedListDF(df['Алгоритм'])
    # print(uniqueM, uniqueN, uniqueNames)

    for M in uniqueM:
        dpi = 200
        fig = plt.figure(dpi = dpi, figsize = (1920 / dpi, 1080 / dpi) )
        mpl.rcParams.update({'font.size': 5})
        ax = plt.axes()
        plt.title(f'Разница между алгоритмами при M = {M}')
        ax.yaxis.grid(True, zorder = 1)
        df_partial1 = df[df['M'] == M].copy()
        NVector = []
        for N in uniqueN:
            df_partial2 = df_partial2 = df_partial1[df_partial1['N'] == N].copy().sort_values('Наибольшая сумма')
            df_partial2 = df_partial2.set_index('Алгоритм')
            medianValue = df_partial2['Наибольшая сумма']
            # print(N, M)
            NVector.append(medianValue.to_dict())
        
        mainDict = {}
        for name in uniqueNames:
            allFromNames = []
            for i in range(len(uniqueN)):
                dictionary = NVector[i]
                allFromNames.append(dictionary[name])
                mainDict[name] = allFromNames

        step = 0
        for name in uniqueNames:
            if(name not in blackList):
                plt.bar([uniqueN[i]+step for i in range(len(uniqueN))], 
                mainDict[name], label = name, alpha = 1.0, width=0.4)
                step += .06
        
        best_names = BestNames(mainDict, blackList, uniqueN)
        plt.xticks(uniqueN, best_names)
        fig.autofmt_xdate(rotation = 45)
        # figure, axis = plt.subplots(2, 2)
        plt.ylabel('Среднее значение')
        plt.xlabel('Количество приборов')
        plt.legend(loc='upper right')
        plt.savefig(f'{mainPath}/Images/Bars/Figure_{M}')
        # plt.show()



def GraphicLine(dataPath, blackList):
    mainPath = makeDirectory(dataPath)
    df = pd.read_csv(mainPath + '/Data.csv')
    uniqueN = sortedListDF(df['N'])
    uniqueM = sortedListDF(df['M'])
    uniqueNames = sortedListDF(df['Алгоритм'])
    # print(uniqueM, uniqueN, uniqueNames)

    for M in uniqueM:
        dpi = 200
        fig = plt.figure(dpi = dpi, figsize = (1920 / dpi, 1080 / dpi) )
        mpl.rcParams.update({'font.size': 5})
        ax = plt.axes()
        ax.yaxis.grid(True, zorder = 1)
        df_partial1 = df[df['M'] == M].copy()
        NVector = []
        for N in uniqueN:
            df_partial2 = df_partial2 = df_partial1[df_partial1['N'] == N].copy().sort_values('Наибольшая сумма')
            df_partial2 = df_partial2.set_index('Алгоритм')
            medianValue = df_partial2['Наибольшая сумма']
            # print(N, M)
            NVector.append(medianValue.to_dict())
        
        mainDict = {}
        for name in uniqueNames:
            allFromNames = []
            for i in range(len(uniqueN)):
                dictionary = NVector[i]
                allFromNames.append(dictionary[name])
                mainDict[name] = allFromNames

        for name in uniqueNames:
            if(name not in blackList):
                plt.title(f'Разница между алгоритмами при M = {M}')
                plt.plot([uniqueN[i] for i in range(len(uniqueN))], 
                mainDict[name], label = name)
        
        best_names = BestNames(mainDict, blackList, uniqueN)
        plt.xticks(uniqueN, best_names)
        fig.autofmt_xdate(rotation = 45)
        # figure, axis = plt.subplots(2, 2)
        plt.ylabel('Среднее значение')
        plt.xlabel('Количество приборов')
        plt.legend(loc='upper right')
        plt.savefig(f'{mainPath}/Images/Lines/Figure_Plot_{M}')
        # plt.show()
