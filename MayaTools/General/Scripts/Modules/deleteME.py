
#ARM ROLLS
"""

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#setup roll bones if obj Exists
if cmds.objExists("driver_upperarm_twist_01_" + side):
    self.buildArmRoll(side)
    
if cmds.objExists("driver_lowerarm_twist_01_" + side):
    self.buildForearmTwist(side)
    
#colorize controls, cleanup attrs, and cleanup hierarchy
for attr in [".sx", ".sy", ".sz", ".v"]:
    cmds.setAttr(ikCtrl + attr, lock = True, keyable = False)
    
for attr in [".sx", ".sy", ".sz", ".rx", ".ry", ".rz", ".v"]:
    cmds.setAttr(ikPvCtrl + attr, lock = True, keyable = False)
    
    
for control in [ikCtrl, ikPvCtrl]:
    if side == "l":
        color = 6
        
    else:
        color = 13

    cmds.setAttr(control + ".overrideEnabled", 1)
    cmds.setAttr(control + ".overrideColor", color)
"""