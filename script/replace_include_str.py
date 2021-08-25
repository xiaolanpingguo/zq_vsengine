#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys


specifical_str_1 = "#include "
specifical_str_2 = "#ifndef "
specifical_str_3 = "#define "
specifical_str_4 = "_H"

specifical_str_5 = "\n\nnamespace zq{\n\n"
specifical_str_6 = "\n\n}"
specifical_str_7 = "namespace zq{"
specifical_str_8 = "#endif"


    
def remove_underline(string):
    if string.find("_") == -1:
        return string

    new_str = ""
    for i in range(len(string)):                     
        c = string[i]
        
        if c == "_":
            continue
            
        if i == 0:
            new_str += c
            continue
            
        if i == len(string) - 1:
            new_str += c
            break
            
        last_c = string[i - 1]
        if last_c == "_":
            if c.isalpha() == True and c.islower() == True:
                new_str += c.upper()                      
            else:
                new_str += "_"
                new_str += c
        else:
            new_str += c
            
    return new_str
    
   
def replace_content(file_name):

    fsingle = file_name.strip()
    if fsingle.find(".") != -1:
        fsingle = fsingle[fsingle.rfind(".") + 1 :]
        
    # determin suffix
    if fsingle != "cpp" and fsingle != "h" and fsingle != "hpp":
        return             
        
    
    fileread = open(file_name.strip(), "r")
    print("open file: " + file_name)
  
    text_lines = fileread.readlines()
    
    # if there was "namespace zq{" 
    for line in text_lines:
        if line.find(specifical_str_7) != -1:
            return
          
    remove_num = len(text_lines)
    remove_found = False
    insert_num = 0
    insert_found = False
    i = 0
    for line in reversed(text_lines):
    
        if line.find(specifical_str_1) != -1 and insert_found == False:
            insert_found = True
            insert_num = len(text_lines) - i
            
        if line.find(specifical_str_8) != -1 and remove_found == False:
            remove_found = True
            remove_num = -i - 1
        i += 1
     
    
    # remove "#endif" for .h file
    if (fsingle == "h" or fsingle == "hpp") and remove_num != len(text_lines):
        text_lines.pop(remove_num)
       
    # add namespace zq
    if insert_num != 0:
        text_lines.insert(insert_num, specifical_str_5)
        
      
    text = ""       
    for i in range(len(text_lines)):     
        line = text_lines[i]       
        
        # found "#ifndef #ifdef"
        if line.find(specifical_str_2) != -1 and line.find(specifical_str_4) != -1: 
            next_line = text_lines[i + 1]
            if next_line.find(specifical_str_3) != -1 and next_line.find(specifical_str_4) != -1: 
                continue
            else:
                # remove "_"
                if line.find(specifical_str_1) != -1: 
                    text += line
                else:
                    text += remove_underline(line)
        elif line.find(specifical_str_3) != -1 and line.find(specifical_str_4) != -1: 
            continue
        else: 
            # remove "_"
            if line.find(specifical_str_1) != -1: 
                text += line
            else:
                text += remove_underline(line)         
                  

    text += specifical_str_6
    
    fileread.close()
    
    
    # Write
    filewrite = open(file_name.strip(), "w")
    filewrite.write(text)
    filewrite.close()
    

def list_dir(path):
    print("-----enter path: "+ path + "------")
    
    filelist=os.listdir(path)
    for file in filelist:   
        
        filename = os.path.join(path,file)
        if os.path.isdir(filename):
            list_dir(filename)
        else:  
            replace_content(filename)
            print("replace done, file: " + filename)
    
    
def replace_all():
    path=input("input dir(eg:D:\\\\picture):")
    list_dir(path)
    
    
replace_all() 
