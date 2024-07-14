import time
import json
import random
import os
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split
import joblib

def extract_runtime_value(line):
    try:
        prep = line.split(':')
        if len(prep) <= 1:
            return None
        grep = prep[1].split(',')
        return float(grep[-1].strip())
    except (ValueError, IndexError):
        return None

def extract_numeric_value(filename):
    try:
        return int(''.join(filter(str.isdigit, filename)))
    except ValueError:
        return 0

def collect_runtime_data(directory='GenData'):
    listdata = os.listdir(directory)
    data = []
    for j in listdata:
        with open(f'{directory}/{j}') as file:
            filelist = file.readlines()
            for line in filelist:
                runtime_value = extract_runtime_value(line)
                if runtime_value is not None:
                    data.append({"readFile": f'{directory}/{j}', "Z": "100", "runtime": runtime_value})
                    
    return data

def save_runtime_data(data, filename="runtime_data.json"):
    with open(filename, "w") as f:
        json.dump(data, f)

def load_runtime_data(filename="runtime_data.json"):
    with open(filename, "r") as f:
        return json.load(f)

def prepare_data(data):
    inputs = []
    outputs = []
    for d in data:
        try:
            readFile_num = extract_numeric_value(d["readFile"])
            Z = int(d["Z"])
            runtime = float(d["runtime"])
            inputs.append([readFile_num, Z])
            outputs.append(runtime)
        except ValueError as e:
            print(f"Skipping invalid data entry: {d} due to error: {e}")
    return inputs, outputs

def train_model(inputs, outputs):
    X_train, X_test, y_train, y_test = train_test_split(inputs, outputs, test_size=0.2, random_state=42)
    model = LinearRegression()
    model.fit(X_train, y_train)
    return model

def save_model(model, filename="runtime_model.pkl"):
    joblib.dump(model, filename)

def load_model(filename="runtime_model.pkl"):
    return joblib.load(filename)

# Collect data
data = collect_runtime_data()

# Save data to file
save_runtime_data(data)

# Load runtime data
data = load_runtime_data()

# Prepare the data
inputs, outputs = prepare_data(data)

# Train the model
model = train_model(inputs, outputs)

# Save the model
save_model(model)
