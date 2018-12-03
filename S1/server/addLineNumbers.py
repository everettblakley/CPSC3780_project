lineNumber = 00001
inTxt = open("sherlock_holmes.txt", "r").readlines()
outTxt = open("sherlock_holmes_2.txt", "w")

for line in inTxt:
    outTxt.write('{0:0>5} {1}'.format(str(lineNumber), line))
    lineNumber += 1

outTxt.close()
