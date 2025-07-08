del /S /F /Q *.obj *.iobj *.opt *.plg *.ipch *.ncb *.pch *.VC.db *.pdb *.ipdb *.idb *.asp *.ncb *.ilk ipch* *.sdf *.tlog *.log *.aps *.bak *.lastbuildstate *.lastcodeanalysissucceeded

del *.exe


rem *** 删除编译产生的垃圾文件 ***

rmdir .\CppTest1\.vs /S /Q
rmdir .\CppTest1\Debug / S/Q
rmdir .\CppTest1\Release / S/Q
rmdir .\CppTest1\x64 / S/Q


