import re

# Open the file && count how many lines the file has (must have n+1)
newf=""
pForm = re.compile('Parent process:.*')
cForm = re.compile('Child process:.*')
with open("test.txt", 'r') as fp:
  for line in fp: # for each line in the file
    #Check the format of each line
    if pForm.match('Parent rocess:999') is not None:
      print('matches')
      newf+=line.strip()+"p\n"
  f.close()
  lines = len(fp.readlines())
print('Total Number of lines:', lines)

#Check if the pids are in ascending order