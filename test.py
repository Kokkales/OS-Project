import re
import os
import random
import colorama
from colorama import Fore

# compile the program
os.system('gcc -o app app.c')

# Create different random inputs and run the c programm for each input and save the output
numOfTestFiles=20
for i in range(1,numOfTestFiles):
  os.system('./app '+ str(random. randint(1,300)) + ' test' + str(i) +'.txt')
os.system('chmod +rw test*.txt')

# Check each output and Open the file && count how many lines the file has (must have n+1)
pFormat = re.compile('Parent process : [0-9]')
cFormat = re.compile('Child process : [0-9]')
passCnt=0
for i in range(1,numOfTestFiles):
  passTest=True
  prevPid=0
  numOfLines = 0
  with open("test"+ str(i) +'.txt', 'r') as fp:
    for line in fp: # for each line in the file
      numOfLines+=1
      #Check the format and Check if the pids are in ascending order
      if (pFormat.match(line) is not None and numOfLines==1) or cFormat.match(line):
        txtSplited=line.split()
        if int(txtSplited[3])<prevPid:
          passTest=False
          # print('Pids ascend problem!')
        else:
          passTest=True
      prevPid=int(txtSplited[3])
    fp.close()
  if passTest==True:
    passCnt+=1
    print(Fore.GREEN+"Test"+str(i)+" : Passed")
  else:
    print(Fore.RED+"Test"+str(i)+" : Failed")
print('Passed : '+ str(passCnt) +'/' + str(numOfTestFiles-1))
