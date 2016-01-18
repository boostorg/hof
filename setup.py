
import os

prefix = '/// '
include_dir = 'include/'
doc_dir = 'doc/src'

def insert_header_each(line, file):
    if line.startswith('=='):
        return [line, "\n", "Header\n", "------\n", "\n", "    #include <{0}>\n".format(file), "\n"]
    else:
        return [line]

def insert_header(lines, file):
    return [line for x in lines for line in insert_header_each(x, file)]

def extract_md(file):
    f = open(file)
    return [line[len(prefix):] for line in f.readlines() if line.startswith(prefix)]

for root, subdirs, files in os.walk(include_dir):
    print("root", root)
    if 'detail' not in root:
        for f in files: 
            file = os.path.join(root, f)
            include_file = file[len(include_dir):]
            md = extract_md(file)
            if len(md) > 0:
                print('Extracting:', file)
                name, ext = os.path.splitext(os.path.basename(file))
                open(os.path.join(doc_dir, name + '.md'), 'w').writelines(insert_header(md, include_file))
