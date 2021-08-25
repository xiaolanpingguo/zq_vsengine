import os
import sys


def get_new_name(old_file_name):
    filename = old_file_name
    filename = filename.lower()
    print(filename)
    filename = filename.replace("vs", "")
    return filename
    

def list_dir(path, renamedir = False):
    print("-----enter path: "+ path + "------")
    filelist=os.listdir(path)
    for file in filelist:
    
        newfilename = get_new_name(file) 
        newname = os.path.join(path,newfilename)
        
        filename = os.path.join(path,file)
        if os.path.isdir(filename):
            if renamedir == True:
                os.rename(filename, newname)
                print("rename dir, old: " + filename + " to new: " + newname)
                list_dir(newname) 
            else:
                list_dir(filename)    
        else:  
            newname = os.path.join(path,newfilename)
            os.rename(filename, newname)
            print("rename file, old: " + filename + " to new: " + newname)
    
    
def rename_all(renamedir = False):
    path=input("input dir(eg:D:\\\\picture):")
    list_dir(path, renamedir)
  

rename_all(True) 