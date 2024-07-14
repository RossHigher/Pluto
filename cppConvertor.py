import subprocess
import datetime
from helper import *

# Создать имя файла

# Задать параметры через React
# Ns = "10,10"
# Ms = "70"
# Interval = "10,20"
# maxMatrixGenerated = "99"

# Запустить скомпилированный C++ файл и передать аргументы
def runAlgorithms(Ns:str, Ms:str, Interval:str, maxMatrixGenerated:str):
    # Тут не нужен txt расширение, в C++ с этим сам справится в соответствующем методе
    fileName = 'data-' + datetime.datetime.now().strftime('%Y-%m-%d_%H-%M-%S') + '_M' + '_'.join(Ms.split(',')) + '.txt'
    print(fileName)
    try:
        result = subprocess.run(
            ['OptimizationTest', fileName, Ns, Ms, Interval, maxMatrixGenerated],
            check=True
            # text=True,
            # capture_output=True
        )
        # Вывод стандартного вывода и ошибок
        print("STDOUT:", result.stdout)
        print("STDERR:", result.stderr)
        return result.stdout, result.stderr
        
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
        print(f"STDOUT: {e.stdout}")
        print(f"STDERR: {e.stderr}")
        return e.stdout, e.stderr


def runGeneticAlgorithms(readFile:str, Z:str, chosen:str):
    # Создать имя файла
    getMValues = getMs(readFile)
    makeFile = 'gen_data-' + datetime.datetime.now().strftime('%Y-%m-%d_%H-%M-%S') + '_' + getMValues + '.txt'

    print(makeFile)
    try:
        result = subprocess.run(
            ['Genetic', readFile, makeFile, Z, chosen],
            check=True
        )
        # Сообщение о завершении
        print("Process finished successfully.")
        return "Process finished successfully."
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
        return f"Error: {e}"


# runGeneticAlgorithms("T_for_M70.txt", "100")

# runAlgorithms(Ns, Ms, Interval, maxMatrixGenerated)
