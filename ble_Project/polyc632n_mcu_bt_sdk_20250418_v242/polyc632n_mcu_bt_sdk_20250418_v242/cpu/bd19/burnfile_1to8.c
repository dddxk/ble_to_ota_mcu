
//#注意：可以将命令行最后的文件名“pol632n_1to8_file.fw”改为自己想定义的文件名，以供一拖八烧录工具使用

cd %~dp0

burnfile_1to8_v213.exe -a unlimited -f jl_isd.fw -k jl_isd.key -o   pol632n_1to8_file.fw



