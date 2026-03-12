import pandas as pd
import os
import time

def substring_after(s, delim):
    return s.partition(delim)[2]

def substring_before(s, delim):
    return s.partition(delim)[0]


def clean(string):
    tmp = string.replace(" ", "")
    tmp = tmp.replace(";", "")
    tmp = tmp.replace("\n", "")
    return tmp

def removeTab(string):
    return string.replace("\t", "")

def removeDecimal(string):
    return string.replace(".", "")

def check(line):
    if "LPDDR2" in line:
        return "LPDDR2"
    if "WideIO" in line:
        return "WideIO"
    if "DDR3" in line:
        return "DDR3"
    if "DDR4" in line:
        return "DDR4"
    if "Serial" in line:
        return "Serial"
    else:
        return None


file = open("extio_technology_control.txt")
lines = file.readlines()

varList = []
outputs = ["IO Area", "IO Dynamic Power", "IO Termination and Bias Power"]


function = '1'
functionType = "LPDDR2"

result = pd.DataFrame(index= [1], columns= outputs)
for x in range(len(lines)):
    var = None
    val = None

    if check(lines[x]) != None:
        functionType = check(lines[x])


    if "  frequency = freq;" in lines[x]:
        function = '2'

    directory = "iocc_results/" + functionType + "_" + function
    if not os.path.isdir(directory):
        os.makedirs(directory)
    if not os.path.isdir("debug"):
        os.makedirs("debug")


    if "=" in lines[x]: #and function =="2" and functionType == "DDR3"
        text = lines[x].split(" = ")
        if len(text) > 1:
            var = clean(text[0])
            val = clean(text[1])
            # print (removeDecimal(val).isnumeric())
            if removeDecimal(val).isnumeric():
                # print(val)
                controlLine = "\t" + var + " = " + str(val) + ";" + "\n"
                varSpec = var + "_" + function + "_" + functionType

                varList.append(removeTab(varSpec))
                if val == "0":
                    val = 1
                elif "." in val:
                    val = float(val)*1000
                else:
                    val = int(val)*1000

                # print (val)
                print (var + " = " + str(val))
                lines[x] = "\t" + var + " = " + str(val) + ";" +"\n"
                
                with open("extio_technology.cc", 'w') as file:
                    file.writelines(lines) 

                with open("debug/"+varSpec+ "code" + ".txt", 'w') as file:
                    file.writelines(lines) 

                os.system("make > /dev/null 2>&1")
                time.sleep(0.3) #if executed too fast, this program will not work properly. Adding a delay will ensure the program will actually be made
                os.system("./cacti -infile cache.cfg > output.txt")
                os.system("./cacti -infile cache.cfg > debug/" + varSpec + ".txt")
                time.sleep(0.3)
                varSpec = removeTab(varSpec)
                os.system("diff control.txt output.txt > " + directory + "/" + varSpec + ".txt")
                time.sleep(0.3)

                diff = open(directory + "/" + varSpec + ".txt")
                diff = diff.readlines()

                curResult = pd.DataFrame(index = [varSpec], columns=outputs)

                for word in outputs:
                    counter = 0; 
                    value = None
                    value2 = None

                    for line in diff:
                        if line.find(word) != -1 and counter == 1:
                            # print("found2")
                            if word == "IO Dynamic Power":
                                value2 = substring_before(line, "PHY")
                                value2 = substring_after(value2, " = ")
                            else:
                                value2 = substring_after(line, " = ")

                            if value == value2:
                                curResult.at[varSpec, word] =  ""
                            else:
                                curResult.at[varSpec, word] =  value + ", " + value2
                            break
                        elif line.find(word) != -1:
                            # print("found1")
                            counter = 1
                            if word == "IO Dynamic Power":
                                value = substring_before(line, "PHY")
                                value = substring_after(value, " = ")
                            else:
                                value = substring_after(line, " = ")
                        else:
                            if pd.isnull(curResult.at[varSpec, word]):
                                curResult.at[varSpec, word] = " "   

                    if word == "IO Termination and Bias Power":
                        result = pd.concat([result, curResult])


                lines[x] = controlLine
                print(result)
                with open("extio_technology.cc", 'w') as file:
                    file.writelines(lines) 
result.to_csv("check.csv", index=True, header=True)

                
