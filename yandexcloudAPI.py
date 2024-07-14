#!/usr/bin/env python
#-*- coding: utf-8 -*-
import boto3
session = boto3.session.Session()
s3 = session.client(
    service_name='s3',
    endpoint_url='https://storage.yandexcloud.net', 
    aws_access_key_id='YCAJE6ImYoePo5puS8MMCuPyK', 
    aws_secret_access_key='YCMC-lfDg_UEM5Y28C-v3ZI_l5m-Ak8dYfta74rP'
)

# Создать новый бакет
# s3.create_bucket(Bucket='bucket-name9837t93888')

# Загрузить объекты в бакет

## Из строки
# s3.put_object(Bucket='bucket-name9837t93888', Key='object_name', Body='TEST', StorageClass='COLD')

## Из файла
s3.upload_file('yandexcloudAPI.py', 'plutogen', 'py_script.py')
# s3.upload_file('yandexcloudAPI.py', 'bucket-name9837t93888', 'script/py_script.py')

# Получить список объектов в бакете
# for key in s3.list_objects(Bucket='bucket-name9837t93888')['Contents']:
#     print(key['Key'])

# # Удалить несколько объектов
# forDeletion = [{'Key':'object_name'}, {'Key':'script/py_script.py'}]
# response = s3.delete_objects(Bucket='bucket-name9837t93888', Delete={'Objects': forDeletion})

# # Получить объект
get_object_response = s3.get_object(Bucket='plutogen',Key='py_script.py')
with open('newyandex.txt', 'wb') as w:
    w.write(get_object_response['Body'].read())
# print()
