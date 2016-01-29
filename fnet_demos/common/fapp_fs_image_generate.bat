@echo Off
@Title Generating ROM File System image files.

cmd /c ..\..\fnet_tools\fnet_fs_generation.exe /project=.\fapp_fs_image_project.fnetfs /force
cmd /c ..\..\fnet_tools\fnet_fs_generation.exe /project=.\fapp_fs_image_project_http09.fnetfs /force

pause