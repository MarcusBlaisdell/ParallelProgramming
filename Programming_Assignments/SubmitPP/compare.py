
def main ():
    serial1  = open ("serial_baselineoutput.txt", "r")
    serial2  = open ("serial_baselineoutput.txt", "r")
    parallel = open ("paralleloutput.txt", "r")

    serial_1 = serial1.readline ()
    serial_2 = serial2.readline ()
    parallel_1 = parallel.readline ()

    i = 0;

    while (serial_1):
        if serial_1 != serial_2:
            print ("serial mismatch, line ", i)

        if serial_1 != parallel_1:
            print ("parallel mismatch, line ", i)

        serial_1 = serial1.readline ()
        serial_2 = serial2.readline ()
        parallel_1 = parallel.readline ()

        i += 1

if __name__=="__main__":
    main ()
