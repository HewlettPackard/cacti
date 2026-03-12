
import pandas as pd
import os
import shutil
import subprocess


names = ["architecture", "current", "semiconductor", "SRAM_data", "CAM_data", "DRAM_data", "DRAM_cell", "chip_data_dict", "wire", "TSV_data"]

def float_to_exponential(num):
    return format(num, "e")

def substring_after(s, delim):
    return s.partition(delim)[2]

# read the unit header but don't add the column label to it 
def parse(file, first, last):
    chunk = file.iloc[first:last]
    chunk= chunk['parameters'].str.split(expand = True)
    chunk = chunk.reset_index(drop=True)
    chunk.loc[len(chunk)] = None
    return chunk

def partition(tech_param):
    tech_param = pd.read_csv(tech_param, names = ['parameters'])

    architecture = parse(tech_param, 1, 13)
    architecture.columns =['parameters', '(unit)', 'hp','lstp','lop','lp-dram','comm-dram']

    current = parse(tech_param, 14, 36)
    current.columns =['parameters', '(unit)', 'temp', 'hp','lstp','lop','lp-dram','comm-dram']

    semiconductor = parse(tech_param, 37, 44)
    semiconductor.columns =['parameters', '(unit)', 'hp', 'lstp', 'lop', 'lp-dram', 'comm-dram']

    SRAM_data = parse(tech_param, 46, 51)
    SRAM_data.columns =['parameters', '(unit)', 'cell_type', 'hp', 'lstp', 'lop','lp-dram','comm-dram']

    CAM_data = parse(tech_param, 53, 58)
    CAM_data.columns =['parameters', '(unit)', 'cell_type', 'hp', 'lstp', 'lop','lp-dram','comm-dram']

    DRAM_data = parse(tech_param, 60, 66)
    DRAM_data.columns =['parameters', '(unit)','cell_type', 'hp', 'lstp', 'lop','lp-dram','comm-dram']

    DRAM_cell= parse(tech_param, 67, 71)
    DRAM_cell.columns =['parameters', '(unit)', 'hp',  'lstp',   'lop',  'lp-dram',   'comm-dram' ]

    chip_data = parse(tech_param, 71, 78)
    chip_data.columns =["parameters", "(unit)", "value"]
    # chip_data_dict = {chip_data[0][0]: chip_data[1][0]}
    # for x in range(8):
    #     chip_data_dict.update ({chip_data[0][x]: chip_data[1][x]})

    wire = parse(tech_param, 79, 90)
    wire.columns =['parameters','(units)', '0/0', '0/1', '0/2', '0/3', '1/0', '1/1', '1/2', '1/3' ]

    TSV_data = parse(tech_param, 91, None)
    TSV_data.columns =['parameters','(units)', '0/0', '0/1', '0/2', '0/3', '1/0', '1/1', '1/2', '1/3']

    return architecture, current, semiconductor, SRAM_data, CAM_data, DRAM_data, DRAM_cell, chip_data, wire, TSV_data

def convert(dict, file):
    for key in dict:
        dict[key].to_csv(file, mode='a', index=False, header=True, sep=' ')

def merge(file):
    dict = {names[0]:data[0]}
    initialize = {"none" : []}
    initialize = pd.DataFrame(data=initialize)
    initialize.to_csv(file, index=False, header=False, sep=' ')
    for x in range(10):
        dict.update({names[x]:data[x]})
    return dict


tech_size = "90nm"
data = partition("tech_params_control/"+ tech_size + ".dat")
tech_param_dict = merge('tech_params/' + tech_size + ".dat")
convert(tech_param_dict, 'tech_params/' + tech_size + ".dat")
os.system("./cacti -infile cache.cfg > control.txt")



outputs = ["Access time", "Cycle time", "Total dynamic read energy per access", "Total dynamic write energy per access", 
           "Total leakage power of a bank", "Total gate leakage power of a bank", "Cache height x width", "IO Area", 
           "IO Dynamic Power", "IO Termination and Bias Power"]

inputs = []
for name in tech_param_dict:
    temp = 0
    chunk = tech_param_dict[name]
    for y in range(len(chunk)):
        if not(pd.isnull(chunk.iloc[y,0])):
            if chunk.iloc[y,0] == "-I_off_n" or chunk.iloc[y,0] == "-I_g_on_n":
                for column in chunk.columns[3:]:
                    inputs.append(name + chunk.iloc[y,0] + "-" + column + "-" + chunk.iloc[y,2])
            else:
                for column in chunk.columns[2:]:
                    inputs.append(name + chunk.iloc[y,0] + "-" + column.replace( '/', ''))
# print (inputs)

table_outputs = pd.DataFrame(columns = outputs, index = inputs)
table_outputs.to_csv("final.csv", index=True, header=True)

