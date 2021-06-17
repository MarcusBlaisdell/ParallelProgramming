###

getList = open("thelist.txt", "r")

line = getList.readline()
line = line[:-1]

myMax = 0

while line:
  readFile = open(line, "r")
  line2 = readFile.readline()
  #line2 = line2[:1]

  while line2:
    line3 = line2.split()
    #print ("line3[1]: ", line3[1], " - myMax: ", myMax)

    if int(line3[1]) > myMax:
      #print (line3[1], " is greater than ", myMax)
      myMax = int(line3[1])
      #print ("myMax: ", myMax)

    line2 = readFile.readline()
    #line2 = line2[:1]

  readFile.close()

  print (myMax)
  myMax = 0
  line = getList.readline()
  line = line[:-1]

getList.close()
