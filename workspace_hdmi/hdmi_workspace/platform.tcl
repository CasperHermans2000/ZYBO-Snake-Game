# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct D:\ES\workspace_hdmi\hdmi_workspace\platform.tcl
# 
# OR launch xsct and run below command.
# source D:\ES\workspace_hdmi\hdmi_workspace\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {hdmi_workspace}\
-hw {D:\ES\hdmi_project\design_1_wrapper.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -fsbl-target {psu_cortexa53_0} -out {D:/ES/workspace_hdmi}

platform write
platform generate -domains 
platform active {hdmi_workspace}
platform generate
platform active {hdmi_workspace}
platform generate -domains 
platform active {hdmi_workspace}
platform generate -domains 
platform generate
platform generate
