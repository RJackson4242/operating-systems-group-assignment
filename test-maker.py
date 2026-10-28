import subprocess
import random

filePath = "test.txt"
outputLogPath = "output.txt"

output = []


def test(n, cutoff, type):

    if type == 'sorted':
        numbers = range(n)
    elif type == 'rsorted':
        numbers = reversed(range(n))
    elif type == 'random':
        numbers = [random.randint(-2**31, 2**31 - 1) for _ in range(n)]

    with open(filePath, "w", encoding="utf-8") as file:
        file.write(f"{n}\n")
        for number in numbers:
            file.write(f"{number} ")

    #run custom test from python easily and easily from python
    result = subprocess.run(["./test-mergesort-custom", filePath, str(cutoff)], capture_output=True, text=True, check=True)
    output.append(f"n: {n}\nCutoff: {cutoff}\nDescription: {type}\n")
    output.append(result.stdout)
    output.append('\n')
    return 

n = 100000000
for x in range(0,5):
    test(n, x, 'sorted')

for x in range(0,5):
    test(n, x, 'rsorted')
    
for x in range(0,5):
    test(n, x, 'random')