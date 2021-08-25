#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys


replace_str_h = "#pragma once"
replace_str_cpp = ""
   
def replace_content(file_name):

    fsingle = file_name.strip()
    if fsingle.find(".") != -1:
        fsingle = fsingle[fsingle.rfind(".") + 1 :]
        
    if fsingle == "cpp":
        text = replace_str_cpp    
    elif fsingle == "h" or fsingle == "hpp":
        text = replace_str_h
        text += "\n\n\n"  
    else:
        return             
        
    
    # We now have the proper header, so we want to ignore the one in the original file
    # and potentially empty lines and badly formatted lines, while keeping comments that
    # come after the header, and then keep everything non-header unchanged.
    # To do so, we skip empty lines that may be at the top in a first pass.
    # In a second pass, we skip all consecutive comment lines starting with "/*",
    # then we can append the rest (step 2).
    fileread = open(file_name.strip(), "r")
    print("open file: " + file_name)
    line = fileread.readline()
    header_done = False

    while line.strip() == "":  # Skip empty lines at the top
        line = fileread.readline()
        
    if line.find("/**********") == -1:  # Godot header starts this way
        # Maybe starting with a non-Godot comment, abort header magic
        header_done = True

    if line.find(replace_str_h) != -1: 
        return

    while not header_done:  # Handle header now
        if line.find("/*") != 0 and line.find("\n") != 0 and line.find("\r\n") != 0:  # No more starting with a comment
            header_done = True
            if line.strip() != "":
                text += line
        line = fileread.readline()
             
        
    while line != "":  # Dump everything until EOF
        text += line
        line = fileread.readline()
        

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
