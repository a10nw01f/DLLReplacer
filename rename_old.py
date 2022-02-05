import uuid
import os

with open("dll_replacer_config.txt") as file:
	name = file.readline().strip()

new_dll = name + ".dll"
old_dll = name + "_old.dll"
if os.path.exists(old_dll):
	tmp_name = name + "_" + str(uuid.uuid4()) + "_old.dll"
	os.rename(old_dll , tmp_name)

os.rename(new_dll, old_dll)