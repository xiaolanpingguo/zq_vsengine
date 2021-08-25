import os
import sys


def list_dir(path, renamedir = False):
    print("-----enter path: "+ path + "------")
    filelist=os.listdir(path)
    for file in filelist:   
        
        filename = os.path.join(path,file)
        if os.path.isdir(filename):
                list_dir(filename)    
        else:  
            fsingle = filename.strip()
            if fsingle.find(".") != -1:
                fsingle = fsingle[fsingle.rfind(".") + 1 :]
                
            if fsingle == "obj" or fsingle == "lib" or file == "SCsub" or fsingle == "py":
                os.remove(filename)
                print("delete file: " + filename)   
    
    
def rename_all():
    path=input("input dir(eg:D:\\\\picture):")
    list_dir(path)
  

rename_all() 