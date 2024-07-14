"""""Это код для помощи в обработке данных, сгенерированных в коде языка C++"""
import pandas as pd
import os
import shutil
# import math


def makeDir(directory, expdir=True):
    try:
        if expdir is True: 
            if len(directory.split('/')) > 1 or len(directory.split('\\')) > 1 or len(directory.split('//')) > 1:
                raise OSError
            directory = 'exp/' + directory
        os.mkdir(directory)
        return directory
    except OSError as error:
        print(f"Directory {directory} already exist")
        return -1

def deleteDir(directory, expdir=True):
    try:
        if expdir is True:
            directory = 'exp/' + directory
        shutil.rmtree(directory)
        return 1
    except OSError as error:
        print('error during deleting file')
        return -1

# makeDir('fkk')
# deleteDir('fkk')

def makeEssentionalDirs():
    try:
        os.mkdir(f'shareOptimization')
    except OSError as error:
        print(f"Directory shareOptimization already exist")

    try:
        os.mkdir(f'T')
    except OSError as error:
        print(f"Directory T already exist")

    try:
        os.mkdir(f'send')
    except OSError as error:
        print(f"Directory send already exist")

    try:
        os.mkdir(f'exp')
    except OSError as error:
        print(f"Directory exp already exist")
        
    try:
        os.mkdir(f'Data')
    except OSError as error:
        print(f"Directory Data already exist")
        
    try:
        os.mkdir(f'GenData')
    except OSError as error:
        print(f"Directory GenData already exist")
        
    print("Succsess!")
    return 1

def MapHelper(value):
    try:
        if len(value.split(' ')) > 1:
            return [int(val) for val in value.split(' ')]
        else:
            return int(value)
    except ValueError:
        return value


def LineConvertHelper(singleLine):
    """На вход подается строка, но в последнем месте стоит \\n поэтому это помогает"""
    singleLine = singleLine.split(':')
    singleLine = [line.strip() for line in singleLine]
    singleLine[len(singleLine) - 1] = ((singleLine[len(singleLine) - 1]).split('\n'))[0]
    return list(map(MapHelper, singleLine))


def ParcerText(file):
    """Строки превращает в один большой список списков"""
    MainList = []
    with open(file, 'r') as dataRead:
        while True:
            singleLine = dataRead.readline()
            if '#' in singleLine: continue
            if singleLine == '': break
            singleLine = LineConvertHelper(singleLine)
            # print(singleLine)
            MainList.append(singleLine)
    return MainList



def PandasMaker(file, dirname:str, maindir='exp'):
    dirname = f'{maindir}/' + dirname
    """Процесс чтения txt файла, сгенерированного на C++, 
    его форматирование и усреднение значений"""
    try:
        os.mkdir(dirname)
    except OSError as error:
        print("No, you can\'t")
    
    if type(file) is not str:
        df = file
    else:
        df = pd.DataFrame(ParcerText(file))


    # df[2] = df[2].astype(float)
    uniqueN = set(df[3].tolist())
    print(df)
    uniqueM = set(df[4].tolist())
    print(uniqueM)
    uniqueNames = set(df[0].tolist())
    # print(uniqueM, uniqueN, uniqueNames)

    MainList = []
    for name in uniqueNames:
        df_partial = df[df[0] == name]
        for N in uniqueN:
            df_partial1 = df_partial[df_partial[3] == N].copy()
            for M in uniqueM:
                df_partial2 = df_partial1[df_partial1[4] == M].copy()
                # if type(file) is str:
                #     df_partial2 = df_partial2.iloc[:-1] 
                val = df_partial2[2].mean()
                # val = float(val)
                HEAD = (df_partial2.head(1)).copy()
                HEAD[2] = val
                # print(HEAD)
                for index, rows in HEAD.iterrows():
                    MainList.append(rows)
    df = pd.DataFrame(MainList)
    df.rename(columns={0:'Алгоритм', 1:'Суммы', 2:'Наибольшая сумма', 3:'N', 4:'M'}, inplace=True)
    
    for N in uniqueN:
        df_partial1 = df[df['N'] == N].copy()
        for M in uniqueM:
            MPath = f'{dirname}/M{M}/'
            try:
                os.mkdir(MPath)
            except OSError as error:
                print("This path does exist")
            
            df_partial2 = df_partial1[df_partial1['M'] == M].copy()
            df_sorted = df_partial2.sort_values('Наибольшая сумма')
            
            df_sorted[['Алгоритм', 'Наибольшая сумма', 'N', 'M']].to_csv(f'{MPath}Log_{N}_{M}.csv', encoding='utf-8', index=False)
            print(df_sorted[['Алгоритм', 'Наибольшая сумма', 'N', 'M']].to_string())
    df.to_csv(f'{dirname}/Data.csv', encoding='utf-8', index=False)
    return uniqueM, uniqueN, uniqueNames


def sortedListDF(df):
    """Уникальные значения по столбцу """
    return sorted(list(set(df.tolist())))

def getUniqueNames(dataPath):
    dataPath = 'exp/' + dataPath + '/Data.csv'
    df = pd.read_csv(dataPath)
    uniqueNames = sortedListDF(df['Алгоритм'])
    return uniqueNames


