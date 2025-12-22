local common = {}

common.build_directory =  "build/bin/%{prj.name}/%{cfg.buildcfg}"
common.object_directory = "!build/obj/%{prj.name}/%{cfg.buildcfg}"

return common