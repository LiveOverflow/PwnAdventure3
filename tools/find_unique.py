import os
import hashlib
import sys

# Information Gathering / Recon - Pwn Adventure 3: part 3
# https://www.youtube.com/watch?v=pzM4o6qxssk

def tree(path):
    files = []
    for file in os.listdir(path):
        full_path = "{}/{}".format(path, file)
        if os.path.isdir(full_path):
            files += tree(full_path)
        else:
            files.append(full_path)
    return files

files = []
for arg in sys.argv[1:]:
    print 'scanning: {}'.format(arg)
    files += tree(arg)

unique_files = {}
dup_files = {}
hashes = []

for f in files:
    h = hashlib.sha1(open(f).read()).hexdigest()
    if h in unique_files:
        if h not in dup_files:
            dup_files[h] = [unique_files[h]]
        del unique_files[h]
    elif h not in dup_files:
        unique_files[h] = f
    if h in dup_files:
        dup_files[h].append(f)

print "\n[*] unique files:"

for h in unique_files:
    print h
    print " >",unique_files[h]

print "\n[*] duplicate files:"

for h in dup_files:
    print h
    for f in dup_files[h]:
        print " >",f