def PredictConst(dataPath, Algoritm, differnce):
    """Вычисляет константу, которая способна предугадать ухудшение алгоритма Крона"""
    dataPath = 'exp/' + dataPath + '/Data.csv'
    df = pd.read_csv(dataPath)
    uniqueN = sortedListDF(df['N'])
    uniqueM = sortedListDF(df['M'])
    uniqueNames = sortedListDF(df['Алгоритм'])
    # print(uniqueM, uniqueN, uniqueNames)

    MN_Pair = []
    for M in uniqueM:
        df_partial1 = df[df['M'] == M].copy()
        for N in uniqueN:
            df_partial2 = df_partial1[df_partial1['N'] == N].copy()
            crone_row = df_partial2[df_partial2['Алгоритм'] == Algoritm].copy()
            other_row = df_partial2[df_partial2['Алгоритм'] != Algoritm].copy()
            other_row = other_row[other_row['Алгоритм'] != 'Crone_Mod'].sort_values('Наибольшая сумма').head(1)
            other_row_value, crone_row_value = float(other_row['Наибольшая сумма'].iloc[0]), float(crone_row['Наибольшая сумма'].iloc[0])
            if other_row_value < crone_row_value and abs(other_row_value - crone_row_value) > differnce:
                detect = [int(crone_row['N'].iloc[0]), int(crone_row['M'].iloc[0])]
                detect.append(detect[0]/detect[1])
                MN_Pair.append(detect)
                print(detect)
                print(crone_row_value, other_row_value)
                break
    
    commut = 0
    for line in MN_Pair:
        commut += line[2]
    if len(MN_Pair) == 0: return -1
    res = commut/len(MN_Pair)
    print(res)
    return res


def PredictValue(predictConst, M):
    """Вычисляет с какого прибора наступает"""
    if predictConst != -1:
        return int(predictConst * M) 
    else:
        return "Not found"


def ConvertImportedToStandart(dataframe):
    newDataFrame = dataframe[['id', 'time_decision','max_value', 'm', 'n']].copy()
    newDataFrame['id'] = 'Romanovsky'
    newDataFrame.rename(columns={'id': 0, 'time_decision': 1,'max_value': 2, 'm': 3, 'n': 4}, inplace=True)
    return newDataFrame
    

def PandasMakerExcel(dirread, dirmake):
    if dirread.endswith('/') is False: dirread += '/'
    filenames = os.listdir(dirread)
    print(filenames)
    dataFrames = [pd.read_excel(dirread + names) for names in filenames]
    dataFrames = [ConvertImportedToStandart(frame) for frame in dataFrames]
    unitedDataFrame = dataFrames[0].copy()
    for i in range(1, len(dataFrames)):
        unitedDataFrame = pd.concat([unitedDataFrame, dataFrames[i]])
    print(unitedDataFrame.to_string())
    PandasMaker(unitedDataFrame, dirmake, maindir='exp')


def Checker(excelfile, csvfile, N):
    if excelfile.endswith('/') is False: excelfile += '/'
    filenames = os.listdir(excelfile)
    print(filenames)
    dataFrames = [pd.read_excel(excelfile + names) for names in filenames]
    dataFrames = [ConvertImportedToStandart(frame) for frame in dataFrames]
    unitedDataFrame = dataFrames[0].copy()
    for i in range(1, len(dataFrames)):
        unitedDataFrame = pd.concat([unitedDataFrame, dataFrames[i]])
    unitedDataFrame[2] = unitedDataFrame[2].astype(float)
    df = pd.DataFrame(ParcerText(csvfile))
    
    Crones = df[df[0] == 'Crone_After_Goldberg']
    Crones = Crones[Crones[3] == N]
    # Crones = Crones.iloc[:-1]
    
    # print(Crones.to_string())
    Crones[2] = Crones[2].astype(float)
    unitedDataFrame = unitedDataFrame[unitedDataFrame[3] == N]
    print(unitedDataFrame.to_string())
    # print(unitedDataFrame[2] == Crones[2])
    val = unitedDataFrame[2].isin(Crones[2]).value_counts()
    print("so value is", val[0])
    val = pd.merge(unitedDataFrame, Crones, on=[2], how='inner')
    # print(val.to_string())

    count  = 0
    print(len(unitedDataFrame[2].tolist()), len(Crones[2].tolist()))
    md = [0, 0]
    for i,j in zip(unitedDataFrame[2].tolist(), Crones[2].tolist()):
        ok = 'is okay'
        if i == j:
            count += 1
        
        else:
            if i > j:
                ok = 'IS NOT OK'
        print(i, j, count, ok)

        md[0] += i
        md[1] += j
    print([m/len(unitedDataFrame[2].tolist()) for m in md])


# for i in range(5, 11):
#     Checker('exp\Tests\Romanovsky\Examples M70N5_15', 'get\gen_data-2024-05-28_02-00-12_M70.txt', 5)

# PandasMaker('GenData/gen_data-2024-05-23_17-09-53_M70.txt', 'TEST')
# PandasMaker('get/gen_data-2024-05-27_15-44-57_M70.txt', 'TEST2')



def getFileNamesFromDir(dirname='T'):
    if dirname.endswith('/') is False: dirname += '/'
    listfiles = os.listdir(dirname)
    return listfiles

def loadExcelFilesIntoDir(dirmake):
    makeDir()


def getMs(T):
    rd = T.split('_')
    # print(rd)
    getM = []
    for val in rd:
        if 'M' in val:
            getM.append(val)
    stringM = '_'.join(getM)
    # print(stringM)
    stringM = stringM.split('.')[0]
    return stringM


def nameNormalize(T):
    T = T[0][1:-1].split('/')[1]
    getM = getMs(T)
    rd = T.split('_')
    # print(rd)
    newName = []
    for val in rd:
        if 'M' in val:
            break
        newName.append(val)
    # print(newName)
    stringName = '_'.join(newName) + '_' + getM
    stringName = stringName.split('.')[0]
    stringName += '.txt'
    # print(stringName)
    return stringName
