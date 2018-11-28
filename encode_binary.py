def bits2String(b):
    return ''.join([chr(int(b[i:i+8], 2)) for i in range(0, len(b), 8)])

filesRoot = r"files/file_"

numFiles = 1
strt = 1
numSet = [10**x for x in range(1, 4)]
for i in range(numFiles):
  inFile = open(filesRoot + str(1) + ".txt", "r")
  outFile = open(filesRoot + str(2) + ".txt", "w")
  for j in range(1, 1001):
    strt = strt+1 if j in numSet else strt 
    outFile.write(str(j) + " " + bits2String(inFile.readline()[strt+1:-9]) + '\n')
  inFile.close()
  outFile.close()