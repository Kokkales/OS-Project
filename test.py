import re
import os
import random
import colorama
from colorama import Fore

# compile the program
os.system('gcc -o app app.c')

# Create different random inputs and run the c programm for each input and save the output
numOfTestFiles=3
for i in range(0,numOfTestFiles):
  os.system('./app '+ str(random. randint(1,100)) + ' test' + str(i) +'.txt')
os.system('chmod +rw test*.txt')

rex=re.compile("^<[0-9]\d{0,5}> -> <kostas[0-9]>$")
passCnt=0
for i in range(0,numOfTestFiles):
  passTest=True
  prevPid=''
  numOfLines = 0
  with open("test"+ str(i) +'.txt', 'r') as fp:
    for line in fp: # for each line in the file
      numOfLines+=1
      array=line.split()
      #Check the format and Check if the pids are in ascending order
      if ((not rex.match(line)) and array[0]<=prevPid):
        passTest=False
      prevPid=array[0]
    fp.close()
  if passTest==True:
    passCnt+=1
    print(Fore.GREEN+"Test"+str(i)+" : Passed")
  else:
    print(Fore.RED+"Test"+str(i)+" : Failed")
print('Passed : '+ str(passCnt) +'/' + str(numOfTestFiles))
