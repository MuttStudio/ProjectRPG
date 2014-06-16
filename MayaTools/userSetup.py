import maya.cmds as cmds
import os
import sys
from functools import partial
import maya.mel as mel

                
def mayaToolsInstall_UI():
    
    if cmds.window("mayaToolsInstall_UI", exists = True):
        cmds.deleteUI("mayaToolsInstall_UI")
        
    window = cmds.window("mayaToolsInstall_UI", w = 300, h = 100, title = "Maya Tools Install", mnb = False, mxb = False)
    
    mainLayout = cmds.columnLayout(w = 300, h = 100)
    formLayout = cmds.formLayout(w = 300, h = 100)
    
    text = cmds.text(label = "ERROR: Could not find Maya Tools directory.\n Please locate folder using the \'Browse\' button.", w = 300)
    cancelButton = cmds.button(label = "Cancel", w = 140, h = 50, c = mayaToolsInstall_Cancel)
    browseButton = cmds.button(label = "Browse", w = 140, h = 50, c = mayaToolsInstall_Browse)
    
    cmds.formLayout(formLayout, edit = True, af = [(text, 'left', 10), (text, 'top', 10)])
    cmds.formLayout(formLayout, edit = True, af = [(cancelButton, 'left', 5), (cancelButton, 'top', 50)])
    cmds.formLayout(formLayout, edit = True, af = [(browseButton, 'right', 5), (browseButton, 'top', 50)])
    
    cmds.showWindow(window)
    cmds.window(window, edit = True, w = 300, h = 100)
    
    
    
def mayaToolsInstall_Cancel(*args):
    
    cmds.deleteUI("mayaToolsInstall_UI")
    cmds.warning("Maya Tools will not be setup")
    


def mayaToolsInstall_Browse(*args):
    
    mayaToolsDir = cmds.fileDialog2(dialogStyle = 2, fileMode = 3)[0]

    #confirm that this is actually the maya tools directory
    if mayaToolsDir.rpartition("/")[2] != "MayaTools":
        cmds.warning("Selected directory is not valid. Please locate the MayaTools directory.")
        
        
    else:
        cmds.deleteUI("mayaToolsInstall_UI")
        
        #create file that contains this path
        path = cmds.internalVar(usd = True) + "mayaTools.txt"
        
        f = open(path, 'w')
        f.write(mayaToolsDir)
        f.close()
        
        
        #run setup
        cmds.file(new = True, force = True)
        
        
        
def mayaTools():
    
    #import custom maya setup script
    print cmds.internalVar(usd = True)
    print cmds.internalVar(upd = True)
    path = cmds.internalVar(usd = True)
    path = path + "mayaTools.txt"
    print path
    
    if os.path.exists(path):
        f = open(path, 'r')
        
        mayaToolsDir = f.readline()
        if not os.path.exists(mayaToolsDir):
            mayaToolsInstall_UI()
        
        path = mayaToolsDir + "/General/Scripts"
        pluginPath = mayaToolsDir + "/General/Plugins"
        
        #look in sys.path to see if path is in sys.path. if not, add it
        if not path in sys.path:
            sys.path.append(path)
            
            
        #run setup
        import customMayaMenu as cmm
        cmm.customMayaMenu()
        cmds.file(new = True, force = True)
    
    
    else:
        mayaToolsInstall_UI()
        
        

#setup script jobs
scriptJobNum2 = cmds.scriptJob(event = ["NewSceneOpened", mayaTools])


