import maya.cmds as cmds
from functools import partial
import os, cPickle, math
import maya.mel as mel
import maya.utils

class multiCharacterFBX():
    def __init__(self):
	
	#get start/end/path
	
	startFrame = cmds.playbackOptions(q = True, min = True)
	endFrame = cmds.playbackOptions(q = True, max = True)
	fileName = cmds.file(q = True, sceneName = True)
	path = fileName.rpartition(".")[0]
	
	
	#get the characters in the scene
	characters = self.getCharacters()
	
	#get rotation interp
	options = cmds.optionVar(list = True)
	for op in options:
	    if op == "rotationInterpolationDefault":
		interp = cmds.optionVar(q = op)	
	cmds.optionVar(iv = ("rotationInterpolationDefault", 3))
	
	
	#Loop through each character in the scene, and export the fbx for that character
	
	#create increment ammount for progress bar
	increment = 50/len(characters)
	step = increment/5	

	for character in characters:
	    

	    #add character suffix to fbx path file
	    exportPath = os.path.normcase(path).rpartition(os.sep)[0] 
	    name = os.path.normcase(path).rpartition(os.sep)[2]
	    name = name +  "_" + character + ".fbx"
	    exportPath = os.path.join(exportPath,  name)
	    
	    #get blendshapes
	    allBlends = cmds.ls(type = "blendShape")
	    jmBlends = ["calf_l_shapes",  "calf_r_shapes",  "head_shapes",  "l_elbow_shapes",  "r_elbow_shapes",  "l_lowerarm_shapes",  "r_lowerarm_shapes",  "l_shoulder_shapes",  "r_shoulder_shapes",  "l_upperarm_shapes",  "r_upperarm_shapes",
		     "neck1_shapes",  "neck2_shapes",  "neck3_shapes",  "pelvis_shapes",  "spine1_shapes",  "spine2_shapes",  "spine3_shapes",  "spine4_shapes",  "spine5_shapes",  "thigh_l_shapes",  "thigh_r_shapes"]
	    blendshapes = []
	    
	    if allBlends != None:
		for shape in allBlends:
		    if shape.partition(":")[2] not in jmBlends:
			if shape.partition(":")[0] == character:
			    blendshapes.append(shape)
	    
	    #if our character has blendshapes, deal with those now
	    if blendshapes != None:
		cube = cmds.polyCube(name = "custom_export")[0]
		
		for shape in blendshapes:
		    try:
			attrs = cmds.listAttr(shape, m = True, string = "weight")
			print attrs
			#counter for index
			x = 1
			
			for attr in attrs:
			    morph = cmds.polyCube(name = attr)[0]
			    if cmds.objExists("custom_export_shapes"):
				cmds.blendShape("custom_export_shapes", edit = True, t = (cube, x, morph, 1.0))
				
			    else:
				cmds.select([morph, cube], r = True)
				cmds.blendShape(name = "custom_export_shapes")
				cmds.select(clear = True)
				
			    cmds.delete(morph)
			    
			    #transfer animation from original to new morph
			    for i in range(int(startFrame), int(endFrame) + 1):
				cmds.currentTime(i)
				value = cmds.getAttr(shape + "." + attr)
				print i, shape, attr, value
				cmds.setKeyframe("custom_export_shapes." + attr, t = (i), v = value)
				
			    x = x + 1	
		    
		    except Exception, e:
			print str(e)
		



	    #duplicate the skeleton
	    newSkeleton = cmds.duplicate(character + ":" + "root", un = False, ic = False)
	    
	    joints = []
	    for each in newSkeleton:
		if cmds.nodeType(each) != "joint":
		    cmds.delete(each)
		    
		else:
		    joints.append(each)
				
	    
	    #constrain the dupe skeleton to the original skeleton
	    constraints = []
	    for joint in joints:
		#do some checks to make sure that this is valid
		parent = cmds.listRelatives(joint, parent = True)
    
		if parent != None:
		    if parent[0] in joints:
			constraint = cmds.parentConstraint(character + ":" + parent[0] + "|" + character + ":" + joint, joint)[0]
			constraints.append(constraint)
			
		else:
		    #root bone?
		    if joint == "root":
			constraint = cmds.parentConstraint(character + ":" + joint, joint)[0]
			constraints.append(constraint)			    
    
    
	    #bake the animation down onto the export skeleton
	    cmds.select("root", hi = True)
	    cmds.bakeResults(simulation = True, t = (startFrame, endFrame))
	    cmds.delete(constraints)
			    
	    

	    
	    #run an euler filter
	    cmds.select("root", hi = True)
	    cmds.filterCurve()
			
	    
	    #export selected
	    #first change some fbx properties
	    string = "FBXExportConstraints -v 1;"
	    string += "FBXExportCacheFile -v 0;"
	    mel.eval(string)
			    
	    cmds.select("root", hi = True)
	    if cmds.objExists("custom_export"):
		cmds.select("custom_export", add = True)
    
		
	    cmds.file(exportPath, es = True, force = True, prompt = False, type = "FBX export")
	    
	    #clean scene
	    cmds.delete("root")
	    if cmds.objExists("custom_export"):
		cmds.delete("custom_export")
		
	    
	    
	#reset rotation interp
	cmds.optionVar(iv = ("rotationInterpolationDefault", interp))	
	
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
    def getCharacters(self):
        
        referenceNodes = []
        references = cmds.ls(type = "reference")
	
        
        for reference in references:
            niceName = reference.rpartition("RN")[0]
            suffix = reference.rpartition("RN")[2]
            if suffix != "":
                if cmds.objExists(niceName + suffix + ":" + "Skeleton_Settings"):
                    referenceNodes.append(niceName + suffix)
                    
            else:
                if cmds.objExists(niceName + ":" + "Skeleton_Settings"):
                    referenceNodes.append(niceName)
                
        return referenceNodes