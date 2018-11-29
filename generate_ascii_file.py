import random
import string

def get_random_string(length):
    random_list = []
    for i in range(length):
        random_list.append(random.choice(string.ascii_uppercase + string.digits))
    return ''.join(random_list)

filesRoot = r"files/file_"

numFiles = 1

for i in range(numFiles):
  outFile = open(filesRoot + str(i) + ".txt", "w")
  for j in range(1000):
    length = random.randint(3, 500)
    line = get_random_string(length)
    line = line + '\n'
    outFile.write(line)
  outFile.close()
