#!/usr/bin/env python

import sys, re, os, commands, time

##################
def fileCopied(fileName):
    command = "ls "+fileName.strip("file:")
    result = commands.getoutput(command)
    
    if(str(result).find("No such file or directory")!=-1 or
       str(result).find("Nie ma takiego pliku ani katalogu")!=-1):
           return False
    return True
##################
def copyFilesFromSE(sourcePath,destinationPath, user):

    destDirectory = sourcePath.split("/")[len(sourcePath.split("/"))-2]+"1"
    endpoint = sourcePath.split("SFN")[0]+"SFN="
    command = "lcg-ls -l "+sourcePath
    output = commands.getoutput(command)
    lines = output.split('\n')
    for line in lines:
        if line.count("/"+user+"/"):
            localPath = line.split()[6]            
            if line.split()[0]=='drwxrwxr-x' and (endpoint+localPath)!=sourcePath:                
                print "directory local path: ",localPath
                copyFilesFromSE(endpoint+localPath,destinationPath, user)
            else:
                destination = destinationPath+"/"+destDirectory+"/"+localPath.split("/")[len(localPath.split("/"))-1]
                destination = destinationPath+localPath.split(user)[1]
                path = (destinationPath+localPath.split(user)[1]).split("file:")[1]
                path = path[:path.rfind("/")]
                command = "mkdir -p "+path
                os.system(command)

                if localPath.find(".root")==-1:
                    continue

                if(fileCopied(destination)):
                    continue
                    
                command = "lcg-cp -v  --vo cms -b -T srmv2 -U srmv2 "+endpoint+localPath+" "+destination + " &"
                #command = "gfal-copy --force "+endpoint+localPath+" "+destination
                print "Executing command: ",command
                os.system(command)
                command = "ps aux | grep lcg-cp"
                while commands.getoutput(command).count("lcg-cp")>15:
                    time.sleep(1)
                

    #command = "srmls "+destinationPath+"/"+destDirectory
    #os.system(command)
##################
##################
## Uzywamy programu tak:
'''
grid-proxy-init
./copyFilesFromSE.py
'''
##
user = "akalinow"
#user = "apyskir"

#sourceEndpoint = "srm://se.grid.icm.edu.pl:8446/srm/managerv2?SFN=//"
sourceEndpoint = "srm://se.cis.gov.pl:8446/srm/managerv2?SFN=//"
destEndpoint = "file:./Data/"


##katalogi ktore checmy skopiowac
directories = [
    "/dpm/cis.gov.pl/home/cms/store/user/akalinow/v28/"
]

## Mozemy kopiowac zawartosc wielu katalogow. Pliki sa kopiowane do katalogow
## o tej samej strukturze so na SE, poczawszy od katalogu wystepujacego po 
## nazwie uzytkownika
for dir in directories:
    copyFilesFromSE(sourceEndpoint+dir,destEndpoint, user)


