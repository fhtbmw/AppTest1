del /S /F /Q *.obj *.iobj *.opt *.plg *.ipch *.ncb *.pch *.VC.db *.pdb *.ipdb *.idb *.asp *.ncb *.ilk ipch* *.sdf *.tlog *.log *.aps *.bak *.lastbuildstate *.lastcodeanalysissucceeded

del *.exe


rem *** 删除编译产生的垃圾文件 ***

rmdir .\CppTest1\.vs /S /Q
rmdir .\CppTest1\Debug / S/Q
rmdir .\CppTest1\Release / S/Q
rmdir .\CppTest1\x64 / S/Q

rmdir .\elecom_hid_uninstall\.vs /S /Q
rmdir .\elecom_hid_uninstall\bin /S /Q
rmdir .\elecom_hid_uninstall\obj /S /Q

rmdir .\elecom_ui\.vs /S /Q
rmdir .\elecom_ui\elecom_hid\.vs /S /Q
rmdir .\elecom_ui\elecom_hid\bin /S /Q
rmdir .\elecom_ui\elecom_hid\obj /S /Q


rmdir .\elecom_update\.vs /S /Q
rmdir .\elecom_update\ElecomUpdate\.vs /S /Q
rmdir .\elecom_update\ElecomUpdate\bin /S /Q
rmdir .\elecom_update\ElecomUpdate\obj /S /Q

rmdir .\elecom_fw_update\FwUpdateDaemon\.vs /S /Q
rmdir .\elecom_fw_update\FwUpdateDaemon\Debug /S /Q
rmdir .\elecom_fw_update\FwUpdateDaemon\Release /S /Q
rmdir .\elecom_fw_update\FwUpdateDaemon\x64 /S /Q

rmdir .\elecom_fw_update\FwUpdateUI\fuhlen_update_fw\FuhlenUpdate_Fw\bin /S /Q
rmdir .\elecom_fw_update\FwUpdateUI\fuhlen_update_fw\FuhlenUpdate_Fw\obj /S /Q


rem *** 清除bin中的垃圾 ***

cd bin
rmdir .\Debug /S /Q
del  /S /Q /F *.dll *.exe
del *.xml

del  .\Release\*.xml
del .\Release\elecomUpdate\*.xml
del .\Release\elecomUpdate\*.config
cd ..

rem *** 从bin\Release中恢复bin\Debug ***

mkdir .\bin\Debug
xcopy /Y /S .\bin\Release\*.* .\bin\Debug

