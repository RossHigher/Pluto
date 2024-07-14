from flask import Flask, request, jsonify, send_file, make_response
from flask_cors import CORS
from graphic import *
from helper import *
from cppConvertor import *
import joblib
import os
import logging
import urllib.parse
import base64
import boto3
from share import *

app = Flask(__name__)
CORS(app)
print("LETS START IT")

session = boto3.session.Session()
s3 = session.client(
    service_name='s3',
    endpoint_url='https://storage.yandexcloud.net', 
    aws_access_key_id='***************', 
    aws_secret_access_key='*************************'
)

makeEssentionalDirs()

# model = joblib.load("runtime_model.pkl")

def extract_numeric_value(filename):
    try:
        return int(''.join(filter(str.isdigit, filename)))
    except ValueError:
        return 0

def estimate_runtime(readFile, Z):
    numeric_value = extract_numeric_value(readFile)
    # estimate = model.predict([[numeric_value, int(Z)]])
    return "10000"



@app.route('/api/get-txt-files', methods=['POST'])
def get_txt_files():
    data = request.json
    directory = data['directory']
    
    try:
        files = [f for f in os.listdir(directory) if f.endswith('.txt')]
        return jsonify({"files": files})
    except Exception as e:
        return jsonify({"message": str(e)}), 500


@app.route('/api/predict-const', methods=['POST'])
def predict_const():
    data = request.json
    mainDirectory = data['mainDirectory']
    Algorithm = data['Algorithm']
    difference = data['difference']
    predictCnst = PredictConst(f'{mainDirectory}', Algorithm, difference)

    if predictCnst is None:
        return jsonify({"message": "Error calculating PredictConst"}), 500

    return jsonify({"predictCnst": predictCnst})

@app.route('/api/predict-value', methods=['POST'])
def predict_value():
    data = request.json
    predictCnst = data['predictCnst']
    someValue = data['someValue']

    result = PredictValue(predictCnst, someValue)

    return jsonify({"result": result})

@app.route('/api/pandas-maker', methods=['POST'])
def pandas_maker():
    data = request.json
    mainDirectory1 = data['mainDirectory1']
    gen_data = data['data']
    print("FUCK")

    PandasMaker(f'{gen_data}', mainDirectory1)
    GraphicBar(f'{mainDirectory1}', blackList)
    GraphicLine(f'{mainDirectory1}', blackList)

    return jsonify({"message": "PandasMaker executed successfully"})

@app.route('/api/pandas-maker-excel', methods=['POST'])
def pandas_maker_excel():
    data = request.json
    mainDirectory1 = data['mainDirectory2']
    ExcelDirectoryRead = data['excelDirectoryRead']

    # Assuming you have a function PandasMakerExcel
    PandasMakerExcel(f'{ExcelDirectoryRead}', mainDirectory1)
    GraphicBar(f'{mainDirectory1}', blackList)
    GraphicLine(f'{mainDirectory1}', blackList)

    return jsonify({"message": "PandasMakerExcel executed successfully"})

@app.route('/api/get-algorithms', methods=['POST'])
def get_algorithms():
    data = request.json
    mainDirectory = data['mainDirectory']
    
    try:
        algorithms = getUniqueNames(mainDirectory)
        return jsonify({"algorithms": algorithms})
    except Exception as e:
        return jsonify({"message": str(e)}), 500

@app.route('/api/run-algorithms', methods=['POST'])
def run_algorithms():
    data = request.json
    Ns = data['Ns']
    Ms = data['Ms']
    Interval = data['Interval']
    maxMatrixGenerated = data['maxMatrixGenerated']

    stdout, stderr = runAlgorithms(Ns, Ms, Interval, maxMatrixGenerated)
    return jsonify({"stdout": 'Success', "stderr": ' - Powered by C++'})

@app.route('/api/estimate-runtime', methods=['POST'])
def estimate_runtime_endpoint():
    data = request.json
    readFile = data['readFile']
    Z = data['Z']

    estimated_time = estimate_runtime(readFile, Z)
    print(f"Estimated time (ms): {estimated_time}")  # Debug print
    
    return jsonify({"estimated_time": estimated_time})

@app.route('/api/run-genetic-algorithms', methods=['POST'])
def run_genetic_algorithms_endpoint():
    data = request.json
    readFile = data['readFile']
    Z = data['Z']

    message = runGeneticAlgorithms(readFile, Z, '0')
    
    return jsonify({"message": message})


@app.route('/api/run-genetic-algorithms-user', methods=['POST'])
def run_genetic_algorithms_user_endpoint():
    data = request.json
    readFile = data['readFile']
    Z = data['Z']

    message = runGeneticAlgorithms(readFile, Z, '1')
    
    return jsonify({"message": message})



@app.route('/api/get-filenames', methods=['GET'])
def get_filenames():
    dirname = request.args.get('dirname', 'T')
    try:
        filenames = getFileNamesFromDir(dirname)
        return jsonify({"filenames": filenames})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

logging.basicConfig(level=logging.DEBUG)

BASE_DIR = os.getcwd()


# @app.route('/api/files-partial', methods=['GET'])
# def list_files_partial():
#     base_directory = os.path.join(BASE_DIR, 'shareOptimization')
#     directories = os.listdir(base_directory)
    
