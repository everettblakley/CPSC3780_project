import os

# for R,D,F in os.walk(os.getcwd()):
#     for f in F:
#         if f.endswith(".txt"):
#             inTxt = open(os.path.join(R, f), "r").readlines()
#             outFileName = f[:f.rfind(".")]  + "_fixed.txt"
#             outTxt = open(os.path.join(R, outFileName), "w")
#             for line in inTxt:
#                 outTxt.write('0{0}'.format(line))
#
#             print "Completed " + f
#             outTxt.close()
#
# for R,D,F in os.walk(os.getcwd()):
#     for f in F:
#         if f.endswith(".txt"):
inTxt = open("sherlock_holmes.txt", "r").readlines()
outFileName = "sherlock_holmes_fixed.txt"
outTxt = open(outFileName, "w")
for line in inTxt:
    lineNum = int(line[:line.find(" ")])
    outTxt.write('{0:0>6} {1}'.format(lineNum, line[line.find(" "):]))

print "Completed "
outTxt.close()
