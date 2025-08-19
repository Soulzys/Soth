@echo off

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl -MT -GR- -Gm- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -DBUILD_ASSERT=1 -FC -Z7 -Fwin32_soth.map ..\code\win32_soth.cpp user32.lib gdi32.lib opengl32.lib
popd

:: -W4 --> warning level 4 (from the compiler). Can also write -Wall --> actives all possible warnings (but then we get flooded)
:: -WX --> treats warnings as errors, i.e. cannot compile if there are warnings left. The idea is to disable the warnings we deem 
::		   meaningless / unimportant enough, and keep the warnings we want to deal with. 
:: -wd<warning code> --> disable the warning with said code name

:: -Z7 --> it used to be -Zi. Casey switched to -Z7 on day16 -> 14:10 min

:: -Oi --> let compiler do intrinsic stuff : if the compiler knows how to do stuff itself (i.e. sin), let it do it straight away

:: -GR- --> disables C++ runtime type information

:: -EHa- --> disables C++ exeption handling stuff.

:: /link -subsystem:windows,5.1 --> day16 ~34min. First step to allow us to run the .exe on Windows XP

:: -MT --> crucial stuff. Related to the runtime library. Learn more about it in day16 ~39min

:: -Gm- --> no rebuilding so we don't care (day16, ~41min)