#     directories_info = []

#     for directory in directories:
#         dir_path = os.path.join(base_directory, directory)
#         if os.path.isdir(dir_path):
#             files = [f for f in os.listdir(dir_path) if f.endswith('.txt')]
#             directories_info.append({
#                 'name': directory,
#                 'files': files
#             })

#     return jsonify(directories_info)



@app.route('/api/files', methods=['GET'])
def list_files():
    print(BASE_DIR)
    directory_path = request.args.get('path', default='.')
    directory_path = BASE_DIR + "/" + directory_path
    print(directory_path)
    if not os.path.exists(directory_path):
        return jsonify({'error': 'Directory does not exist'}), 404

    try:
        files = os.listdir(directory_path)
        files.sort()
        print(files)
        files_and_folders = [
            {'name': f, 'isDirectory': os.path.isdir(os.path.join(directory_path, f))}
            for f in files
        ]
        return jsonify(files_and_folders)
    except Exception as e:
        print("BINGO")
        return jsonify({'error': 'Unable to scan directory', 'details': str(e)}), 500

@app.route('/api/file-content', methods=['GET'])
def file_content():
    file_path = request.args.get('path', default='.')
    file_path = BASE_DIR + "/" + file_path
    extension = os.path.splitext(file_path)[1].lower()
    app.logger.info(f"Fetching file content for path: {file_path}")

    if not os.path.exists(file_path):
        return jsonify({'error': 'File does not exist'}), 404

    try:
        if extension in ['.txt', '.csv']:
            with open(file_path, 'r', encoding='utf-8') as file:
                data = file.read()
            return jsonify({'content': data})
        else:
            with open(file_path, 'rb') as file:
                data = file.read()
            encoded_data = base64.b64encode(data).decode('utf-8')
            app.logger.info(f"Successfully encoded file {file_path}")
            return jsonify({'content': encoded_data})
    except Exception as e:
        app.logger.error(f"Error reading file {file_path}: {e}")
        return jsonify({'error': 'Unable to read file', 'details': str(e)}), 500


# @app.route('/api/download-file', methods=['GET'])
# def download_file():
#     file_path = request.args.get('path')
#     print(file_path)
#     if not file_path:
#         return jsonify({'error': 'File path is required'}), 400
#     file_path ='shareOptimization\\' + '\\'.join(file_path.split('/'))
#     full_path = os.path.join(BASE_DIR, file_path)
#     print(full_path)
#     if not os.path.exists(full_path):
#         return jsonify({'error': 'File does not exist'}), 404

#     try:
#         return send_file(full_path, as_attachment=True)
#     except Exception as e:
#         return jsonify({'error': 'Unable to download file', 'details': str(e)}), 500




@app.route('/api/files-partial', methods=['GET'])
def list_yandex_files():
    bucket_name = 'logg-gen'  # Ваше имя бакета
    prefix = 'shareOptimization/'  # Префикс пути

    try:
        result = s3.list_objects_v2(Bucket=bucket_name)
        if 'Contents' in result:
            files = [obj['Key'] for obj in result['Contents']]
            directories = {}
            for file in files:
                if file.endswith('/'):
                    directories[file] = []
                else:
                    dir_name = os.path.dirname(file) + '/'
                    if dir_name in directories:
                        directories[dir_name].append(os.path.basename(file))
                    else:
                        directories[dir_name] = [os.path.basename(file)]
            return jsonify(directories)
        else:
            return jsonify([]), 404
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/api/download-file', methods=['GET'])
def download_file():
    file_path = request.args.get('path', default='.')
    print("OH FUUUUUUUCK!",file_path)
    bucket_name = 'logg-gen'  # Ваше имя бакета

    try:
        file_obj = s3.get_object(Bucket=bucket_name, Key=file_path)
        data = file_obj['Body'].read()
        getName = file_path.split('/')
        with open(f'DownloadedTasks/{getName[1]}', 'wb') as writedata:
            writedata.write(data)
        response = make_response(data)
        response.headers['Content-Disposition'] = f'attachment; filename={os.path.basename(file_path)}'
        response.mimetype = 'application/octet-stream'
        deleteChosenTask(file_path, s3)
        return response
    except Exception as e:
        return jsonify({'error': str(e)}), 500



@app.route('/api/share-tasks', methods=['POST'])
def share_tasks():
    data = request.json
    tasksFileNameSimple = data['tasksFileNameSimple']
    print(tasksFileNameSimple)
    inviteComputerSystemNumber = int(data['inviteComputerSystemNumber'])
    try:
        shareTasksValues(tasksFileNameSimple, inviteComputerSystemNumber, s3)
        return jsonify({"message": "Tasks shared and uploaded successfully"})
    except Exception as e:
        return jsonify({"error": str(e)}), 500


@app.route('/api/upload-gen-data', methods=['POST'])
def upload_gen_data():
    data = request.json
    filename = data.get('filename')
    
    try:
        sendGenData(filename, s3)
        return jsonify({"message": f"File {filename} uploaded successfully"})
    except Exception as e:
        return jsonify({"message": str(e)}), 500


@app.route('/api/gen-data-unite', methods=['POST'])
def generate_data():
    try:
        genDataUnite(s3)
        return jsonify({"message": "Data generated successfully"}), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True, port=5001)
