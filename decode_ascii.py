import random
import string

def get_random_string2(length):
    random_list = []
    for i in range(length):
        random_list.append(random.choice(string.ascii_uppercase + string.digits))
    asciiString = ''.join(random_list)
    asciiList = [bin(ord(x))[2:].zfill(8) for x in asciiString]
    return ''.join(str(x)for x in asciiList)

def get_random_string(length):
	randomList = []
	for i in range(length):
		randomList.append(random.choice(string.ascii_uppercase + string.digits))
	return ''.join(randomList)

filesRoot = r"files/file_"

numFiles = 1

for i in range(numFiles):
  outFile = open(filesRoot + str(1) + ".txt", "w")
  for j in range(2):
    length = random.randint(3, 500)
    lineNumLen = 5 - len(str(j+1))
    lineNum = ''.zfill(lineNumLen) + str(j+1)
    line = get_random_string(length) #+ "00001010"
    #line = str(j+1) + " " + line + '\n'
    line = lineNum + " " + line + '\n'
    outFile.write(line)
  outFile.close()
