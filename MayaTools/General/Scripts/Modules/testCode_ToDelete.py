import maya.cmds as cmds
import maya.mel as mel
import os


class TEST_RIG():
    
    def __init__(self):
        
        
        #test code
        import Modules.ART_Core
        reload(Modules.ART_Core)
        coreNodes = Modules.ART_Core.RigCore()
        
        import Modules.ART_Arm
        reload(Modules.ART_Arm)
        Modules.ART_Arm.Arm(True, "", None, "l", "driver_spine_05", 6, False)
        Modules.ART_Arm.Arm(False, "", None, "r", "driver_spine_05", 13, False)
        
