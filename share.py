import os
import shutil
from helper import *
from random import shuffle
import boto3
import datetime


def makeUniqueKey():
    pass

def makeArchive(zipname, dirread):
    try:
        zipname = 'send/' + zipname
        dirread = 'exp/' + dirread
        shutil.make_archive(f'{zipname}', 'zip', dirread)
        return zipname + '.zip'
    except EOFError as er:
        return -1

# makeArchive('beetls', 'Gen Test M101')

def sendArchive(dirname):
    dirname = 'send/' + dirname
    pass


def uploadToYandex(tasksFileNameSimple, dirname, yandexC):
    listfiles = os.listdir(dirname)
    # print(listfiles)
    for files in listfiles:
        print("PENGUINOS" ,files)
        yandexC.upload_file(dirname + files, 'logg-gen',  f'{tasksFileNameSimple}/{files}')
    
    deleteDir(dirname, False)



def shareTasksValues(tasksFileNameSimple, inviteComputerSystemNumber, yandexC):
    try:
        homeDir = 'shareOptimization/'
        tasksFileName = 'T/' + tasksFileNameSimple
        try:
            os.mkdir(homeDir + tasksFileNameSimple)
        except OSError as er:
            pass
        homeDir = homeDir + tasksFileNameSimple + '/'
        lenOfFile = -1
        fileArray = None
        with open(tasksFileName, 'r') as file:
            fileArray = file.readlines()
            lenOfFile = len(fileArray)
        if lenOfFile == -1 or fileArray is None: return -1
        lenOfFile = lenOfFile - 1
        parts = lenOfFile//inviteComputerSystemNumber
        partialTasks = [[] for i in range(inviteComputerSystemNumber)]
        print(parts)
        getColumnsInfo = fileArray[0]
        fileArrayNew = fileArray[1::].copy()
        # print(len(partialTasks))
        # print('check differences ', fileArrayNew[0]," AND " ,fileArray[0])
        # print('check last of fileArray', [fileArray[len(fileArray)-1]])
        # print('check last of fileArrayNew', [fileArrayNew[len(fileArrayNew)-1]])
        # print('check last of fileArrayNew', [fileArrayNew[lenOfFile-1]])
        print(lenOfFile, len(fileArrayNew), len(fileArray))
        shuffle(fileArrayNew)
        position = 0
        for index in range(lenOfFile):
            position = index % len(partialTasks) 
            partialTasks[position].append(fileArrayNew[index])
        # print(partialTasks)
        getFileName = tasksFileNameSimple.split('.')[0]
        for i in range(inviteComputerSystemNumber):
            partialTasks[i].insert(0,'#' + getFileName + getColumnsInfo)
        for l in partialTasks:
            print(len(l))
        for i in range(inviteComputerSystemNumber):
            with open(f'{homeDir}{getFileName}_PART_{i+1}_OF_{inviteComputerSystemNumber}.txt', 'w') as writePartialData:
                for line in partialTasks[i]:
                    writePartialData.write(line)
        
        
        uploadToYandex(tasksFileNameSimple, homeDir, yandexC)
    except OSError as er:
        print(er)

# shareTasksValues('T_for_M70.txt', 4, None)



# shareTasksValues('T_data-2024-05-20_21-24-33_M70.txt', 10, s3)


def deleteChosenTask(taskpath, yandexC, bucketname='logg-gen'):
    forDeletion = [{'Key':'object_name'}, {'Key':taskpath}]
    response = yandexC.delete_objects(Bucket=bucketname, Delete={'Objects': forDeletion})

# deleteChosenTask('T_data-2024-05-23_00-11-30_M80.txt/T_data-2024-05-23_00-11-30_M80_PART_3_OF_5.txt', s3)


def sendGenData(filename, yandexC):
    yandexC.upload_file('GenData/' + filename, 'gen-collect', filename)


# session = boto3.session.Session()
# s3 = session.client(
#     service_name='s3',
#     endpoint_url='https://storage.yandexcloud.net', 
#     aws_access_key_id='YCAJE6ImYoePo5puS8MMCuPyK', 
#     aws_secret_access_key='YCMC-lfDg_UEM5Y28C-v3ZI_l5m-Ak8dYfta74rP'
# )

def contentData(key, bucketname, yandexC):
    data = yandexC.get_object(Bucket=bucketname, Key=key)
    contents = data['Body'].read()
    decodedFile = contents.decode('utf-8')
    return decodedFile


def uniteData(yandexC, bucketname, keyname):
    pass


def genDataUnite(yandexC, bucketname='gen-collect'):
    listoffiles = [key.get('Key') for key in yandexC.list_objects(Bucket=bucketname)['Contents']]
    # print(listoffiles)
    headerFromFile:set = set()
    for key in listoffiles:
        fileTxt = contentData(key, bucketname, yandexC)
        temp_list = fileTxt.split('\n')
        headerFromFile.add(nameNormalize(temp_list))
        # print(temp_list[0][1:-1].split('/')[1])
    dictContainer = {header:[] for header in headerFromFile}
    # print(dictContainer)
    for key in listoffiles:
        fileTxt = contentData(key, bucketname, yandexC)
        temp_list = fileTxt.split('\n')
        dictContainer[nameNormalize(temp_list)].append(key)

    print(dictContainer)

    for kValue in list(dictContainer.keys()):
        items = dictContainer[kValue]
        if items != []:
            dataInsideItems = items[0]
            fileTxt = contentData(dataInsideItems, bucketname, yandexC)
            temp_list = (fileTxt.split('\n'))[0][1:-1].split('/')[1]
            temp_list = temp_list.split('_')[-1]
            print('temp_list ', temp_list)
            value = temp_list.split('.')[0]
            print(value)
            value = int(value.split('-')[-1])
            print("THIS SHIT IS HERE! ", len(items), value)
            if len(items) == value:
                print("CHECK HAPPEND!")
                Text = f'#{kValue}\n'
                for item in items:
                    textFromItem = contentData(item, bucketname, yandexC).split('\r\n')
                    # print(textFromItem)
                    Text += '\n'.join(textFromItem[1:])
                print("FUCK!", Text)
                getMValues = getMs(kValue)
                print('------')
                makeFile = 'gen_data-' + datetime.datetime.now().strftime('%Y-%m-%d_%H-%M-%S') + '_' + getMValues + '.txt'
                with open(f'get/{makeFile}', 'w') as wrt:
                    wrt.write(Text)
                for item in items:
                    deleteChosenTask(item, yandexC, bucketname)


# genDataUnite(s3)

# getMs('T_data-2024-05-23_00-11-30_M70_M80_M55_PART-3-OF-5.txt')

# nameNormalize('T_data-2024-05-25_13-39-07_M69.txt')
# genDataUnite(s3)