#parse through each chunk and do it bruh
for chunkName in tech_param_dict:
    for y in range (len(tech_param_dict[chunkName])):
        folder = chunkName + str(tech_param_dict[chunkName].iloc[y,0])
        directory = "results/" + folder + "/"

        if not os.path.isdir(directory):
            os.makedirs(directory)

        if pd.isnull(input):
            continue
        
        for x in range (len(tech_param_dict[chunkName].columns)):
            
            print (folder)


            if folder == "current-I_off_n" or folder == "current-I_g_on_n":
                input = folder + "-" + str(tech_param_dict[chunkName].columns[x]) + "-" + str(tech_param_dict[chunkName].iloc[y,2])
            else:
                input = folder + "-" + str(tech_param_dict[chunkName].columns[x]).replace( '/', '')

            print (input)

            cell = tech_param_dict[chunkName].iloc[y,x]
            if not(pd.isnull(cell)) and cell[0].isdigit() and input.find("-temp") == -1:
                print(tech_param_dict[chunkName].iat[y,x])
                if tech_param_dict[chunkName].iat[y,x] == '0':
                    tech_param_dict[chunkName].iat[y,x] = '1'
                else:
                    tech_param_dict[chunkName].iat[y,x] = float_to_exponential(float(tech_param_dict[chunkName].iat[y,x])*100)
                print(tech_param_dict[chunkName].iat[y,x])
                

                # print (tech_param_dict[chunkName])
                convert(tech_param_dict, 'tech_params/' + tech_size + ".dat") #converts dictionary into .dat file

                print (subprocess.check_output("./cacti -infile cache.cfg > output.txt", shell=True, text=True))
                os.system("./cacti -infile cache.cfg > output.txt")
                # if str(subprocess.check_output("./cacti -infile cache.cfg > output.txt", shell=True, text=True)).find("Assertion failed:") != -1:
                #     print("hello")
                #     data = partition("tech_params_control/90nm.dat") #resets data tuple
                #     tech_param_dict = merge('tech_params/90nm.dat') #puts each dataframe in the dictionary, labeled
                #     convert(tech_param_dict, 'tech_params/90nm.dat') #converts dictionary into .dat file
                #     continue
                os.system("diff control.txt output.txt > " + directory + input + ".txt")

                diff = open(directory + input + ".txt")
                lines = diff.readlines()

                for word in outputs:
                    counter = 0; 
                    value = None
                    value2 = None
                    for line in lines:
                        if line.find(word) != -1 and counter == 1:
                            # print("found2")
                            value2 = substring_after(line, ":")
                            value2 = value2.replace('\n', '')
                            table_outputs.at[input,word] =  value + ", " + value2 + "\n"
                            break
                        elif line.find(word) != -1:
                            # print("found1")
                            counter = 1
                            value = substring_after(line, ":")
                            value = value.replace('\n', '')
                        elif line.find("ERROR: no valid tag organizations found") != -1:
                            # print("found1")
                            table_outputs.loc[input] = "ERROR: no valid tag organizations found"
                            break
                        else:
                            # print (table_outputs.at[input, word])
                            if pd.isnull(table_outputs.at[input, word]):
                                table_outputs.at[input, word] = " "   
                            # print (table_outputs.at[input, word])

                data = partition('tech_params_control/' + tech_size + ".dat") #resets data tuple
                tech_param_dict = merge('tech_params/' + tech_size + ".dat") #puts each dataframe in the dictionary, labeled
                convert(tech_param_dict, 'tech_params/' + tech_size + ".dat") #converts dictionary into .dat file

table_outputs.to_csv("cache.csv", index=True, header=True)
    
'''
readL: outputs dataframesss

process: change the values, write to data, call cacti
output to a different file and process that one instead 

write: input dataframes

get the table going .csv


IO Area (sq.mm) = inf
IO Dynamic Power (mW) = 1506.36 PHY Power (mW) = 232.752 PHY Wakeup Time (us) = 27.503
IO Termination and Bias Power (mW) = 2505.96


'''


# process = subprocess.Popen(bash_command.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# output, error = process.communicate()

# # Decode output and error from bytes to string
# output_str = output.decode('utf-8')
# error_str = error.decode('utf-8')

# # Print output and error
# print("Output:", output_str)


# for y in range (5):
#     for x in range (7):
#         if not(pd.isnull(data.iloc[y+1,x+2])) :
#             data = partition("idk/cacti/tech_params/90nm.dat", tech_result)
#             data.iloc[y+1,x+2] = float_to_exponential(float(data.iloc[y+1,x+2])*10)
#     # print(data)




# print (partition("idk/cacti/tech_params/90nm.dat", tech_result))
