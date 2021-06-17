
### python testing

def main ():
    a = 867.0
    b = 1000.0 - a
    Heads = 0
    Tails = 0
    modVal = a / b

    print ("modVal: ", modVal)

    coin = [0] * 1000

    for i in range (1000):
        print (i, "%", modVal, " = ", i % modVal)
        if i % modVal < 1:
            coin[i] = 0
        else:
            coin[i] = 1

    for i in range (1000):
        if coin[i] == 1:
            Heads += 1
        else:
            Tails += 1

    print ("Heads: ", Heads, " - Tails: ", Tails)

if __name__ == '__main__':
    main ()
